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
	if(!(PINB & 1<<PINB2))return;//if cs is low then ignore
	spi_s.one_transfer = 1;
	
	if(SPDR && spi_s.send_block == 0 && spi_s.spi_send_mode){ spi_s.is_master_dumb = 1; return;}
	else {spi_s.is_master_dumb = 0;}
	if(spi_s.spi_send_mode)return;
	
	if(SPDR == BLOCK_SPI){spi_s.send_block=1;}//block sending
	else if(SPDR == UNBLOCK_SPI){spi_s.send_block=0; return;}//unblock sending


	
	writeBuffer(&buf, SPDR);
	SPDR = 0;
	//spi_s.one_transfer = 0;
	//DDRB|=1<<DDB6;
	//PORTB|=1<<PINB6;
} 




void SPIInit(void){
			
	DDRB &= ~(1<<DDB7); //set HIZ mode
	PORTB &= ~(1<<PORTB7);
	
	DDRB |= 1<<DDB4; //set MISO output
	DDRB|=1<<DDB6;
	
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

void writeSpi(spi_t *spi, hskp_t *hskp, uint8_t instr, uint16_t data, uint16_t timeout){
	
	uint32_t start_tick = getTick(hskp);//start counting ms
	while(!(PINB & 1<<PINB2)|| spi->send_block){}			//wait for CS high and send blocking
	spi->spi_send_mode = 1;					//set transmission flag
	disableHSKP(&housekp);					//disable led updating
	spi->one_transfer = 0;					//clear SPI ISR flag

	while(1){

		SPDR = TRANSMIT_KEYWORD;			//set transmit keyword
		
		DDRB |= 1<<DDB7;
		PORTB &= ~(1<<PORTB7);				//set INT low


		while(!spi->one_transfer){if(getTick(hskp)>start_tick+timeout){break;}}
											//wait for transmission or timeout
		spi->one_transfer = 0;
		if(!spi->is_master_dumb){ break;}	//if master is not reading then retry
	}
	
	SPDR = instr;
	
	while(!spi->one_transfer){if(getTick(hskp)>start_tick+timeout){break;}}
		spi->one_transfer = 0;

	SPDR = (uint8_t)data;
	
	while(!spi->one_transfer){if(getTick(hskp)>start_tick+timeout){break;}}
		spi->one_transfer = 0;

	SPDR = (uint8_t)(data>>8);
		
	while(!spi->one_transfer){if(getTick(hskp)>start_tick+timeout){break;}}
		spi->one_transfer = 0;
	
	PORTB &= ~(1<<PORTB7); //set INT high
	DDRB &= ~(1<<DDB7);
	
	//PCICR |= 1<< PCIE0;
	enableHSKP(&housekp);
	spi->spi_send_mode = 0;	
}

void writeSpiBuffer(spi_t *spi, hskp_t *hskp, uint8_t instr, uint8_t* data, uint8_t length, uint16_t timeout){
	
	uint32_t start_tick = getTick(hskp);
	while(!(PINB & 1<<PINB2)){}
	disableHSKP(&housekp);
	spi->spi_send_mode = 1;
	
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
	spi->spi_send_mode = 0;
	//if(getTick()>start_tick+timeout)errorHandler();
}