/*
 * motor.h
 *
 *  Created on: Aug 1, 2024
 *      Author: Matej
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stdint.h"

typedef struct{
	uint16_t motor_position;
	uint16_t old_position;
	uint16_t new_position;
	uint8_t started;
	int delta;
	uint8_t speed;
	uint8_t direction;
	uint8_t user_request;

}volatile motor_driver_t;

extern motor_driver_t hmot1;


void motor_init(volatile motor_driver_t *motor);
void motor_left_speed(uint16_t speed);
void motor_right_speed(uint16_t speed);
void motor_speed(volatile motor_driver_t *motor, uint16_t speed);
void motor_enable(void);
void motor_disable(void);
void motor_left(volatile motor_driver_t *motor);
void motor_right(volatile motor_driver_t *motor);
void motor_brake(volatile motor_driver_t *motor);
void motor_stop(volatile motor_driver_t *motor);
uint16_t motor_get_position(volatile motor_driver_t *motor);
void motor_calculate_delta(volatile motor_driver_t *motor);
void motor_move_to_pos(volatile motor_driver_t *motor, uint16_t position);


#endif /* INC_MOTOR_H_ */
