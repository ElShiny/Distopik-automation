/*
 * settings.h
 *
 *  Created on: Jul 18, 2024
 *      Author: Matej
 */

#ifndef ADDONS_INC_SETTINGS_H_
#define ADDONS_INC_SETTINGS_H_

#include "led.h"
#include "ACE.h"

//Definitions for locations within discrete input registers
#define SYSTEM_STATUS 	0 	 //IDK YET
#define MODULE_TYPE 	1	 // ACTUATOR OR UI

#define MODULE_ID_1 2		 //5 BIT OR MORE MODULE IDENTIFICATION, MSB LAST
#define MODULE_ID_2 3
#define MODULE_ID_3 4
#define MODULE_ID_4 5
#define MODULE_ID_5 6


//Definitions for locations within input registers

#define ACE_ABSOLUTE_VALUE 0
#define ACE_RELATIVE_VALUE 1
#define ACE_DELTA 2
#define ACE_CALCULATED_ANGLE 3
#define LED_OFFSET 4
#define LED_RETURN_ARRAY 10



//Definitions for locations within coil registers
#define REMOTE_ADDRESS_CHANGE 0 	// CAN REMOTELY CHANGE MODBUS ID BY SETTING THIS ADDRESS TO ONE
#define REMOTE_SOFTWARE_RESET 10


//definitions for locations within holding registers
#define MODBUS_ADDRESS 		0		//LOCATION FOR PREFERRED ADDRESS
#define SETTINGS_CHANGED 	1

#define ACE_MODE 				2
#define ACE_DISABLE 			3
#define ACE_INCOMING_VALUE 		4
#define ACE_INCOMING_VALUE_CHANGE 	5
#define ACE_START_ANGLE			6
#define ACE_STOP_ANGLE			7
#define ACE_SCALE				8



#define LED_MODE			20
#define LED_DISABLE			21
#define LED_START_LED		22
#define LED_STOP_LED		23
#define LED_SWITCH_STEPS	24
#define LED_FRONT_COLOR		30 // 3 REGS
#define LED_BACK_COLOR		33 // 3 REGS
#define LED_ARRAY			40 // 90 REGS

#define FLASH_MODBUS_ADDRESS_LOCATION 0x08007800


typedef struct{
	uint8_t modbus_change;
}settings_t;

extern volatile settings_t settings;


void system_settings(void);

void settings_init(void);
void btn_press_time(void);
void addres_change_remote(void);
void address_change_local(void);
void btn_adress_change(void);
void MB_startup(void);

void ACE_settings(volatile ace_driver_t *settings);
void led_settings(volatile led_driver_t *settings);

#endif /* ADDONS_INC_SETTINGS_H_ */
