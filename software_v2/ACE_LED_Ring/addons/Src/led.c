/*
 * led.c
 *
 *  Created on: Apr 19, 2024
 *      Author: Matej
 */

#include "led.h"
#include "gpio.h"
#include "i2c.h"
#include "port.h"


void led_init(void)
{
	  uint8_t arr[2] = {0xCF, 0xAE};


	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr, 2, 100) == HAL_BUSY){}
	  uint8_t arr1[2] = {0xA0, 0x01};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr1, 2, 100) == HAL_BUSY){}
	  uint8_t arr2[2] = {0xA1, 30};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr2, 2, 100) == HAL_BUSY){}

		for(int a = 0x90; a < 0x9F; a++){
			uint8_t arr3[2] = {a, 60};
			while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr3, 2, 100) == HAL_BUSY){}
		}

		for(int i = 0 ; i<30; i++){
		  uint8_t arr5[4] = {led_adr_arr[i], 0,255,100};
		  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr5, 4, 100) == HAL_BUSY){}
		}

}

void led_set(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {

	for(int i = 0 ; i<30; i++){
	  uint8_t arr5[4] = {led_adr_arr[i], 255,0,0};
	  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr5, 4, 100) == HAL_BUSY){}
	}

	uint8_t arr[4] = { led_adr_arr[led], r, g, b };
	while (HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr, 4, 100) == HAL_BUSY) {
	}
}

