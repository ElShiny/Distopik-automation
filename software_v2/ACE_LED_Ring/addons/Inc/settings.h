/*
 * settings.h
 *
 *  Created on: Jul 18, 2024
 *      Author: Matej
 */

#ifndef ADDONS_INC_SETTINGS_H_
#define ADDONS_INC_SETTINGS_H_

//Definitions for locations within discrete input registers
#define SYSTEM_STATUS 	0 	 //IDK YET
#define MODULE_TYPE 	1	 // ACTUATOR OR UI

#define MODULE_ID_1 2		 //5 BIT OR MORE MODULE IDENTIFICATION, MSB LAST
#define MODULE_ID_2 3
#define MODULE_ID_3 4
#define MODULE_ID_4 5
#define MODULE_ID_5 6


//Definitions for locations within input registers

#define ACE_OUTGOING_VALUE 0



//Definitions for locations within coil registers
#define REMOTE_ADDRESS_CHANGE 0 	// CAN REMOTELY CHANGE MODBUS ID BY SETTING THIS ADDRESS TO ONE
#define REMOTE_SOFTWARE_RESET 10


//definitions for locations within holding registers
#define MODBUS_ADDRESS 		0		//LOCATION FOR PREFERRED ADDRESS
#define SETTINGS_CHANGED 	1

#define ACE_MODE 			2
#define ACE_DISABLE 		3
#define ACE_INCOMING_VALUE 	4
#define ACE_START_ANGLE		5
#define ACE_STOP_ANGLE		6
#define ACE_SCALE			7


#define LED_MODE			20
#define LED_DISABLE			21
#define LED_START_ANGLE		22
#define LED_STOP_ANGLE		23
#define LED_FRONT_COLOR		24 // 3 REGS
#define LED_BACK_COLOR		27 // 3 REGS
#define LED_ARRAY			30 // 90 REGS

#define FLASH_MODBUS_ADDRESS_LOCATION 0x08007800


typedef struct{
	uint8_t modbus_change;
}settings_t;

extern volatile settings_t settings;


void system_settings(void);
void addres_change_remote(void);
void address_change_local(void);
void btn_adress_change();
void MB_startup(void);


#endif /* ADDONS_INC_SETTINGS_H_ */
