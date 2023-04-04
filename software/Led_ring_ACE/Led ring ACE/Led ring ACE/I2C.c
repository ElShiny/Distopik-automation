/*
 * I2C.c
 *
 * Created: 04/04/2023 15:27:50
 *  Author: ElShiny
 */ 

#include "I2C.h"
#include <avr/io.h>

// 222kHz clock
#define BITRATE 10;                             // Hardware I2C bitrate
#define PRESCALER 0;							// SET PRESCALER TO 1


void I2CInit(void) {
  TWBR = BITRATE;
  TWSR &= ~(0x2 & PRESCALER);
}

// Start transmission by sending address
uint8_t I2CStart (uint8_t address, uint8_t rw) {

	TWCR |= 1<<TWINT | 1<<TWSTA | 1<<TWEN;	//send start 
	while (!(TWCR & (1<<TWINT)));			//wait for start flag
	if ((TWSR & 0xF8) != TWS3 && (TWSR & 0xF8) != TWS4) return 1;	//check start status
	TWDR = address<<1 | rw;
	TWCR = 1<<TWINT | 1<<TWEN;
	while (!(TWCR & (1<<TWINT)));
	
	
	
	

}

bool TinyI2CMaster::restart(uint8_t address, int32_t readcount) {
	return TinyI2CMaster::start(address, readcount);
}

void TinyI2CMaster::stop (void) {
	TWI0.MCTRLB |= TWI_MCMD_STOP_gc;                                // Send STOP
	while (!(TWI0.MSTATUS & TWI_BUSSTATE_IDLE_gc));                 // Wait for bus to return to idle state
}

uint8_t I2CRead(void) {
  if (I2Ccount != 0) I2Ccount--;
  while (!(TWI0.MSTATUS & TWI_RIF_bm));                           // Wait for read interrupt flag
  uint8_t data = TWI0.MDATA;
  // Check slave sent ACK?
  if (I2Ccount != 0) TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;         // ACK = more bytes to read
  else TWI0.MCTRLB = TWI_ACKACT_NACK_gc;                          // Send NAK
  return data;
}

uint8_t TinyI2CMaster::readLast (void) {
  I2Ccount = 0;
  return TinyI2CMaster::read();
}

uint8_t TinyI2CMaster::write (uint8_t data) {
  TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;                            // Prime transaction
  TWI0.MDATA = data;                                              // Send data
  while (!(TWI0.MSTATUS & TWI_WIF_bm));                           // Wait for write to complete
  if (TWI0.MSTATUS & (TWI_ARBLOST_bm|TWI_BUSERR_bm))return false; // Fails if bus error or arblost
  return !(TWI0.MSTATUS & TWI_RXACK_bm);                          // Returns true if slave gave an ACK
}


