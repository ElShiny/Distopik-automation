/*
 * SPI.c
 *
 * Created: 06/04/2023 15:30:13
 *  Author: Matej
 */ 
#define F_CPU 8000000

#include "SPI.h"
#include "housekeeping.h"
#include "settings.h"
#include <avr/interrupt.h>
#include <util/delay.h>



ISR(PCINT0_vect){
	if(!(PINB & 1<<PINB2) ||/* !(PINB & 1<<PINB7)*/ spi_s.spi_busy)return;
	
	writeBuffer(&buf, SPDR);
	SPDR = 0;
}




void SPIInit(void){
	
	DDRB &= ~(1<<DDB7); //set HIZ mode
	PORTB &= ~(1<<PORTB7);
	
	DDRB |= 1<<DDB4; //set MISO output
	
	DDRB |= 1<< DDB6;
	
	PCICR |= 1<< PCIE0; //interrupt for SS
	PCMSK0 |= 1<<PCINT2;
	
	SPCR = 1<<SPE;// | 1<<SPIE; //enable SPI
}


void bufferInit(buffer_t *buffer){
	
	buffer->read_index = 0;
	buffer->write_index = 0;
	buffer->buffer_length = 0;
	buffer->buffer[0] = 0;
	buffer->buffer[1] = 0;
}

int writeBuffer(buffer_t *buffer, uint8_t val){
	
	if(buffer->buffer_length == BUFFER_SIZE - 1) return -1;
	buffer->buffer[buffer->write_index] = val;
	buffer->write_index++;
	buffer->buffer_length++;
	if(buffer->write_index == BUFFER_SIZE - 1) buffer->write_index = 0;
	return 0;
}

int readBuffer(buffer_t *buffer){
	
	if(buffer->buffer_length == 0) return -1;
	int buf = buffer->buffer[buffer->read_index];
	buffer->read_index++;
	buffer->buffer_length--;
	if(buffer->read_index == BUFFER_SIZE - 1) buffer->read_index = 0;
	return buf;
}

int readBufferLength(buffer_t *buffer){
	return buffer->buffer_length;
}

void writeSpi(spi_t *spi, hskp_t *hskp, uint8_t instr, uint8_t data, uint8_t timeout){
	
	uint32_t start_tick = getTick(hskp);
	while(!(PINB & 1<<PINB2)){}
	disableHSKP(&housekp);
	spi->spi_busy = 1;


	SPSR;
	SPDR = TRANSMIT_KEYWORD;
	
	PORTB &= ~(1<<PORTB7);//set INT low
	DDRB |= 1<<DDB7;
	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2)){if(getTick(hskp)>start_tick+timeout)break;}
	
	SPDR = instr;
	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2)){if(getTick(hskp)>start_tick+timeout)break;}
	
	SPDR = data;
	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2)){if(getTick(hskp)>start_tick+timeout)break;}
	
	PORTB &= ~(1<<PORTB7); //set INT high
	DDRB &= ~(1<<DDB7);
	
	//PCICR |= 1<< PCIE0;
	enableHSKP(&housekp);
	spi->spi_busy = 0;
}

void writeSpiBuffer(spi_t *spi, hskp_t *hskp, uint8_t instr, uint8_t* data, uint8_t length, uint8_t timeout){
	
	uint32_t start_tick = getTick(hskp);
	while(!(PINB & 1<<PINB2)){}
	disableHSKP(&housekp);
	spi->spi_busy = 1;
	
	SPSR;		//clearing spif flag
	SPDR = TRANSMIT_KEYWORD;		//set instr
	
	PORTB &= ~(1<<PORTB7);	//set INT low
	DDRB |= 1<<DDB7;

	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2)){if(getTick(hskp)>start_tick+timeout)break;}
	
	SPDR = instr;
	
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2)){if(getTick(hskp)>start_tick+timeout)break;}
	
	SPDR = length;
	
	for(int i=0; i<length; i++){
		while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2)){if(getTick(hskp)>start_tick+timeout)break;}
		SPDR = data[i];
	}
	while(!(SPSR & 1<<SPIF) || !(PINB & 1<<PINB2)){if(getTick(hskp)>start_tick+timeout) break;}
	PORTB &= ~(1<<PORTB7); //set INT high
	DDRB &= ~(1<<DDB7);
	//_delay_us(10);
	
	enableHSKP(&housekp);
	spi->spi_busy = 0;
	//if(getTick()>start_tick+timeout)errorHandler();
}