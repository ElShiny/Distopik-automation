/*
 * ADC.c
 *
 * Created: 16/05/2023 13:27:32
 *  Author: Matej
 */ 


#include "ADC.h"
#include "housekeeping.h"
#include <avr/io.h>


void ADCInit(adc_t *adc){
	
	ADMUX |= 1<<REFS0 | ADC4D;
	ADCSRA |= 1<<ADEN | 0b111;
	
	DDRD |= 1<<0;//motor sleep
	//PORTD |= 1<<PORTD0;
	
	DDRB |= 1<<DDB1;//motor en
	PORTB |= 1<<PORTB1;
	
	DDRB |= 1<<DDB0;//motor smer
	
	
	adc->pot_pos = ADCRead();
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

void PWMInit(){
	
}

void MotorRot(uint8_t rotation){
	switch(rotation){
		case POT_LEFT:
			PORTB |= 1<<PORTB0;
			break;
			
		case POT_RIGHT:
			PORTB &= ~(1<<PORTB0);
			break;
	}
}
void MotorEn(uint8_t en){
	if(en) PORTD &= ~(1<<PORTD0);
	else PORTD |= 1<<PORTD0;
}


void MovePot(adc_t *adc, uint8_t pos){

	

}