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
#include "I2C.h"
#include "SPI.h"
#include "housekeeping.h"
#include "settings.h"

//ace_t ace_rot;



int main(void)
{
	MCUSR = 0;//disable watchdog
	wdt_disable();
	
	timersInit(&housekp);
	I2CInit();
	SPIInit();
	bufferInit(&buf);
	
	sei();
	
					
	_delay_ms(100);
	//DDRB |= 1<<DDB6;
	//PORTB |= 1<<PORTB6;
		MCP_init();
		
		
	while (1){
		
		if(parseSPI(&spi_s, &buf, &housekp) == -1)errorHandler();
		//_delay_ms(20);
	}

	while(1); //shouldnt reach this
}

