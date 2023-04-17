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
#include "ACE.h"
#include "I2C.h"
#include "led_drv.h"
#include "SPI.h"
#include "housekeeping.h"





int main(void)
{
	ACEInit();
	timersInit();
	I2CInit();
	LEDInit();
	SPIInit();
	bufferInit();
	
	sei();
	
					
		_delay_ms(100);
		
		writeSpi(10, buffer_length, 10);
		//setLEDRgb(30);
		int start_tick = 0;
		while(buffer_length != 3){if(getTick()>start_tick+1000)break;};
		writeSpi(10, buffer_length, 10);
		setLEDRgb(90);

	while(1); //shouldnt reach this
}

