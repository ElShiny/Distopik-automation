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

#define LED_ADR 0x34


void LEDInit(void);
uint8_t LEDSetReg(uint8_t adr, uint8_t val);
uint8_t LEDGetReg(uint8_t adr);


#endif /* LED_DRV_H_ */