/*
 * ADC.c
 *
 * Created: 16/05/2023 13:27:32
 *  Author: Matej
 */ 


#include "ADC.h"
#include "housekeeping.h"
#include <stdlib.h>
#include <avr/io.h>


void ADCInit(adc_t *adc){
	
	ADMUX |= 1<<REFS0 | ADC4D;
	ADCSRA |= 1<<ADEN | 0b111;
	
	DDRD |= 1<<0;//motor sleep
	//PORTD |= 1<<PORTD0;
	
	DDRB |= 1<<DDB1;//motor en
	//PORTB |= 1<<PORTB1;
	
	DDRB |= 1<<DDB0;//motor smer
	
	adc->spi_changed = 1;
	adc->pot_pos = 4095;
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


void MovePot(adc_t *adc, pwm_t *pwm, uint8_t pos){
	
	if(adc->spi_changed == 0){
		MotorEn(0);
		return;
	}
	else MotorEn(1);

	if(pos < adc->adc_val_new){MotorRot(LEFT);}
	if(pos > adc->adc_val_new){MotorRot(RIGHT);}
		
	if(abs(pos - adc->adc_val_new) < 400)OCR1A = 75;
	else OCR1A = 150;
	
	if(pos == adc->adc_val_new){
		if(pwm->first_time == 1){pwm->first_time = 0;}
		MotorEn(0);
		adc->spi_changed = 0;
	}
		
	//MotorEn(1);

}