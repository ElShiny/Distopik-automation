/*
 * motor.c
 *
 *  Created on: Aug 1, 2024
 *      Author: Matej
 */

#include "motor.h"
#include "tim.h"
#include "adc.h"
#include <stdlib.h>


motor_driver_t hmot1;


void motor_init(volatile motor_driver_t *motor){
	motor_get_position(motor);
	motor->old_position = motor->motor_position;

	HAL_TIM_Base_Start(&htim1);
}

void motor_left_speed(uint16_t speed){
	TIM1->CCR1 = speed * 10;
}

void motor_right_speed(uint16_t speed){
	TIM1->CCR4 = speed * 10;
}

void motor_speed(volatile motor_driver_t *motor, uint16_t speed){
	motor->speed = speed;
	TIM1->CCR4 = speed * 10;
	TIM1->CCR1 = speed * 10;
}

void motor_enable(void){
	LL_GPIO_SetOutputPin(MOT_SLEEP_GPIO_Port, MOT_SLEEP_Pin);
}

void motor_disable(void){
	LL_GPIO_ResetOutputPin(MOT_SLEEP_GPIO_Port, MOT_SLEEP_Pin);
}

void motor_left(volatile motor_driver_t *motor){
	motor->direction = 1;
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void motor_right(volatile motor_driver_t *motor){
	motor->direction = 2;
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void motor_brake(volatile motor_driver_t *motor){
	motor_speed(motor, 110);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void motor_stop(volatile motor_driver_t *motor){
	motor_speed(motor, 0);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

uint16_t motor_get_position(volatile motor_driver_t *motor){
	int avg = 0;
	for(int i = 0; i<10; i++){
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 2);
		avg += HAL_ADC_GetValue(&hadc1);
	}

	motor->new_position = avg/(float)10;
	motor->motor_position = motor->new_position;
	return motor->motor_position;
}

void motor_calculate_delta(volatile motor_driver_t *motor){
	if(motor->new_position == motor->old_position){
		motor->delta = 0;
		return;
	}

	motor->delta = motor->new_position - motor->old_position;
	motor->old_position = motor->new_position;
	return;
}

void motor_move_to_pos(volatile motor_driver_t *motor, uint16_t position){

	//if(motor->user_request == 0)motor->started = 30;

	uint16_t delta = abs(motor->motor_position - position);

	if(delta < 20 && motor->started == 0){
		motor_stop(motor);
		motor->started = 0;
		return;
	}
	else if(motor->motor_position < position && motor->started == 0){
		//go left
		if(delta < 400) motor_speed(motor, 50);
		else motor_speed(motor, 100);

		motor_left(motor);
		motor->started = 1;
		return;
	}
	else if(motor->motor_position > position && motor->started == 0){
		//go right
		if(delta < 400) motor_speed(motor, 50);
		else motor_speed(motor, 100);

		motor_right(motor);
		motor->started = 1;
		return;
	}


	if(motor->started == 1){
		//driving PI
		if(delta > 400){
			motor_speed(motor, 100);

			//if(abs(motor->delta) > 30 )motor_speed(motor, motor->speed-1);
			//if(abs(motor->delta) < 30 )motor_speed(motor, motor->speed+1);
			//if(motor->speed >= 100) motor->speed = 99;
			//if(motor->speed < 1) motor->speed = 1;
		}
		if(delta < 400 && delta > 200) motor_speed(motor, 10);
		if(delta < 200)	motor_speed(motor, 5);

		if(motor->motor_position > position && motor->direction == 1){
			motor_right(motor);
		}
		if(motor->motor_position < position && motor->direction == 2){
			motor_left(motor);
		}

		if(delta < 10){
			motor_brake(motor);
			motor->started = 2;
		}

//		if((motor->delta == 0) && (motor->speed >3)){
//			motor_brake(motor);
//			motor->started = 22;
//		}

	}

	if(motor->started >= 2){
		motor->started += motor->started;
	}

	if(motor->started > 20){
		motor_stop(motor);
		motor->started = 0;
		motor->direction = 0;
		motor->user_request = 0;
	}

}


