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





int main(void)
{
	ACEInit();
	I2CInit();
	LEDInit();
	SPIInit();
	bufferInit();
	
	sei();
	
	disableTimer();

	while(1){
					
		//_noop
		//writeSpiBuffer(20, buffer, 15);
		//_delay_ms(10);
		//writeSpi(10, buffer_length);

 		if (buffer_length != 0){
			disableTimer();
			 _delay_ms(1);
 			int data = readBuffer();
 
 			if(data == 15){
				
				int length=0;
				int start=0;
				
				while(buffer_length == 0);
				length = readBuffer();
				while(buffer_length == 0);
				start = readBuffer();
				
				while(buffer_length != length);
				
				//setLEDRgb(length);
				ace_val = length;
				if (length <= 90){
				
					for(int i = start; i< length; i++){
						rgb_array[i] = readBuffer();
						//setLEDRgb(i);
					}
				}
				
				writeSpiBuffer(20, rgb_array, length);
				writeSpi(10, buffer_length);
				
			}
			else if(data == 30){
				//writeSpi(10, 0xff);
				uint8_t d[] = {10, 20, 30, 40};
				writeSpiBuffer(20, d, 4);
			}
			bufferInit();
			enableTimer();
		}
		enableTimer();
 		if(ace_changed){
 			writeSpi(10, ace_val);
 			ace_changed = 0;
 		}	
	}
}

