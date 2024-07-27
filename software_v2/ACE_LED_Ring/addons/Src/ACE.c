/*
 * ACE.c
 *
 *  Created on: Jul 15, 2024
 *      Author: Matej
 */

#include "ACE.h"
#include "main.h"
#include "settings.h"

void ACEInit(volatile ace_driver_t *ace){

	LL_GPIO_SetOutputPin(ACE_EN_GPIO_Port, ACE_EN_Pin);

	//ace->ace_val_old = readACEQuick();
	//ace->ace_max = 127;
	//ace->ace_led_changed = 1;

}

uint8_t readACEQuick(void){
	LL_GPIO_ResetOutputPin(ACE_EN_GPIO_Port, ACE_EN_Pin);

	asm("NOP\n\t"
		"NOP\n\t"
		"NOP\n\t");

	uint8_t val = encoderMap[ !LL_GPIO_IsInputPinSet(ACE8_GPIO_Port, ACE8_Pin) << 7
							 |!LL_GPIO_IsInputPinSet(ACE7_GPIO_Port, ACE7_Pin) << 6
							 |!LL_GPIO_IsInputPinSet(ACE6_GPIO_Port, ACE6_Pin) << 5
							 |!LL_GPIO_IsInputPinSet(ACE5_GPIO_Port, ACE5_Pin) << 4
							 |!LL_GPIO_IsInputPinSet(ACE4_GPIO_Port, ACE4_Pin) << 3
							 |!LL_GPIO_IsInputPinSet(ACE3_GPIO_Port, ACE3_Pin) << 2
							 |!LL_GPIO_IsInputPinSet(ACE2_GPIO_Port, ACE2_Pin) << 1
							 |!LL_GPIO_IsInputPinSet(ACE1_GPIO_Port, ACE1_Pin)
							 ];

	LL_GPIO_SetOutputPin(ACE_EN_GPIO_Port, ACE_EN_Pin);
	return val;
}

void ace_abs_to_rel(volatile ace_driver_t *ace){


	if(ace->val_old > 117 && ace->val_new < 10) ace->delta = ace->val_new- ace->val_old + 128;		//corrections for zero crossing
	else if(ace->val_old < 15 && ace->val_new > 110) ace->delta = ace->val_new - ace->val_old - 128;
	else ace->delta = ace->val_new - ace->val_old;

	ace->relative_val = ace->relative_val + ace->delta;

	ace->val_old = ace->val_new;

	//if (ace->ace_val > ace->ace_max)ace->ace_val = ace->ace_max;
	//if (ace->ace_val < 0)ace->ace_val = 0;

	if(ace->delta){
		ace->val_changed = 1;
		//ace->ace_led_changed = 1;
	}
}

void ACE_return_values(volatile ace_driver_t *ace, USHORT *input_buffer, USHORT *holding_buffer){
	input_buffer[ACE_ABSOLUTE_VALUE] = ace->absolute_val;
	input_buffer[ACE_RELATIVE_VALUE] = ace->relative_val;
	input_buffer[ACE_DELTA] = ace->delta;
	input_buffer[ACE_CALCULATED_ANGLE] = ace->angle;


	//holding_buffer[ACE_INCOMING_VALUE_CHANGE] = ace->incoming_val_change;


}

void ACE_settings_parser(volatile ace_driver_t *ace){

	if(ace->disable) return;

	ace->absolute_val = readACEQuick();

	if(ace->absolute_val == ace->val_new) return;
	ace->val_new = ace->absolute_val;

	ace_abs_to_rel(ace);


	switch (ace->mode) {
		case 0:
			ACE_mode_0(ace);
			break;
		case 1:
			ACE_mode_1(ace);
			break;
		case 2:
			ACE_mode_2(ace);
			break;

		default:

			break;
	}

	return;
}

void ACE_mode_0(volatile ace_driver_t *ace){

	ace->angle = (float)ACE_STEP_ANGLE * (float)ace->absolute_val;

	if(ace->angle <= ace->start_angle) ace->angle = ace->start_angle;
	if(ace->angle >= ace->stop_angle) ace->angle = ace->stop_angle;

	return;
}

void ACE_mode_1(volatile ace_driver_t *ace){

	ace->angle = (float)ACE_STEP_ANGLE * (float)ace->relative_val;

	if(ace->angle <= ace->start_angle) ace->angle = ace->start_angle;
	if(ace->angle >= ace->stop_angle) ace->angle = ace->stop_angle;

	return;
}

void ACE_mode_2(volatile ace_driver_t *ace){
	float scaling_factor = ace->scale/(float)100;

	ace->angle += (float)ACE_STEP_ANGLE * (float)ace->delta * (float)scaling_factor;

	if(ace->angle <= ace->start_angle) ace->angle = ace->start_angle;
	if(ace->angle >= ace->stop_angle) ace->angle = ace->stop_angle;

	return;
}



