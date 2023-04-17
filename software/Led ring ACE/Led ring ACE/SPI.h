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

#define BUFFER_SIZE 100

volatile uint8_t buffer[BUFFER_SIZE];
volatile uint8_t buffer_length;
volatile uint8_t read_index;
volatile uint8_t write_index;

void SPIInit(void);
void bufferInit(void);
int writeBuffer(uint8_t val);
int readBuffer(void);
void writeSpi(uint8_t instr, uint8_t data, uint8_t timeout);
void writeSpiBuffer(uint8_t instr, uint8_t* data, uint8_t length, uint8_t timeout);


#endif /* SPI_H_ */