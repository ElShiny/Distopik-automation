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
#include "ACE.h"
#include "I2C.h"
#include "SoftwareSerial.h"
#include "led_drv.h"


int main(void)
{
	softSerialBegin(9600);
	ACEInit();
	I2CInit();
	LEDInit();
	
	while(1){
	
	//debug_printf("ACE: %d\r\n", readACEQuick());
	int led = ((uint16_t)readACEQuick()*15)>>6;
	//debug_printf("LED: %d\r\n", led);
		for(int i = 1; i<(led+1); i++){
			if( i <led){
				LEDSetReg(i, 100);
				LEDSetReg(i+1, 100);
				LEDSetReg(i+2, 100);}
				else{
									LEDSetReg(i, 0);
									LEDSetReg(i+1, 0);
									LEDSetReg(i+2, 0);
				}

		}
	//_delay_ms(100);
	}
	
}

