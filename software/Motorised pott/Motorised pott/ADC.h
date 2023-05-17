/*
 * ADC.h
 *
 * Created: 16/05/2023 13:27:49
 *  Author: Matej
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>


void ADCInit(void);
uint8_t ADCRead(void);
void MovePot(uint8_t pos);

extern uint8_t pot_pos;


#endif /* ADC_H_ */