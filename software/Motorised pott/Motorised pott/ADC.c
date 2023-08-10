/*
 * ADC.c
 *
 * Created: 16/05/2023 13:27:32
 *  Author: Matej
 */ 

#ifndef F_CPU
#define  F_CPU 8000000
#endif

#include "ADC.h"
#include "housekeeping.h"
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>






void ADCInit(adc_t *adc){
	
	ADMUX |= 1<<REFS0 | ADC4D;
	ADCSRA |= 1<<ADEN | 0b111;
	
	DDRD |= 1<<0;//motor sleep
	//PORTD |= 1<<PORTD0;
	
	DDRB |= 1<<DDB1;//motor en
	//PORTB |= 1<<PORTB1;
	
	DDRB |= 1<<DDB0;//motor smer
	
	adc->spi_changed = 1;
	adc->pot_pos = 0;
}

uint16_t ADCRead(void){
	ADCSRA |= 1<<ADSC;
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA |= 1<<ADIF;
	ADCSRA &= ~(0<<ADIF);
	uint8_t val_low = ADCL;
	uint16_t val = ADCH<<8 | val_low;
	return val;
}

void MotorRot(uint8_t rotation){
	switch(rotation){
		case LEFT:
			PORTB |= 1<<PORTB0;
			break;
			
		case RIGHT:
			PORTB &= ~(1<<PORTB0);
			break;
	}
}
void MotorEn(uint8_t en){
	if(!en) PORTD &= ~(1<<PORTD0);
	else PORTD |= 1<<PORTD0;
}


void MovePot(adc_t *adc, pwm_t *pwm, uint16_t pos){
	
	if(adc->spi_changed == 0){
		MotorEn(0);
		return;
	}
	else MotorEn(1);

	if(pos < adc->adc_val_new){MotorRot(LEFT);}
	if(pos > adc->adc_val_new){MotorRot(RIGHT);}
		
	if(abs(pos - adc->adc_val_new) < 200)OCR1A = 73;
	else OCR1A = 150;
	
	if(pos == adc->adc_val_new){
		if(pwm->first_time == 1){
			MotorRot(LEFT);
			_delay_ms(100);
			pwm->first_time = 0;
			}
		MotorEn(0);
		adc->spi_changed = 0;
	}
		
	//MotorEn(1);

}

int movingAvg(int *ptrArrNumbers, long *ptrSum, int pos, int len, int nextNum)
{
	//Subtract the oldest number from the prev sum, add the new number
	*ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
	//Assign the nextNum to the position in the array
	ptrArrNumbers[pos] = nextNum;
	//return the average
	return *ptrSum / len;
}