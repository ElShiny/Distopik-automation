/*
 * settings.c
 *
 *  Created on: Jul 18, 2024
 *      Author: Matej
 */


#include "main.h"
#include "gpio.h"
#include "settings.h"
#include "housekeep.h"
#include "mb.h"
#include "port.h"
#include "mbport.h"
#include "i2c.h"
#include "led.h"
#include "ACE.h"
#include "string.h"

//#include "core_cm3.h"

uint16_t btn_time = 0;


extern USHORT   usRegDiscreteBuf[REG_DISCRETE_NREGS];
extern USHORT   usRegCoilBuf[REG_COIL_NREGS];
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];




void system_settings(void){

	btn_press_time();

	if(usRegCoilBuf[REMOTE_ADDRESS_CHANGE] == 1) btn_adress_change();

	return;
}

void settings_init(void){

	//set up identificators
	usRegDiscreteBuf[MODULE_TYPE] = 0; //0 for UI

	usRegDiscreteBuf[MODULE_ID_1] = 1; //ACE led ring is 0x1
	usRegDiscreteBuf[MODULE_ID_2] = 0;
	usRegDiscreteBuf[MODULE_ID_3] = 0;
	usRegDiscreteBuf[MODULE_ID_4] = 0;
	usRegDiscreteBuf[MODULE_ID_5] = 0;

	//MODES FOR MODULES
	usRegHoldingBuf[ACE_MODE] = 1;
	usRegHoldingBuf[ACE_DISABLE] = 0;
	usRegHoldingBuf[ACE_INCOMING_VALUE] = 100;
	usRegHoldingBuf[ACE_INCOMING_VALUE_CHANGE] = 0;
	usRegHoldingBuf[ACE_START_ANGLE] = 0;
	usRegHoldingBuf[ACE_STOP_ANGLE]  = 360;
	usRegHoldingBuf[ACE_SCALE] = 1;

	usRegHoldingBuf[LED_MODE] = 0;
	usRegHoldingBuf[LED_DISABLE] = 0;
	usRegHoldingBuf[LED_START_LED] = 24;
	usRegHoldingBuf[LED_STOP_LED] = 348;
	usRegHoldingBuf[LED_SWITCH_STEPS] = 5;

	usRegHoldingBuf[LED_FRONT_COLOR] = 255;
	usRegHoldingBuf[LED_FRONT_COLOR + 1] = 255;
	usRegHoldingBuf[LED_FRONT_COLOR + 2] = 255;
	usRegHoldingBuf[LED_BACK_COLOR] = 100;
	usRegHoldingBuf[LED_BACK_COLOR + 1] = 0;
	usRegHoldingBuf[LED_BACK_COLOR + 2] = 0;


	usRegHoldingBuf[SETTINGS_CHANGED] = 1;

	return;
}

void btn_press_time(void){
	if(!LL_GPIO_IsInputPinSet(USR_BTN_GPIO_Port, USR_BTN_Pin) && (btn_time<=200)) btn_time++;
	else if(!LL_GPIO_IsInputPinSet(USR_BTN_GPIO_Port, USR_BTN_Pin) && btn_time>=200){
		btn_adress_change();
		btn_time = 0;
	}
	else btn_time = 0;
}

void btn_adress_change(void){
	__disable_irq();
	HAL_FLASH_Unlock();

	FLASH_PageErase(15);
	CLEAR_BIT (FLASH->CR, (FLASH_CR_PER));
	uint64_t data = 0x1;

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_MODBUS_ADDRESS_LOCATION, data);
	CLEAR_BIT (FLASH->CR, (FLASH_CR_PG));
	HAL_FLASH_Lock();
	HAL_NVIC_SystemReset();

	return;
}

void MB_startup(void){

	uint32_t *p = (uint32_t *)FLASH_MODBUS_ADDRESS_LOCATION;

	if (*p == 0xFFFFFFFF) {
		uint64_t data = 10;

		HAL_FLASH_Unlock();
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_MODBUS_ADDRESS_LOCATION, data);
		HAL_FLASH_Lock();

		HAL_NVIC_SystemReset();
	}

	else if(*p == 0x1){

		if(eMBInit(MB_RTU, 0x1, 0, 921600, MB_PAR_NONE) != MB_ENOERR )Error_Handler();
		if(eMBEnable() != MB_ENOERR)Error_Handler();
		__enable_irq();

		usRegHoldingBuf[MODBUS_ADDRESS] = 0;
		usRegHoldingBuf[SETTINGS_CHANGED] = 0;

		while(1){
			if(eMBPoll()!=MB_ENOERR)Error_Handler();

			if(usRegHoldingBuf[MODBUS_ADDRESS] != 0
			   && usRegHoldingBuf[SETTINGS_CHANGED] == 1)break;

			HAL_Delay(100);
			for(int i = 0 ; i<30; i++){
			  uint8_t arr6[4] = {led_adr_arr[i], 0,0,255};
			  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr6, 4, 100) == HAL_BUSY){}
			}
			HAL_Delay(100);
			for(int i = 0 ; i<30; i++){
			  uint8_t arr6[4] = {led_adr_arr[i], 255,0,0};
			  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr6, 4, 100) == HAL_BUSY){}
			}
		}

		HAL_FLASH_Unlock();

		FLASH_PageErase(15);
		CLEAR_BIT (FLASH->CR, (FLASH_CR_PER));
		uint64_t data = usRegHoldingBuf[MODBUS_ADDRESS];

		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_MODBUS_ADDRESS_LOCATION, data);
		CLEAR_BIT (FLASH->CR, (FLASH_CR_PG));
		HAL_FLASH_Lock();

		//HAL_Delay(3000);
		HAL_NVIC_SystemReset();

	}
	else if(*p>255){
		Error_Handler();
	}
	else{

		//HAL_Delay(10000);
		if(eMBInit(MB_RTU, (UCHAR) *p, 0, 921600, MB_PAR_NONE) != MB_ENOERR )Error_Handler();
		if(eMBEnable() != MB_ENOERR)Error_Handler();

		usRegDiscreteBuf[SYSTEM_STATUS] = 1;
		__enable_irq();
	}
}

void ACE_settings(volatile ace_driver_t *settings){

	settings->mode = usRegHoldingBuf[ACE_MODE];
	settings->disable = usRegHoldingBuf[ACE_DISABLE];
	settings->incoming_angle = usRegHoldingBuf[ACE_INCOMING_VALUE];
	settings->incoming_val_change = usRegHoldingBuf[ACE_INCOMING_VALUE_CHANGE];
	settings->start_angle = usRegHoldingBuf[ACE_START_ANGLE];
	settings->stop_angle = usRegHoldingBuf[ACE_STOP_ANGLE];
	settings->scale = usRegHoldingBuf[ACE_SCALE];
	settings->setting_changed = 1;
}

void led_settings(volatile led_driver_t *settings){

	for(int i = 0; i <90; i++)settings->rgb_array[i] = usRegHoldingBuf[LED_ARRAY + i];
	for(int i = 0; i <3; i++)settings->back_color[i] = usRegHoldingBuf[LED_BACK_COLOR + i];
	for(int i = 0; i <3; i++)settings->front_color[i] = usRegHoldingBuf[LED_FRONT_COLOR + i];

	settings->mode = usRegHoldingBuf[LED_MODE];
	settings->disable = usRegHoldingBuf[LED_DISABLE];
	settings->start_angle = usRegHoldingBuf[LED_START_LED];
	settings->stop_angle = usRegHoldingBuf[LED_STOP_LED];
	settings->steps = usRegHoldingBuf[LED_SWITCH_STEPS];
	settings->settings_changed = 1;
}
