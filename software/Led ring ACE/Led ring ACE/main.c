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
//#include "SoftwareSerial.h"
#include "led_drv.h"
#include "SPI.h"





int main(void)
{
	//softSerialBegin(9600);
	ACEInit();
	I2CInit();
	LEDInit();
	SPIInit();
	bufferInit();
	
	sei();
	
	while(1){
	
		debug_printf("ACE: %d\r\n", ace_val);

	_delay_ms(1000);
	}
	
}

