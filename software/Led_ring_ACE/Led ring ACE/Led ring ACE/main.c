/*
 * Led ring ACE.c
 *
 * Created: 31/03/2023 13:46:39
 * Author : Matej
 */ 

#ifndef F_CPU
#define  F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "ACE.h"


int main(void)
{
	/* Replace with your application code */
	DDRB |= (1<<PINB6);
	DDRD |= (1<<ACE_EN);
	PORTD |= (1<<ACE_EN);
	while (1)
	{
		PORTB ^= (1<<PINB6);
		PORTD ^= (1<<ACE_EN);
		_delay_ms(2000);

	}
	
}

