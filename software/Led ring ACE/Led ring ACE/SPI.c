/*
 * SPI.c
 *
 * Created: 06/04/2023 15:30:13
 *  Author: Matej
 */ 

#include "SPI.h"


void SPISlaveInit(void){
	
	DDRB &= ~(1<<DDB7);
	PORTB &= ~(1<<PORTB7); //INT pin to HIZ	
	
	PCICR |= 1<<PCIE0;
	PCMSK0 |= 1<<PCINT2;
	
	DDRB |= 1<<DDB4;
	SPCR = 1<<SPE;
	
	
}

ISR(PCINT0_vect){
	
}