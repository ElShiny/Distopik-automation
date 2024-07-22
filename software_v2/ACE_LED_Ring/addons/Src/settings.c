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

//#include "core_cm3.h"

uint16_t btn_time = 0;
extern USHORT   usRegHoldingStart;
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

extern USHORT   usRegCoilStart;
extern USHORT   usRegCoilBuf[REG_COIL_NREGS];

void system_settings(void){

	//if(CoilRegs[REMOTE_SOFTWARE_RESET]) NVIC_SystemReset();
	//if(CoilRegs[REMOTE_ADDRESS_CHANGE]) addres_change_remote();
	if(!LL_GPIO_IsInputPinSet(USR_BTN_GPIO_Port, USR_BTN_Pin) && (btn_time<=200)) btn_time++;
	else if(!LL_GPIO_IsInputPinSet(USR_BTN_GPIO_Port, USR_BTN_Pin) && btn_time>=200){
		btn_adress_change();
		//settings.modbus_change = 1;
		btn_time = 0;
	}
	else btn_time = 0;

	if(usRegCoilBuf[REMOTE_ADDRESS_CHANGE] != 0)btn_adress_change();

	return;
}

void btn_adress_change(){
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
		uint64_t data = 0x10;

		for(int i = 0 ; i<30; i++){
		  uint8_t arr6[4] = {led_adr_arr[i], 255,0,0};
		  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr6, 4, 100) == HAL_BUSY){}
		}
		HAL_Delay(500);

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
		for(int i = 0 ; i<30; i++){
		  uint8_t arr6[4] = {led_adr_arr[i], 0,255,0};
		  while(HAL_I2C_Master_Transmit(&hi2c1, IS3_ADR, arr6, 4, 100) == HAL_BUSY){}
		}
		//HAL_Delay(10000);
		if(eMBInit(MB_RTU, (UCHAR) *p, 0, 921600, MB_PAR_NONE) != MB_ENOERR )Error_Handler();
		if(eMBEnable() != MB_ENOERR)Error_Handler();
		__enable_irq();
	}
}
