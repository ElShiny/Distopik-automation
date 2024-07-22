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

//volatile led_driver_t led_settings;
extern USHORT   usRegHoldingStart;
extern USHORT   usRegHoldingBuf[200];



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

	uint8_t output_array[101];
	memset(output_array, 0, 101);

	output_array[0] = led_adr_arr[0];
//	memcpy(usRegHoldingBuf, output_array, 101);
	memcpy(settings->rgb_array+start_led*3, rgb_arr, led_count*3);

//	for(int i = 1; i<101; i++){
	//	output_array[i] = i;
	//}

	for(int i = 0; i<30; i++){
	//int i = 0;
		output_array[led_adr_arr[i]+2] = settings->rgb_array[i*3];
		output_array[led_adr_arr[i]+1] = settings->rgb_array[i*3+1];
		output_array[led_adr_arr[i]] = settings->rgb_array[i*3+2];

	}

    //while (HAL_I2C_GetState(&hi2c1) != HAL_I2C_STATE_READY)
    //{
    //}// Wait for the end of the transfer
	while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, output_array, 101, 100) == HAL_BUSY){}
	//if(HAL_I2C_Master_Transmit_DMA(&hi2c1, IS3_ADR, output_array, 101)!= HAL_OK) Error_Handler();
}

/*
void led_set_custom_size_overlay(volatile led_driver_t *settings, uint8_t *rgb_arr, uint8_t start_led, uint8_t led_count){

	uint8_t output_array[101];
	memset(output_array, 0, 101);

	output_array[0] = led_adr_arr[0];

	memcpy(settings->rgb_array+start_led*3, rgb_arr, led_count*3);

	for(int i = 0; i<30; i++){
	//int i = 0;
		output_array[led_adr_arr[i]+2] = settings->rgb_array[i*3];
		output_array[led_adr_arr[i]+1] = settings->rgb_array[i*3+1];
		output_array[led_adr_arr[i]] = settings->rgb_array[i*3+2];

	}

	while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, output_array, 101, 100) == HAL_BUSY){}
}*/

void setDEMOLEDRgb(uint8_t value){

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
	led_set_custom_size_overlay(&led_settings, led_clr, 0, 30);
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
}
