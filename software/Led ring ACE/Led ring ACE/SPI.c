/*
 * SPI.c
 *
 * Created: 06/04/2023 15:30:13
 *  Author: Matej
 */ 
#define F_CPU 8000000

#include "SPI.h"
#include <avr/interrupt.h>
#include <util/delay.h>




uint8_t buffer[BUFFER_SIZE];
uint8_t buffer_length;
uint8_t read_index;
uint8_t write_index;
uint8_t spi_busy;



ISR(PCINT0_vect){

	if(!(PINB & 1<<PINB2) || !(PINB & 1<<PINB7 ) || spi_busy)return;
	cli();
	writeBuffer(SPDR);
	SPDR = 0;
	sei();
}


void SPIInit(void){
			
	DDRB &= ~(1<<DDB7); //set HIZ mode
	PORTB &= ~(1<<PORTB7);
	
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
	buffer[0] = 0;
	buffer[1] = 0;
}

int writeBuffer(uint8_t val){
	
	if(buffer_length == BUFFER_SIZE) return -1;
	buffer[write_index] = val;
	write_index++;
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

void writeSpi(uint8_t instr, uint8_t data){
	_delay_us(50);
	while(spi_busy);
	

		
	spi_busy = 1;
	SPSR;
	SPDR = instr;
	
	PORTB &= ~(1<<PORTB7);//set INT low
	DDRB |= 1<<DDB7;
	
	cli();
	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2));
	
	SPDR = data;
	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2));
	
	PORTB &= ~(1<<PORTB7); //set INT high
	DDRB &= ~(1<<DDB7);
	//_delay_us(10);
	
	sei();
	spi_busy = 0;	
}

void writeSpiBuffer(uint8_t instr, uint8_t* data, uint8_t length){
	
	while(spi_busy || !(PINB & 1<<PINB7));
	spi_busy = 1; //set global busy flag
	
	cli();
	
	SPSR;		//clearing spif flag
	SPDR = instr;		//set instr
	
	PORTB &= ~(1<<PORTB7);	//set INT low
	DDRB |= 1<<DDB7;

	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2));
	SPDR = length;
	
	for(int i=0; i<length; i++){
		while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2));
		SPDR = data[i];
	}
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2));
	PORTB &= ~(1<<PORTB7); //set INT high
	DDRB &= ~(1<<DDB7);
	//_delay_us(10);
	
	sei();
	spi_busy = 0;
}