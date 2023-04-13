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


void SPIInit(void);
void bufferInit(void);
int writeBuffer(uint8_t val);
int readBuffer(void);
void writeSpi(uint8_t data);


#endif /* SPI_H_ */