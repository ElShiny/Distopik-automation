/*
 * I2C.h
 *
 * Created: 04/04/2023 15:27:31
 *  Author: ElShiny
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <avr/io.h>

#define I2C_WRITE 0
#define I2C_READ 1



void I2CInit(void);
uint8_t I2CWrite (uint8_t data);
uint8_t I2CRead (uint8_t ack);
uint8_t I2CStart(uint8_t address, uint8_t rw); 
void I2CStop(void);
void MCP_init(void);
uint8_t mirror_byte(uint8_t byte);
void set_ladder(uint8_t val, uint8_t laddder);


#endif /* I2C_H_ */