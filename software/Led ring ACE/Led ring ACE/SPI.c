/*
 * SPI.c
 *
 * Created: 06/04/2023 15:30:13
 *  Author: Matej
 */ 

#include "SPI.h"
#include <avr/interrupt.h>


uint8_t buffer[BUFFER_SIZE];
uint8_t buffer_length;
uint8_t read_index;
uint8_t write_index;



// ISR(PCINT0_vect){
// 
// 	if(!(PINB & 1<<PINB2)) return;
// 	cli();
// 	
// 	writeBuffer(SPDR);
// 	PORTB ^= 1<<PORTB6;
// 	
// 	sei();
// }


void SPIInit(void){
			
	DDRB &= ~(1<<PINB7); //set HIZ mode
	PORTB &= ~(1<<PINB7);
	
	DDRB |= 1<<DDB4; //set MISO output
	
	DDRB |= 1<< DDB6;
	
	PCICR |= 1<< PCIE0; //interrupt for SS
	PCMSK0 |= 1<<PCINT2;
	
	SPCR = 1<<SPE; //enable SPI
}


void bufferInit(void){
	
	read_index = 0;
	write_index = 0;
	buffer_length = 0;
}

int writeBuffer(uint8_t val){
	
	if(buffer_length == BUFFER_SIZE) return -1;
	buffer[write_index++] = val;
	buffer_length++;
	if(write_index == BUFFER_SIZE) write_index = 0;
	return 0;
}

int readBuffer(void){
	
	if(buffer_length == 0) return -1;
	int buf = buffer[read_index];
	read_index++;
	buffer_length--;
	if(read_index == BUFFER_SIZE) read_index = 0;
	return buf;
}