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


extern USHORT usRegHoldingBuf[REG_HOLDING_NREGS];
extern USHORT usRegCoilBuf[REG_COIL_NREGS];

void start_housekeeping(void){
	HAL_TIM_Base_Start_IT(&htim14);
}

void stop_housekeeping(void){

}



void housekeeping_tasks(void){

	//check for settings updates
	//if(check_for_settings_changes()){
		//SYSTEM SETTINGS
		system_settings();
		//ACE SETTINGS
		//LED SETTINGS

	//}

	//ACE execute
	//LED execute

	return;
}

uint8_t check_for_settings_changes(){

	if (usRegHoldingBuf[SETTINGS_CHANGED] == 1) return 1;
	return 0;
}
