/*
 * ADC.h
 *
 * Created: 16/05/2023 13:27:49
 *  Author: Matej
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

typedef struct{
	int volatile adc_val;
	uint8_t pot_pos;
	uint8_t adc_val_new;
	uint8_t adc_val_old;
	uint8_t volatile adc_changed;
}adc_t;

adc_t adc_rot;


void ADCInit(adc_t *adc);
uint8_t ADCRead(void);
void MovePot(adc_t *adc, uint8_t pos);


#endif /* ADC_H_ */