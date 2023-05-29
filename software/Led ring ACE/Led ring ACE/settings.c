/*
 * settings.c
 *
 * Created: 14/04/2023 15:29:10
 *  Author: Matej
 */ 


#include "settings.h"
#include "SPI.h"
#include "ACE.h"
#include "led_drv.h"
#include "housekeeping.h"
#include <avr/interrupt.h>





int parseSPI(spi_t *spi, ace_t *ace, led_drv_t *led_set, buffer_t *buffer, hskp_t *hskp){
	
	if(buffer->buffer_length == 0)return 0;
	if(readBuffer(buffer) != 0xcf)return 0;
	ace->ace_val = 10;

	//PORTB ^= 1<<PORTB6;
	//disableHSKP();
	int start_tick = getTick(hskp);
	
	parsing_state = PARSING_INSTR;
	while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
	int cmd = readBuffer(buffer);
	ace->ace_val = 20;
	switch(cmd){
		
		case 0: //do nothing
			break;
		
		case 1: //write ace value
			writeSpi(spi, hskp, 1, ace->ace_val, 10);
			break;
		
		case 2: //get ace value
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			ace->ace_val = readBuffer(buffer);
			break;
		
		case 3: //get ace value
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			led_set->ace_en = readBuffer(buffer);
			break;
			
		case 20:// LED mode
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			led_set->mode = readBuffer(buffer);
			break;
			
		case 21:// set LED buffer
			bufToRGBArray(led_set, hskp);
			//writeSpi(101, 0xaa, 100);
			break;

		case 22:// read LED buffer
			writeSpiBuffer(spi, hskp, 22, led_set->rgb_array, 90, 100);
			break;
		//case 23:// set LED 
			
		case 252://report current buffer
			soft_reset();
			break;
		case 253://report current buffer
			writeSpi(spi, hskp, 253, readBufferLength(buffer), 100);
			break;
		
		case 254://report current buffer
			writeSpiBuffer(spi, hskp, 254, buffer->buffer, BUFFER_SIZE, 100);
			break;
					
		case 255:
			writeSpi(spi, hskp, 255, ACE_LED_RING, 100);
			break;
		default: bufferInit(buffer);
		
	}
	//bufferInit();
	//PORTB ^= 1<<PORTB6;
	return 0;	
}

void errorHandler(void){
	cli();
	setDEMOLEDRgb(90);
	while(1);
	
}
