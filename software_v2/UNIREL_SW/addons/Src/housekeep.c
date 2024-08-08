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

		//ADD CUSTOM SETTINGS
		relay_settings(&hrel1);

		//RESET SETTINGS CHANGED BIT
		reset_settings_changed();
	}
	//ADD SETTIGS PARSERS
	relay_settings_parser(&hrel1);
	relay_return_values(&hrel1, usRegInputBuf);


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
