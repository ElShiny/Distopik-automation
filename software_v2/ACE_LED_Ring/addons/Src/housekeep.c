/*
 * housekeep.c
 *
 *  Created on: Jul 16, 2024
 *      Author: Matej
 */

#include "main.h"
#include "mb.h"
#include "port.h"
#include "mbport.h"
#include "settings.h"
#include "tim.h"
#include "housekeep.h"


extern USHORT usRegHoldingBuf[REG_HOLDING_NREGS];
extern USHORT usRegCoilBuf[REG_COIL_NREGS];
extern USHORT usRegInputBuf[REG_INPUT_NREGS];

void start_housekeeping(void){
	HAL_TIM_Base_Start_IT(&htim14);
}

void stop_housekeeping(void){
	HAL_TIM_Base_Stop_IT(&htim14);
}



void housekeeping_tasks_callback(void){

	//SYSTEM SETTINGS
	system_settings();

	//check for settings updates
	if(check_for_settings_changes()){

		//ACE SETTINGS
		ACE_settings(&hace1);
		//LED SETTINGS
		led_settings(&hled1);
		//RESET SETTINGS CHANGED BIT
		reset_settings_changed();
	}

	ACE_settings_parser(&hace1);

	if(hace1.incoming_val_change == 1){
		hled1.rot_angle = hace1.incoming_angle;
		hace1.incoming_val_change = 0;
		usRegHoldingBuf[ACE_INCOMING_VALUE_CHANGE] = 0;
		led_settings_parser(&hled1);
	}
	else if(hace1.val_changed == 1 || hled1.settings_changed == 1){
		hled1.rot_angle = hace1.angle;
		hace1.val_changed = 0;
		hled1.settings_changed == 0;
		led_settings_parser(&hled1);
	}

	ACE_return_values(&hace1, usRegInputBuf, usRegHoldingBuf);
	led_return_values(&hled1, usRegInputBuf);

	//ACE execute
	//LED execute

	return;
}

uint8_t check_for_settings_changes(void){
	if (usRegHoldingBuf[SETTINGS_CHANGED] == 1) return 1;
	return 0;
}

void reset_settings_changed(void){
	usRegHoldingBuf[SETTINGS_CHANGED] = 0;
	return;
}
