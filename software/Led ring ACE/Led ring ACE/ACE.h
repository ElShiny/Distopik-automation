/*
 * ACE.h
 *
 * Created: 31/03/2023 18:38:24
 *  Author: ElShiny
 */ 


#ifndef ACE_H_
#define ACE_H_

#include <stdint.h>
//#define ACE_PIN_INV


/* ACE PIN ASSIGMENTS 1 */
#define ACE_P1 1<<PINB1 //D0
#define ACE_P2 1<<PINB0 //D1
#define ACE_P3 1<<PIND7 //D2
#define ACE_P4 1<<PIND6 //D3
#define ACE_P5 1<<PINC2 //D4
#define ACE_P6 1<<PINC1 //D5
#define ACE_P7 1<<PINC0 //D6
#define ACE_P8 1<<PINC7 //D7

#define ACE_P1_PORT PINB
#define ACE_P2_PORT PINB
#define ACE_P3_PORT PIND
#define ACE_P4_PORT PIND
#define ACE_P5_PORT PINC
#define ACE_P6_PORT PINC
#define ACE_P7_PORT PINC
#define ACE_P8_PORT PINC

#define ACE_EN PIND3


typedef struct{
	int volatile ace_val;
	uint8_t ace_val_new;
	uint8_t ace_val_old;
	uint8_t volatile ace_changed;
	uint8_t volatile ace_led_changed;
}ace_t;

ace_t ace_rot;


void ACEInit(ace_t *ace);
uint8_t readACEValRaw(void);
uint8_t ACETransform(uint8_t val);
uint8_t isBitSet(uint8_t reg, uint8_t pin);
uint8_t readACEQuick(void);
void absoluteToRelative(ace_t *ace);

 static __flash const uint8_t encoderMap[256] = {
	0xFF,0x38,0x28,0x37,0x18,0xFF,0x27,0x34,0x08,0x39,0xFF,0xFF,0x17,0xFF,0x24,0x0D,
	0x78,0xFF,0x29,0x36,0xFF,0xFF,0xFF,0x35,0x07,0xFF,0xFF,0xFF,0x14,0x13,0x7D,0x12,
	0x68,0x69,0xFF,0xFF,0x19,0x6A,0x26,0xFF,0xFF,0x3A,0xFF,0xFF,0xFF,0xFF,0x25,0x0E,
	0x77,0x76,0xFF,0xFF,0xFF,0x6B,0xFF,0xFF,0x04,0xFF,0x03,0xFF,0x6D,0x6C,0x02,0x01,
	0x58,0xFF,0x59,0xFF,0xFF,0xFF,0xFF,0x33,0x09,0x0A,0x5A,0xFF,0x16,0x0B,0xFF,0x0C,
	0xFF,0xFF,0x2A,0x2B,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x15,0xFF,0x7E,0x7F,
	0x67,0xFF,0x66,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x5B,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x74,0x75,0xFF,0xFF,0x73,0xFF,0xFF,0xFF,0x5D,0x5E,0x5C,0xFF,0x72,0x5F,0x71,0x00,
	0x48,0x47,0xFF,0x44,0x49,0xFF,0xFF,0x1D,0xFF,0x46,0xFF,0x45,0xFF,0xFF,0x23,0x22,
	0x79,0xFF,0x7A,0xFF,0x4A,0xFF,0xFF,0x1E,0x06,0xFF,0x7B,0xFF,0xFF,0xFF,0x7C,0x11,
	0xFF,0xFF,0xFF,0x43,0x1A,0xFF,0x1B,0x1C,0xFF,0x3B,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x05,0xFF,0xFF,0xFF,0x6E,0xFF,0x6F,0x10,
	0x57,0x54,0xFF,0x2D,0x56,0x55,0xFF,0x32,0xFF,0xFF,0xFF,0x2E,0xFF,0xFF,0xFF,0x21,
	0xFF,0x53,0xFF,0x2C,0x4B,0xFF,0xFF,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x20,
	0x64,0x3D,0x65,0x42,0xFF,0x3E,0xFF,0x31,0x63,0x3C,0xFF,0x2F,0xFF,0xFF,0xFF,0x30,
	0x4D,0x52,0x4E,0x41,0x4C,0x3F,0xFF,0x40,0x62,0x51,0x4F,0x50,0x61,0x60,0x70,0xFF };

#endif /* ACE_H_ */