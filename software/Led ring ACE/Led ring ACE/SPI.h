/*
 * SPI.h
 *
 * Created: 06/04/2023 14:33:14
 *  Author: ElShiny
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <avr/io.h>
#include "housekeeping.h"

#define BUFFER_SIZE 120

#define RECIEVE_KEYWORD 0xCF
#define TRANSMIT_KEYWORD 0xDF

enum spi_states_e{SPI_READ, SPI_WRITE, SPI_ERR};
	
	
typedef struct {
	volatile uint8_t buffer[BUFFER_SIZE];
	volatile uint8_t buffer_length;
	volatile uint8_t read_index;
	volatile uint8_t write_index;
} buffer_t;

buffer_t buf;

typedef struct {
	volatile uint8_t spi_send_mode;
	volatile uint8_t send_block;
	volatile uint8_t is_master_dumb;
	volatile uint8_t one_transfer;
	}spi_t;
	
spi_t spi_s;



void SPIInit(void);
void bufferInit(buffer_t *buffer);
int writeBuffer(buffer_t *buffer, uint8_t val);
int readBuffer(buffer_t *buffer);
int readBufferLength(buffer_t *buffer);
void writeSpi(spi_t *spi, hskp_t *hskp, uint8_t instr, uint16_t data, uint8_t timeout);
void writeSpiBuffer(spi_t *spi, hskp_t *hskp, uint8_t instr, uint8_t* data, uint8_t length, uint8_t timeout);



	

#endif /* SPI_H_ */