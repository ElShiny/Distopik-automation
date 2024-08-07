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
#include "motor.h"

#include <stdlib.h>


extern USHORT usRegHoldingBuf[REG_HOLDING_NREGS];
extern USHORT usRegCoilBuf[REG_COIL_NREGS];
extern USHORT usRegInputBuf[REG_INPUT_NREGS];

int cycles = 0;
int location = 0;

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
		location = usRegHoldingBuf[MOTT_LOCATION];
		hmot1.user_request = 1;

		//RESET SETTINGS CHANGED BIT
		reset_settings_changed();
	}
	//ADD SETTIGS PARSERS



	if(cycles == 5){
		motor_get_position(&hmot1);
		motor_calculate_delta(&hmot1);
		motor_move_to_pos(&hmot1, location);
		cycles = 0;
	}
	else cycles++;

	//if(dt<hmot1.delta)dt=abs(hmot1.delta);

	//usRegInputBuf[0] = 15;
	usRegInputBuf[0] = hmot1.motor_position;




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
