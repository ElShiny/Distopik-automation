/*
 * I2C.c
 *
 * Created: 04/04/2023 15:27:50
 *  Author: ElShiny
 */ 

#include "I2C.h"

// 222kHz clock
#define BITRATE 2       // Hardware I2C bitrate// 2 for 400kHz, 32 for 100kHz
#define PRESCALER 0		// SET PRESCALER TO 1


void I2CInit(void){
  TWBR = BITRATE;
  TWSR &= ~(0x2 & PRESCALER);
}

// Start transmission by sending address
uint8_t I2CStart (uint8_t address, uint8_t rw) {

	TWCR |= 1<<TWINT | 1<<TWSTA | 1<<TWEN;		//send start 
	while (!(TWCR & (1<<TWINT)));				//wait for start flag
	if ((TWSR & 0xF8) != 1<<TWS3 && (TWSR & 0xF8) != 1<<TWS4) return 1;	//check start status
	TWDR = address<<1 | rw;						//write adr+rw
	TWCR = 1<<TWINT | 1<<TWEN;					//proceed
	while (!(TWCR & (1<<TWINT)));				//wait for send
	if (((TWSR & 0xF8) != (1<<TWS4|1<<TWS3)) &&
	((TWSR & 0xF8) != 1<<TWS6)) return 1;	//check if sent correctly
	return 0;

}

void I2CStop (void) {
	TWCR = 1<<TWINT|1<<TWSTO|1<<TWEN;	// Send STOP
	//while (!(TWCR & (1<<TWINT)));		// Wait for bus to return to idle state
}

uint8_t I2CWrite (uint8_t data) {
  TWDR = data;
  TWCR = 1<<TWINT | 1 << TWEN;
  while (!(TWCR & 1<<TWINT));
  if((TWSR & 0xF8) != (1<<TWS5|1<<TWS3)) return 1;
  return 0;
  //return (TWSR & 0xF8);
}

uint8_t I2CRead (uint8_t ack) {
	TWCR = 1<<TWINT | 1<<TWEN | ack<<TWEA;
	while (!(TWCR & 1<<TWINT));
	if ((TWSR & 0xF8) != (1<<TWS6|1<<TWS4) &&
	(TWSR & 0xF8) != (1<<TWS6|1<<TWS4|1<<TWS3)) return 1;
	return TWDR;
}