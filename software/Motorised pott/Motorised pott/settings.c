/*
 * settings.c
 *
 * Created: 14/04/2023 15:29:10
 *  Author: Matej
 */ 


#include "settings.h"
#include "SPI.h"
#include "ADC.h"
#include "housekeeping.h"
#include <avr/interrupt.h>





int parseSPI(void){
	
	if(buffer_length == 0)return 0;

	//PORTB ^= 1<<PORTB6;
	//disableHSKP();
	int start_tick = getTick();
	
	parsing_state = PARSING_INSTR;
	int cmd = readBuffer();
	
	switch(cmd){
		
		case 0: //do nothing
			break;
		
		case 1: //write ace value
			writeSpi(1, pot_pos, 10);
			break;
		
		case 2: //get ace value
			while(readBufferLength() == 0){if(getTick()>(start_tick+MAX_TIMEOUT))return -1;}
			pot_pos = readBuffer();
			break;
		
		case 3: //get ace value
			while(readBufferLength() == 0){if(getTick()>(start_tick+MAX_TIMEOUT))return -1;}
			//led_settings.ace_en = readBuffer();
			break;
			
		case 20:// LED mode
			while(readBufferLength() == 0){if(getTick()>(start_tick+MAX_TIMEOUT))return -1;}
			//led_settings.mode = readBuffer();
			break;
			
		case 21:// set LED buffer
			//bufToRGBArray(&led_settings);
			//writeSpi(101, 0xaa, 100);
			break;

		case 22:// read LED buffer
			//writeSpiBuffer(22, led_settings.rgb_array, 90, 100);
			break;
		//case 23:// set LED 
			
		case 252://report current buffer
			soft_reset();
			break;
		case 253://report current buffer
			writeSpi(253, readBufferLength(), 100);
			break;
		
		case 254://report current buffer
			writeSpiBuffer(254, buffer, BUFFER_SIZE, 100);
			break;
					
		case 255:
			writeSpi(255, MOTORISED_POTT, 10);
			break;
		default: bufferInit();
		
	}
	bufferInit();
	//PORTB ^= 1<<PORTB6;
	return 0;	
}

void errorHandler(void){
	cli();
	//setDEMOLEDRgb(30);
	while(1);
	
}
