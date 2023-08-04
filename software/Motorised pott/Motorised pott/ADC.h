/*
 * ADC.h
 *
 * Created: 16/05/2023 13:27:49
 *  Author: Matej
 */ 


#ifndef ADC_H_
#define ADC_H_

#define LEFT 0
#define RIGHT 1

#include <stdint.h>
#include "housekeeping.h"

typedef struct{
	//int volatile adc_val;
	uint16_t pot_pos;
	uint16_t adc_val_new;
	uint16_t adc_val_old;
	uint8_t volatile adc_changed;
	uint8_t volatile ui_changed;
	uint8_t volatile spi_changed;
}adc_t;

adc_t adc_rot;


void ADCInit(adc_t *adc);
uint16_t ADCRead(void);
void MovePot(adc_t *adc, pwm_t *pwm, uint8_t pos);
void MotorRot(uint8_t rotation);
void MotorEn(uint8_t en);


#endif /* ADC_H_ */