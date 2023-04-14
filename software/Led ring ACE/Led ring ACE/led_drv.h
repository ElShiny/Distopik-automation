/*
 * led_drv.h
 *
 * Created: 05/04/2023 01:10:26
 *  Author: ElShiny
 */ 


#ifndef LED_DRV_H_
#define LED_DRV_H_


#include <stdint.h>
#include <avr/io.h>

#define IS3_ADR 0x34


void LEDInit(void);
uint8_t LEDSetReg(uint8_t adr, uint8_t val);
uint8_t LEDGetReg(uint8_t adr);
uint8_t setLED(uint8_t led, uint8_t rgb);
void setLEDRgb(uint8_t value);


static uint8_t rgb_array[90] = {};

static const uint8_t led_adr_arr[]={0x01, 0x11, 0x21, 0x31, 0x41, 0x51,
									0x04, 0x14, 0x24, 0x34, 0x44, 0x54,
									0x07, 0x17, 0x27, 0x37, 0x47, 0x57,
									0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A,
									0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D};


#endif /* LED_DRV_H_ */