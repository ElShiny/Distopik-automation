/*
 * led.c
 *
 *  Created on: Apr 19, 2024
 *      Author: Matej
 */

#include "stdint.h"
#include "string.h"

#include "led.h"
#include "gpio.h"
#include "i2c.h"
#include "port.h"
#include "main.h"
#include "port.h"
#include <math.h>
#include "settings.h"

//volatile led_driver_t led_settings;
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];
extern USHORT 	usRegInputBuf[REG_INPUT_NREGS];



void led_init(void)
{
	  //reset registers
	  uint8_t arr[2] = {0xCF, 0xAE};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr, 2, 100) == HAL_BUSY){}

	  //configuration register
	  uint8_t arr1[2] = {0xA0, 0x01};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr1, 2, 100) == HAL_BUSY){}

	  //global current control register
	  uint8_t arr2[2] = {0xA1, 30};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr2, 2, 100) == HAL_BUSY){}

	  //Scaling registers
	  for(int a = 0x90; a < 0x9F; a++){
	  	uint8_t arr3[2] = {a, 60};
	  	while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr3, 2, 100) == HAL_BUSY){}
	  }

	  //PWM registers
	  for(int i = 0 ; i<30; i++){
		uint8_t arr5[4] = {led_adr_arr[i], 0,0,0};
		while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr5, 4, 100) == HAL_BUSY){}
	  }

}

void led_set(uint8_t led, uint8_t *rgb) {

	  uint8_t arr[4] = {led_adr_arr[led], rgb[2], rgb[1], rgb[0]};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr, 4, 100) == HAL_BUSY){}
}


void led_set_custom_size_overlay(volatile led_driver_t *settings, uint8_t *rgb_arr, uint8_t start_led, uint8_t led_count){

	if(start_led + led_count > 30)Error_Handler();

	uint8_t output_array[101];
	memset(output_array, 0, 101);
	memcpy(settings->rgb_array+start_led*3, rgb_arr, led_count*3);

	output_array[0] = led_adr_arr[0];

	for(int i = 0; i<30; i++){
		output_array[led_adr_arr[i]+2] = settings->rgb_array[i*3];
		output_array[led_adr_arr[i]+1] = settings->rgb_array[i*3+1];
		output_array[led_adr_arr[i]] = settings->rgb_array[i*3+2];

	}

	while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, output_array, 101, 100) == HAL_BUSY){}
}

void led_demo_value(uint8_t value){

	int led = value-1;//((value)*23)>>6;
	int bck_clr = 0;
	uint8_t clr[3];
	uint8_t led_clr[90];


	for(int i = 0; i<30; i++){
		if(i<=led && led<= 30){RGBFrom222(clr, 0b110000); memcpy( led_clr+i*3, clr, 3); bck_clr = 0;}
		else if(i<=led-30 && 30<=led && led<=60){RGBFrom222(clr, 0b001100); memcpy( led_clr+i*3, clr, 3);bck_clr = 0b110000;}
		else if(i<=led-60 && 60<=led && led<=90){RGBFrom222(clr, 0b000011); memcpy( led_clr+i*3, clr, 3);bck_clr = 0b001100;}
		else {RGBFrom222(clr, bck_clr); memcpy( led_clr+i*3, clr, 3);}
	}

	led_set_custom_size_overlay(&hled1, led_clr, 0, 30);
}

void led_demo_buffering(uint8_t value){
	uint8_t rgb[3] = {0,0,0};
	int i = value;

	//for(int i = 0; i<90; i++){
		if(i<30){RGBFrom222(rgb, 0b110000);}
		if(30<=i && i<60){RGBFrom222(rgb, 0b001100);}
		if(60<=i && i<90){RGBFrom222(rgb, 0b000011);}
		led_set(i%30, rgb);
	//}
}

void RGBFrom222(uint8_t *arr, uint8_t color){

	arr[0] = ((color>>4)&0x3)<<6;
	arr[1] = ((color>>2)&0x3)<<6;
	arr[2] = ((color)&0x3)<<6;
	return;
}

void led_set_all_zero(volatile led_driver_t *settings){

	uint8_t output_array[101];
	memset(output_array, 0, 101);
	memset(settings->rgb_array, 0, 90);

	output_array[0] = led_adr_arr[0];

	while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, output_array, 101, 100) == HAL_BUSY){}
}

void led_return_values(volatile led_driver_t *led, USHORT *input_buffer){
	//holding_buffer[LED_ARRAY] = led->rgb_array;
	//memcpy(&input_buffer[LED_ARRAY], led->rgb_array, 90);
	for(int i = 0; i<90; i++){
		input_buffer[LED_RETURN_ARRAY + i] = led->rgb_array[i];
	}



	//holding_buffer[ACE_INCOMING_VALUE_CHANGE] = ace->incoming_val_change;


}

void led_settings_parser(volatile led_driver_t *led){

	if(led->disable == 1){
		led_set_all_zero(led);
		return;
	}

	switch (led->mode) {
		case 0:
			led_mode_0(led);
			break;
		case 1:
			led_mode_1(led);
			break;
		case 2:
			led_mode_2(led);
			break;
		case 3:
			led_mode_3(led);
			break;


		default:
			break;
	}

	return;
}

void led_mode_0(volatile led_driver_t *led){

	//led_set_custom_size_overlayled();
	int start_led = roundf(led->start_angle/(float)LED_STEP_ANGLE);
	int stop_led = roundf(led->stop_angle/(float)LED_STEP_ANGLE);
	int angle = led->rot_angle;
	//int tmp_angle = 0;

	if(start_led>=30||stop_led>30)Error_Handler();



	uint8_t led_clr[90];
	memset(led_clr, 0, 90);


	if(start_led>stop_led){
		stop_led+=30;
		if(led->rot_angle < led->stop_angle)angle = led->rot_angle + 360;
	}

		for(int i = start_led; i<stop_led; i++){

//			if(i>=30)angle = 360 + led->rot_angle;
//			else angle = led->rot_angle;

			if(angle > (i*LED_STEP_ANGLE)){
				memcpy( led_clr+i%30*3, led->front_color, 3);
			}

			if(angle <= (i*LED_STEP_ANGLE)){
				memcpy( led_clr+i%30*3, led->back_color, 3);
			}
		}
		led_set_custom_size_overlay(led, led_clr, 0, 30);
}

void led_mode_1(volatile led_driver_t *led){

	//led_set_custom_size_overlayled();
	int angle = led->rot_angle;
	//int tmp_angle = 0;

	//if(start_led>=30||stop_led>30)Error_Handler();

	uint8_t led_arr[90];
	memset(led_arr, 0, 90);


			if(angle >= 0 && angle < 90){
				for(int i = 1; i<8; i++){
					memcpy( led_arr+i*3, led->front_color, 3);
				}
			}
			else if(angle >= 90 && angle < 180){
				for(int i = 8; i<15; i++){
					memcpy( led_arr+i*3, led->front_color, 3);
				}
			}
			else if(angle >= 180 && angle < 270){
				for(int i = 16; i<23; i++){
					memcpy( led_arr+i*3, led->front_color, 3);
				}
			}
			else if(angle >= 270 && angle <= 360){
				for(int i = 23; i<30; i++){
					memcpy( led_arr+i*3, led->front_color, 3);
				}
			}
			else return;
		led_set_custom_size_overlay(led, led_arr, 0, 30);
}

void led_mode_2(volatile led_driver_t *led){

	//led_set_custom_size_overlayled();
	int angle = led->rot_angle;
	float angle_per_step = 360/(float)led->steps;
	float leds_per_step = 30/(float)led->steps;

	int offset = (int)(angle/angle_per_step)*leds_per_step;
	if((int)offset+leds_per_step>30)return;
	//int tmp_angle = 0;

	//if(start_led>=30||stop_led>30)Error_Handler();
	usRegInputBuf[LED_OFFSET] = offset;

	uint8_t led_arr[90];
	memset(led_arr, 0, 90);

	for(int i = 0; i<(int)leds_per_step; i++){

		memcpy( led_arr+(i+offset)*3, led->front_color, 3);
	}

	led_set_custom_size_overlay(led, led_arr, 0, 30);
}

void led_mode_3(volatile led_driver_t *led){

	led_set_custom_size_overlay(led, led->rgb_array, 0, 30);
}

