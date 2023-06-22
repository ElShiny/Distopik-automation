/*
 * settings.c
 *
 * Created: 14/04/2023 15:29:10
 *  Author: Matej
 */ 


#include "settings.h"
#include "SPI.h"
#include "housekeeping.h"
#include <avr/interrupt.h>





int parseSPI(spi_t *spi, buffer_t *buffer, hskp_t  *hskp){
	
	if(buffer->buffer_length == 0)return 0;
	if(readBuffer(buffer) != RECIEVE_KEYWORD)return 0;

	//PORTB ^= 1<<PORTB6;
	//disableHSKP();
	int start_tick = getTick(hskp);
	
	parsing_state = PARSING_INSTR;
	while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
	int cmd = readBuffer(buffer);

	switch(cmd){
		
		case 0: //do nothing
			break;
		
		case 1: //write ace value
		
			break;
		
		case 2: //get ace value
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			set_ladder(readBuffer(buffer), 0);
			break;
		
		case 3: //get ace value
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			set_ladder(readBuffer(buffer), 1);
			break;
			
		case 20:// LED mode
			break;
			
		case 21:// set LED buffer
			break;

		case 22:// read LED buffer
			break;
		case 23:// set start led

			break;
		case 24:// set stop led

			break;
		case 25:// set front color

			break;
		case 26:// set back color
			break;
			
			
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
			writeSpi(spi, hskp, 255, UNIREL_POT_MINI, 100);
			break;
		default: bufferInit(buffer);
		
	}
	//bufferInit();
	//PORTB ^= 1<<PORTB6;
	return 0;	
}

void errorHandler(void){
	cli();

	while(1);
	
}
