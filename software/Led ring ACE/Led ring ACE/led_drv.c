/*
 * led_drv.c
 *
 * Created: 05/04/2023 01:15:40
 *  Author: Matej
 */ 

#include "led_drv.h"
#include "ACE.h"
#include "SPI.h"
#include "I2C.h"
#include "housekeeping.h"
#include "settings.h"

volatile led_drv_t led_settings;

void LEDInit(void){
	
// 	const int rgb = 0b001100;
	
	LEDSetReg(0xCF, 0xAE); //reset 
	
	LEDSetReg(0xA0, 0x01);
	LEDSetReg(0xA1, 30);

	for(int a = 0x90; a < 0x9F; a++){
		LEDSetReg(a, 60);
	}
	led_settings.ace_en= 1;
	
	DDRC |= 1<<PINC3;
	PORTC |= 1<<PINC3;
		
}

uint8_t LEDSetReg(uint8_t adr, uint8_t val){
	
	if(I2CStart(IS3_ADR, I2C_WRITE)) return 1;
	if(I2CWrite(adr)) return 1;
	if(I2CWrite(val)) return 1;
	I2CStop();
	return 0;
}


uint8_t LEDGetReg(uint8_t adr){

	if(I2CStart(IS3_ADR, I2C_WRITE)) return 1;
	if(I2CWrite(adr)) return 1;
	I2CStop();
	if(I2CStart(IS3_ADR, I2C_READ)) return 1;
	uint8_t ret = I2CRead(0);
	I2CStop();
	return ret;
}

uint8_t setLED(uint8_t led, uint8_t *rgb){
	uint8_t led_adr = led_adr_arr[led];
	if(I2CStart(IS3_ADR, I2C_WRITE)) return 1;
	if(I2CWrite(led_adr)) return 1;
	
	I2CWrite(rgb[0]);
	I2CWrite(rgb[1]);
	I2CWrite(rgb[2]);
	
	I2CStop();
	return 0;
}

uint8_t setLEDArray(led_drv_t *settings){
	
	for(int j = 0; j<6; j++){
		uint8_t led_adr = led_adr_arr[j];
		if(I2CStart(IS3_ADR, I2C_WRITE)) return 1;
		if(I2CWrite(led_adr)) return 1;
		
		for(int i = 0; i<30; i+=6){
			I2CWrite(settings->rgb_array[i+j]);
			I2CWrite(settings->rgb_array[i+1+j]);
			I2CWrite(settings->rgb_array[i+2+j]);
		}
	}
	return 0;
}

void setColors(led_drv_t *settings){
	
	int start_tick = getTick();
	while(readBufferLength() < 3){if(getTick()>(start_tick+MAX_TIMEOUT))break;}
	for(int i = 0; i < 3; i++){settings->front_color[i] = readBuffer();}
	while(readBufferLength() < 3){if(getTick()>(start_tick+MAX_TIMEOUT))break;}
	for(int i = 0; i < 3; i++){settings->back_color[i] = readBuffer();}
}

uint8_t RGBTo222(uint8_t *color){
	return((color[0]&0xc0)>>2)||((color[1]&0xc0)>>4)||((color[2]&0xc0)>>6);
}

void RGBFrom222(uint8_t *arr, uint8_t color){

	arr[0] = ((color>>4)&0x3)<<6;
	arr[1] = ((color>>2)&0x3)<<6;
	arr[2] = ((color)&0x3)<<6;
}


void setDEMOLEDRgb(uint8_t value){
	int led = value-1;//((value)*23)>>6;
	int bck_clr = 0;
	uint8_t clr[3];
	
	for(int i = 0; i<30; i++){
		if(i<=led && led<= 30){RGBFrom222(clr, 0b110000); setLED(i, clr); bck_clr = 0;}
		else if(i<=led-30 && 30<=led && led<=60){RGBFrom222(clr, 0b001100); setLED(i, clr);bck_clr = 0b110000;}
		else if(i<=led-60 && 60<=led && led<=90){RGBFrom222(clr, 0b000011); setLED(i, clr);bck_clr = 0b001100;}
		else {RGBFrom222(clr, bck_clr); setLED(i, clr);}
	}
}

int bufToRGBArray(led_drv_t *settings){
	int start_tick = getTick();
		
	while(readBufferLength() < 2){if(getTick()>(start_tick+MAX_TIMEOUT)){break;}}
	
	int length = readBuffer();
	int start = readBuffer();

	while(readBufferLength() < length){if(getTick()>(start_tick+MAX_TIMEOUT)){break;}}
	
	for(int i = 0; i < length; i++){
		settings->rgb_array[i+start] = readBuffer();	
	}
	return 0;
}