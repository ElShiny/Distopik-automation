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





int parseSPI(spi_t *spi, adc_t *adc, buffer_t *buffer, hskp_t *hskp){
	
	if(buffer->buffer_length == 0)return 0;
	if(readBuffer(buffer) != RECIEVE_KEYWORD)return 0;

	int start_tick = getTick(hskp);
	
	parsing_state = PARSING_INSTR;
	while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
	int cmd = readBuffer(buffer);
	
	switch(cmd){
		
		case 0: //do nothing
			break;
		
		case 1: //write ace value
			writeSpi(&spi_s, &housekp, 1, (uint8_t)(adc->pot_pos>>2), 10);
			break;
		
		case 2: //get ace value
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			adc->pot_pos = readBuffer(buffer);
			break;
		
		case 3: //get ace value
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			//led_settings.ace_en = readBuffer();
			break;
			
		case 20:// LED mode
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT))return -1;}
			//led_settings.mode = readBuffer();
			break;
			
		case 252://report current buffer
			soft_reset();
			break;
		case 253://report current buffer
			writeSpi(&spi_s, &housekp, 253, readBufferLength(buffer), 100);
			break;
		
		case 254://report current buffer
			writeSpiBuffer(&spi_s, &housekp, 254, buffer, BUFFER_SIZE, 100);
			break;
					
		case 255:
			writeSpi(&spi_s, &housekp, 255, MOTORISED_POTT, 100);
			break;
		default: bufferInit(buffer);
		
	}
	return 0;	
}

void errorHandler(void){
	cli();
	while(1);
}
