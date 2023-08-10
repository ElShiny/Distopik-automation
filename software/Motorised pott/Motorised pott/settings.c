/*
 * settings.c
 *
 * Created: 14/04/2023 15:29:10
 *  Author: Matej
 */ 

#ifndef F_CPU
#define  F_CPU 8000000
#endif


#include "settings.h"
#include "SPI.h"
#include "ADC.h"
#include "housekeeping.h"
#include <avr/interrupt.h>
#include <util/delay.h>


int parse_tick;
enum setting_states_e current_state = 0;

int parseData(spi_t *spi, adc_t *adc, buffer_t *buffer, hskp_t *hskp, uint8_t cmd){
	
	int start_tick = getTick(hskp);
	uint32_t data;
	
	switch(cmd){
		
		case 1: //send potentiometer position
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT)){current_state = WAIT_BLOCK; return -1;}}
			if(readBuffer(buffer) != END_DATA){current_state = WAIT_BLOCK; return -2;}
			writeSpi(&spi_s, &housekp, 1, adc->pot_pos, 50);
			current_state = FINISHED_INSTR;
			break;
		
		case 2: //get new position
			data = assembleU16(buffer, hskp, start_tick);
			if(data < 0)return -1;
			writeSpi(&spi_s, &housekp, 2, adc->pot_pos, 50);
			adc->pot_pos = data;
			adc->spi_changed = 1;
			current_state = FINISHED_INSTR;
			break;
			
		case 100://reset machine
			soft_reset();
			break;
			
		case 101://report current buffer length
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT)){current_state = WAIT_BLOCK; return -1;}}
			if(readBuffer(buffer) != END_DATA){current_state = WAIT_BLOCK; return -2;}
			writeSpi(&spi_s, &housekp, 253, readBufferLength(buffer), 100);
			current_state = FINISHED_INSTR;
			break;
		
		case 102://report current buffer
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT)){current_state = WAIT_BLOCK; return -1;}}
			if(readBuffer(buffer) != END_DATA){current_state = WAIT_BLOCK; return -2;}
			writeSpiBuffer(&spi_s, &housekp, 254, buffer->buffer, BUFFER_SIZE, 100);
			current_state = FINISHED_INSTR;
			break;
					
		case 127://send machine type
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT)){current_state = WAIT_BLOCK; return -1;}}
			if(readBuffer(buffer) != END_DATA){current_state = WAIT_BLOCK; return -2;}
			writeSpi(&spi_s, &housekp, 255, MOTORISED_POTT, 1000);
			current_state = FINISHED_INSTR;
			break;
		default: return 0;
		
	}
	return 0;
}

int recieveStateMachine(spi_t *spi, adc_t *adc, buffer_t *buffer, hskp_t *hskp){
	uint8_t data = 0;
	switch(current_state){
		
		case WAIT_BLOCK:
			if(readBuffer(buffer) == BLOCK_SPI){current_state = WAIT_INSTR;}
			break;
		case WAIT_INSTR:
			data = readBuffer(buffer);
			if(data == 0 || data == BLOCK_SPI) break;
			if(data == UNBLOCK_SPI){current_state = WAIT_BLOCK; break;}
			if(data > 127){current_state = WAIT_BLOCK; break;}
			if(parseData(spi, adc, buffer, hskp, data)<0){current_state = STATE_ERROR; break;}
			break;
		case FINISHED_INSTR: 
			current_state = WAIT_BLOCK;
		case STATE_ERROR:
			PORTB^=1<<PINB6;
			current_state = WAIT_BLOCK;
			break;
	}
			
			
			
	return 0;
}

uint16_t uint4TOuint16(uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3){
	return ((uint16_t)data3<<12)|((uint16_t)data2<<8)|((uint16_t)data1<<4)|(uint16_t)data0;
}

int32_t assembleU16(buffer_t *buffer, hskp_t *hskp, int tick ){
	uint8_t data0,data1,data2,data3;
	
	while(readBufferLength(buffer) < 5){if(getTick(hskp)>(tick+MAX_TIMEOUT)){current_state = WAIT_BLOCK; return -1;}}
	data0 = readBuffer(buffer);
	data1 = readBuffer(buffer);
	data2 = readBuffer(buffer);
	data3 = readBuffer(buffer);
	if(readBuffer(buffer) != END_DATA){current_state = WAIT_BLOCK; return -2;}
	return uint4TOuint16(data0, data1, data2, data3);
}

void errorHandler(void){
	cli();
	while(1);
}