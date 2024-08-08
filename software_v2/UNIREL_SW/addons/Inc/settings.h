/*
 * settings.h
 *
 *  Created on: Jul 18, 2024
 *      Author: Matej
 */

#ifndef ADDONS_INC_SETTINGS_H_
#define ADDONS_INC_SETTINGS_H_

#include "relay.h"


//Definitions for locations within discrete input registers
#define SYSTEM_STATUS 	0 	 //IDK YET
#define MODULE_TYPE 	1	 // ACTUATOR OR UI

#define MODULE_ID_1 2		 //5 BIT OR MORE MODULE IDENTIFICATION, MSB LAST
#define MODULE_ID_2 3
#define MODULE_ID_3 4
#define MODULE_ID_4 5
#define MODULE_ID_5 6


//Definitions for locations within input registers

#define MCPS_CONNECTED 	0
#define RELAY_RETURN_VALUES	1

//Definitions for locations within coil registers
#define REMOTE_ADDRESS_CHANGE 0 	// CAN REMOTELY CHANGE MODBUS ID BY SETTING THIS ADDRESS TO ONE
#define REMOTE_SOFTWARE_RESET 10


//definitions for locations within holding registers
//#define HOLDING_START 1000
#define MODBUS_ADDRESS 		0		//LOCATION FOR ---------- ADDRESS
#define SETTINGS_CHANGED 	1

#define RELAY_MCPS_ENABLED 	2
#define RELAY_LINES_ENABLED 3
#define RELAY_MASK			10 // 10 bytes
#define RELAY_VALUES		20 // 10 bytes




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
void relay_settings(volatile relay_drv_t *settings);
void write_u16_to_u8(volatile uint8_t *destination, volatile uint16_t *source, uint8_t length);
void write_u8_to_u16(volatile uint16_t *destination, volatile uint8_t *source, uint8_t length);

//void ACE_settings(volatile ace_driver_t *settings);
//void led_settings(volatile led_driver_t *settings);

#endif /* ADDONS_INC_SETTINGS_H_ */
