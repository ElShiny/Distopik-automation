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
#include "relay.h"


#include "mb.h"
#include "port.h"
#include "mbport.h"
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
	usRegDiscreteBuf[MODULE_TYPE] = 1; //0 for UI, 1 for AKT

	usRegDiscreteBuf[MODULE_ID_1] = 1; //UNIREL_POT is 0x1
	usRegDiscreteBuf[MODULE_ID_2] = 0;
	usRegDiscreteBuf[MODULE_ID_3] = 0;
	usRegDiscreteBuf[MODULE_ID_4] = 0;
	usRegDiscreteBuf[MODULE_ID_5] = 0;

	usRegHoldingBuf[RELAY_MCPS_ENABLED] = 0x1f;
	usRegHoldingBuf[RELAY_LINES_ENABLED] = 0x3ff;
	for(int i = 0; i<10; i++)usRegHoldingBuf[RELAY_MASK + i] = 0xff;
	for(int i = 0; i<10; i++)usRegHoldingBuf[RELAY_VALUES + i] = 0;

	relay_settings(&hrel1);

	//usRegHoldingBuf[SETTINGS_CHANGED] = 1;

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

void relay_settings(volatile relay_drv_t *settings){
	settings->lines_enabled = usRegHoldingBuf[RELAY_LINES_ENABLED];
	settings->mcps_enabled = usRegHoldingBuf[RELAY_MCPS_ENABLED];
	settings->values_changed = 1;

	write_u16_to_u8(settings->line_relay_mask, &usRegHoldingBuf[RELAY_MASK], 10);
	if(compare_arrays(settings->line_value, &usRegHoldingBuf[RELAY_VALUES], 10) == 0){
		settings->values_changed = 1;
		write_u16_to_u8(settings->line_value, &usRegHoldingBuf[RELAY_VALUES], 10);
	}


}

void write_u16_to_u8(volatile uint8_t *destination, volatile uint16_t *source, uint8_t length){
	for(int i = 0; i<length; i++){
		destination[i] = source[i];
	}
}
void write_u8_to_u16(volatile uint16_t *destination, volatile uint8_t *source, uint8_t length){
	for(int i = 0; i<length; i++){
		destination[i] = source[i];
	}
}
