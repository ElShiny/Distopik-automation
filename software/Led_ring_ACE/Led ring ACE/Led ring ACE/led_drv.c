/*
 * led_drv.c
 *
 * Created: 05/04/2023 01:15:40
 *  Author: Matej
 */ 

#include "led_drv.h"
#include "I2C.h"


void LEDInit(void){
	
	LEDSetReg(0xA0, 0x01);
	LEDSetReg(0xA1, 30);

	for(int a = 0x90; a < 0x9E; a++){
		LEDSetReg(a, 100);
	}
	
	
	
	LEDSetReg(0x01, 150);
	LEDSetReg(0x12, 150);
	LEDSetReg(0x23, 150);	
	
	DDRC |= 1<<PINC3;
	PORTC |= 1<<PINC3;
	
}

uint8_t LEDSetReg(uint8_t adr, uint8_t val){
	
	if(I2CStart(LED_ADR, I2C_WRITE)) return 1;
	if(I2CWrite(adr)) return 2;
	if(I2CWrite(val)) return 3;
	I2CStop();
	return 0;
}


uint8_t LEDGetReg(uint8_t adr){

	if(I2CStart(LED_ADR, I2C_WRITE)) return 1;
	if(I2CWrite(adr)) return 1;
	I2CStop();
	if(I2CStart(LED_ADR, I2C_READ)) return 1;
	uint8_t ret = I2CRead(0);
	I2CStop();
	return ret;
}