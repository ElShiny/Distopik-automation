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
#include "SPI.h"
#include "ADC.h"
#include "housekeeping.h"
#include "settings.h"





int main(void)
{
	MCUSR = 0;//disable watchdog
	wdt_disable();

	ADCInit();
	timersInit();
	SPIInit();
	bufferInit();
	
	sei();		
	_delay_ms(100);
	

	
		
		
	while (1){
		
		parseSPI();
		
		if(adc_changed){
			writeSpi(1, adc_val_new, 10);
			adc_changed = 0;
		}
		//_delay_ms(50);
	}

	while(1); //shouldnt reach this
}

