/*
 SoftwareSerial.c (formerly SoftwareSerial.cpp, formerly NewSoftSerial.c) -
 Single-instance software serial library for ATtiny, modified from Arduino SoftwareSerial.
 -- Interrupt-driven receive and other improvements by ladyada
 (http://ladyada.net)
 -- Tuning, circular buffer, derivation from class Print/Stream,
 multi-instance support, porting to 8MHz processors,
 various optimizations, PROGMEM delay tables, inverse logic and
 direct port writing by Mikal Hart (http://www.arduiniana.org)
 -- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
 -- 20MHz processor support by Garrett Mace (http://www.macetech.com)
 -- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)
 -- Port to ATtiny84A / C by Michael Shimniok (http://www.bot-thoughts.com/)

Notes on the ATtiny84A port. To save space I've:
 - Converted back to C
 - Removed the ability to have mulitple serial ports,
 - Hardcoded the RX pin to PA0 and TX pin to PA1
 - Using & mask versus modulo (%)
 - A few other tweaks to get the code near 1k
More notes:
 - Converted from Arduinoish library stuff (pins etc)
 - Error is too high at 57600 (0.64%) and 115200 (2.12%)
 - Ok at 38400 and lower.
 - Still working out how to prevent missing bits when writing characters

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

//
// Defines
//
#define true 1
#define false 0
#define HIGH 1
#define LOW 0

// 
// Includes
// 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "SoftwareSerial.h"
#include "stdarg.h"
#include <stdio.h>
#include "SPI.h"
#include "ACE.h"
//
// Globals
//

uint16_t _rx_delay_centering = 0;
uint16_t _rx_delay_intrabit = 0;
uint16_t _rx_delay_stopbit = 0;
uint16_t _tx_delay = 0;

uint16_t _buffer_overflow = false;

// static data
static char _receive_buffer[_SS_MAX_RX_BUFF];
static volatile uint8_t _receive_buffer_tail;
static volatile uint8_t _receive_buffer_head;
static uint8_t was_set;
//static SoftwareSerial *active_object;

// private methods
#define rx_pin_read() (SERPIN & (1<<RXPIN))

// private static method for timing
static inline void tunedDelay(uint16_t delay);

//
// Lookup table
//
typedef struct _DELAY_TABLE {
	long baud;
	unsigned short rx_delay_centering;
	unsigned short rx_delay_intrabit;
	unsigned short rx_delay_stopbit;
	unsigned short tx_delay;
} DELAY_TABLE;



static const DELAY_TABLE table[] PROGMEM =
{
	//  baud    rxcenter    rxintra    rxstop  tx
	{ 115200,   1,          5,         5,      3,      },
	{ 57600,    1,          15,        15,     13,     },
	{ 38400,    2,          25,        26,     23,     },
	{ 31250,    7,          32,        33,     29,     },
	{ 28800,    11,         35,        35,     32,     },
	{ 19200,    20,         55,        55,     52,     },
	{ 14400,    30,         75,        75,     72,     },
	{ 9600,     50,         114,       114,    121,    },
	{ 4800,     110,        233,       233,    230,    },
	{ 2400,     229,        472,       472,    469,    },
	{ 1200,     467,        948,       948,    945,    },
	{ 600,      948,        1895,      1895,   1890,   },
	{ 300,      1895,       3805,      3805,   3802,   },
};

const int XMIT_START_ADJUSTMENT = 6;



//
// Private methods
//

/* static */
inline void tunedDelay(uint16_t delay) {
	
	uint8_t tmp = 0;

	asm volatile("sbiw    %0, 0x01 \n\t"
			"ldi %1, 0xFF \n\t"
			"cpi %A0, 0xFF \n\t"
			"cpc %B0, %1 \n\t"
			"brne .-10 \n\t"
			: "+w" (delay), "+a" (tmp)
			: "0" (delay)
	);
}


//
// Interrupt handling, receive routine
//
ISR(PCINT0_vect) {
			if((PINB & 1<<PINB2) && was_set==1){
				cli();
				
				writeBuffer(SPDR);
				PORTB ^= 1<<PORTB6;
				
				sei();
				was_set = 0;
				//ace_val --;
				return;
			}
			else if(!(PINB & 1<<PINB2)){
				was_set = 1;
				ace_val ++;
				return;
			}
	uint8_t d = 0;

	// If RX line is high, then we don't see any start bit
	// so interrupt is probably not for us
	if ( !rx_pin_read() ) {
		// Wait approximately 1/2 of a bit width to "center" the sample
		tunedDelay(_rx_delay_centering);

		// Read each of the 8 bits
		for (uint8_t i = 0x1; i; i <<= 1) {
			tunedDelay(_rx_delay_intrabit);
			uint8_t noti = ~i;
			if (rx_pin_read())
				d |= i;
			else // else clause added to ensure function timing is ~balanced
				d &= noti;
		}

		// skip the stop bit
		tunedDelay(_rx_delay_stopbit);

		// if buffer full, set the overflow flag and return
		if (((_receive_buffer_tail + 1) & _SS_RX_BUFF_MASK) != _receive_buffer_head) {  // circular buffer
			// save new data in buffer: tail points to where byte goes
			_receive_buffer[_receive_buffer_tail] = d; // save new byte
			_receive_buffer_tail = (_receive_buffer_tail + 1) & _SS_RX_BUFF_MASK;  // circular buffer
		} else {
			_buffer_overflow = true;
		}
	}
	

}




//
// Public methods
//

void softSerialBegin(long speed) {
	unsigned i;

	_receive_buffer_head = _receive_buffer_tail = 0;
	_buffer_overflow = false;
	SERDDR |= (1<<TXPIN); // set TX for output
	SERDDR &= ~(1<<RXPIN); // set RX for input
	SERPORT |= (1<<TXPIN)|(1<<RXPIN); // assumes no inverse logic

	for (i = 0; i < sizeof(table) / sizeof(table[0]); ++i) {
		long baud = pgm_read_dword(&table[i].baud);
		if (baud == speed) {
			_rx_delay_centering = pgm_read_word(&table[i].rx_delay_centering);
			_rx_delay_intrabit = pgm_read_word(&table[i].rx_delay_intrabit);
			_rx_delay_stopbit = pgm_read_word(&table[i].rx_delay_stopbit);
			_tx_delay = pgm_read_word(&table[i].tx_delay);
			// Set up RX interrupts, but only if we have a valid RX baud rate
			PCICR |= (1<<PCIE0);
			PCMSK0 |= (1<<RXPIN);
			tunedDelay(_tx_delay);
			sei();
			return;
		}
	}

	// No valid rate found
	// Indicate an error
}

void softSerialEnd() {
	PCMSK0 = 0;
}

// Read data from buffer
int softSerialRead() {
	// Empty buffer?
	if (_receive_buffer_head == _receive_buffer_tail)
		return -1;

	// Read from "head"
	uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
	_receive_buffer_head = (_receive_buffer_head + 1) & _SS_RX_BUFF_MASK; // circular buffer
	return d;
}

int softSerialAvailable() {
	return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) & _SS_RX_BUFF_MASK; // circular buffer
}

bool softSerialOverflow(void) {
	bool ret = _buffer_overflow;
	_buffer_overflow = false;
	return ret;
}

size_t softSerialWrite(uint8_t b) {
	if (_tx_delay == 0) {
		//setWriteError();
		return 0;
	}

	uint8_t oldSREG = SREG; // store interrupt flag
	cli();	// turn off interrupts for a clean txmit

	// Write the start bit
	SERPORT &= ~(1<<TXPIN); // tx pin low
	tunedDelay(_tx_delay + XMIT_START_ADJUSTMENT);

	// Write each of the 8 bits
	for (byte mask = 0x01; mask; mask <<= 1) {
		if (b & mask) // choose bit
			SERPORT |= (1<<TXPIN); // tx pin high, send 1
		else
			SERPORT &= ~(1<<TXPIN); // tx pin low, send 0

		tunedDelay(_tx_delay);
	}
	SERPORT |= (1<<TXPIN); // tx pin high, restore pin to natural state

	//sei();
	SREG = oldSREG; // turn interrupts back on
	tunedDelay(_tx_delay);

	return 1;
}

void softSerialFlush() {
	uint8_t oldSREG = SREG; // store interrupt flag
	cli();
	_receive_buffer_head = _receive_buffer_tail = 0;
	SREG = oldSREG; // restore interrupt flag
	//sei();
}


int softSerialPeek() {
	// Empty buffer?
	if (_receive_buffer_head == _receive_buffer_tail)
		return -1;

	// Read from "head"
	return _receive_buffer[_receive_buffer_head];
}

void debug_printf(const char *format, ...)
{
	char buffer[50];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	for(int i = 0; buffer[i] != 0 ; i++)
	{
		softSerialWrite(buffer[i]);

	}
	va_end(args);
}

