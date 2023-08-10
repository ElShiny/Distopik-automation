/*
 * Led ring ACE.c
 *
 * Created: 31/03/2023 13:46:39
 * Author : ElShiny
 */ 

#ifndef F_CPU
#define  F_CPU 8000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "ACE.h"
#include "I2C.h"
#include "led_drv.h"
#include "SPI.h"
#include "housekeeping.h"
#include "settings.h"

//ace_t ace_rot;



int main(void)
{
	MCUSR = 0;//disable watchdog
	wdt_disable();
	
	SPIInit();
	timersInit(&housekp);
	ACEInit(&ace_rot);
	I2CInit();
	LEDInit(&led_settings);
	bufferInit(&buf);
	
	sei();
	
					
	_delay_ms(100);
	//DDRB |= 1<<DDB6;
	//PORTB |= 1<<PORTB6;
		
		
	while (1){
		
		if(buf.buffer_length){
			recieveStateMachine(&spi_s, &ace_rot, &led_settings, &buf, &housekp);
		}
		//_delay_ms(20);
		if(ace_rot.ace_changed){
			ace_rot.ace_changed = 0;
			writeSpi(&spi_s, &housekp, 1, ace_rot.ace_val, 10);
		}
	}

	while(1); //shouldnt reach this
}

