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
#include "SoftwareSerial.h"


int main(void)
{
	softSerialBegin(9600);
	initACE();
	while (1) {
			debug_printf("raw: %d\r\n",readACEQuick());
// 			_delay_ms(100);
		}
	
}

