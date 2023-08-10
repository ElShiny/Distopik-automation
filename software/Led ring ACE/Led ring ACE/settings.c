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



int parse_tick;
enum setting_states_e current_state = 0;

int parseData(spi_t *spi, ace_t *ace, led_drv_t *led_set, buffer_t *buffer, hskp_t *hskp, uint8_t cmd){
	
	int start_tick = getTick(hskp);
	uint32_t data;
	
	switch(cmd){
		
		case 1: //send ace value
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT)){current_state = WAIT_BLOCK; return -1;}}
			if(readBuffer(buffer) != END_DATA){current_state = WAIT_BLOCK; return -2;}
			writeSpi(spi, hskp, 1, ace->ace_val, 50);
			current_state = FINISHED_INSTR;
			break;
		
		case 2: //set ace value
			data = assembleU16(buffer, hskp, start_tick);
			if(data < 0)return -1;
			ace->ace_val = data;
			ace->ace_led_changed = 1;
			ace->ace_changed = 1;
			current_state = FINISHED_INSTR;
			break;
		
		case 3: //led enable
			data = assembleU16(buffer, hskp, start_tick);
			if(data < 0)return -1;
			led_set->led_en = data;
			current_state = FINISHED_INSTR;
			break;
			
		case 20:// LED mode
			data = assembleU16(buffer, hskp, start_tick);
			if(data < 0)return -1;
			led_set->mode = data;
			current_state = FINISHED_INSTR;
			break;
			
// 		case 21:// set LED buffer //not yet fixed
// 			bufToRGBArray(led_set, hskp);
// 			//writeSpi(101, 0xaa, 100);
// 			break;

		case 22:// read LED buffer
			while(readBufferLength(buffer) == 0){if(getTick(hskp)>(start_tick+MAX_TIMEOUT)){current_state = WAIT_BLOCK; return -1;}}
			if(readBuffer(buffer) != END_DATA){current_state = WAIT_BLOCK; return -2;}
			writeSpiBuffer(spi, hskp, 22, led_set->rgb_array, 90, 100);
			current_state = FINISHED_INSTR;
			break;
			
		case 23:// set start led angle
			data = assembleU16(buffer, hskp, start_tick);
			if(data < 0)return -1;
			led_set->start_led = data;
			ace->ace_led_changed = 1;
			current_state = FINISHED_INSTR;
			break;
			
		case 24:// set stop led angle
			data = assembleU16(buffer, hskp, start_tick);
			if(data < 0)return -1;
			led_set->stop_led = data;
			ace->ace_led_changed = 1;
			current_state = FINISHED_INSTR;
			break;
		case 25:// set front color
			bufToRGB(led_set->front_color, buffer, hskp, start_tick);
			ace->ace_led_changed = 1;
			current_state = FINISHED_INSTR;
			break;
		case 26:// set back color
			bufToRGB(led_set->back_color, buffer, hskp, start_tick);
			ace->ace_led_changed = 1;
			current_state = FINISHED_INSTR;
			break;
			
			
		case 100://report current buffer
			soft_reset();
			break;
		case 101://report current buffer
			writeSpi(spi, hskp, 253, readBufferLength(buffer), 100);
			break;
		
		case 102://report current buffer
			writeSpiBuffer(spi, hskp, 254, buffer->buffer, BUFFER_SIZE, 100);
			break;
					
		case 127:
			writeSpi(spi, hskp, 255, ACE_LED_RING, 1000);
			break;
		default: bufferInit(buffer);
		
	}
	//bufferInit();
	//PORTB ^= 1<<PORTB6;
	return 0;	
}

int recieveStateMachine(spi_t *spi, ace_t *ace, led_drv_t *led_set, buffer_t *buffer, hskp_t *hskp){
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
		if(parseData(spi, ace, led_set, buffer, hskp, data)<0){current_state = STATE_ERROR; break;}
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
	setDEMOLEDRgb(90);
	while(1);
	
}
