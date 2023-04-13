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

	while(1){

		int data = readBuffer();
		if (data != -1){
			disableTimer();
			if(data == 15){
				while((data = readBuffer()) == -1);
				ace_val = data;
				//writeSpi(ace_val);
			}
			else if(data == 30){
				writeSpi(ace_val);
			}
		}
		enableTimer();
// 		if(ace_changed){
// 			writeSpi(0xcc);
// 			writeSpi(ace_val);
// 			ace_changed = 0;
// 		}
	}
	
}

