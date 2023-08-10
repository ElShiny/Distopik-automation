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
#include "housekeeping.h"
#include "ACE.h"
#include "SPI.h"

#define IS3_ADR 0x34

typedef struct{
	uint8_t rgb_array[90];
	uint8_t front_color[3];
	uint8_t back_color[3];
	
	uint8_t mode;
	uint8_t start_led;
	uint8_t stop_led;
	uint8_t rot_percent;
	uint8_t led_en;
	}led_drv_t;

extern volatile led_drv_t led_settings;

void LEDInit(led_drv_t *led);

uint8_t LEDSetReg(uint8_t adr, uint8_t val);//writing functions
uint8_t LEDGetReg(uint8_t adr);

uint8_t setLED(uint8_t led, uint8_t *rgb);//led setting
uint8_t setLEDArray(led_drv_t *settings);

void RGBFrom222(uint8_t *arr, uint8_t color);//color manipulation
uint8_t RGBTo222(uint8_t *color);

void setDEMOLEDRgb(uint8_t value);//helper functions
void set_ring(led_drv_t *led, ace_t *ace, uint8_t val);
int bufToRGBArray(led_drv_t *settings, hskp_t *hskp);
int bufToRGB(uint8_t *arr, buffer_t *buf, hskp_t *hskp, int tick);






static const uint8_t led_adr_arr[]={0x01, 0x11, 0x21, 0x31, 0x41, 0x51,
									0x04, 0x14, 0x24, 0x34, 0x44, 0x54,
									0x07, 0x17, 0x27, 0x37, 0x47, 0x57,
									0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A,
									0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D};


#endif /* LED_DRV_H_ */