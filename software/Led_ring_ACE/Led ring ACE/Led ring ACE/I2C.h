/*
 * I2C.h
 *
 * Created: 04/04/2023 15:27:31
 *  Author: ElShiny
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>



void I2CInit(void)
uint8_t read(void);
uint8_t readLast(void);
bool write(uint8_t data);
bool start(uint8_t address, int32_t readcount);
bool restart(uint8_t address, int32_t readcount);
void stop(void);


int32_t I2Ccount;
uint8_t transfer(uint8_t data);


extern TinyI2CMaster TinyI2C;



#endif /* I2C_H_ */