
/*
 * led.h
 *
 *  Created on: Apr 19, 2024
 *      Author: Matej
 */

#ifndef ADDONS_INC_LED_H_
#define ADDONS_INC_LED_H_

#include <stdint.h>
#include "port.h"


#define LED_STEP_ANGLE 12

typedef struct{
	uint8_t rgb_array[90];
	uint8_t front_color[3];
	uint8_t back_color[3];

	uint8_t disable;
	uint8_t mode;
	uint16_t start_angle;
	uint16_t stop_angle;
	uint16_t rot_angle;
	uint8_t steps;


	uint8_t settings_changed;
	}led_driver_t;

extern volatile led_driver_t hled1;

void led_init(void);
void led_set(uint8_t led, uint8_t *rgb);
void led_set_custom_size_overlay(volatile led_driver_t *settings, uint8_t *rgb_arr, uint8_t start_led, uint8_t  );
void RGBFrom222(uint8_t *arr, uint8_t color);//color manipulation
void led_demo_value(uint8_t value);//helper functions
void led_demo_buffering(uint8_t value);
void led_set_all_zero(volatile led_driver_t *settings);
void led_return_values(volatile led_driver_t *led, USHORT *input_buffer);

void led_settings_parser(volatile led_driver_t *led);
void led_mode_0(volatile led_driver_t *led);
void led_mode_1(volatile led_driver_t *led);
void led_mode_2(volatile led_driver_t *led);
void led_mode_3(volatile led_driver_t *led);



static const uint8_t led_adr_arr[]={0x01, 0x11, 0x21, 0x31, 0x41, 0x51,
									0x04, 0x14, 0x24, 0x34, 0x44, 0x54,
									0x07, 0x17, 0x27, 0x37, 0x47, 0x57,
									0x0A, 0x1A, 0x2A, 0x3A, 0x4A, 0x5A,
									0x0D, 0x1D, 0x2D, 0x3D, 0x4D, 0x5D};




#endif /* ADDONS_INC_LED_H_ */
