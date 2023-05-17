/*
 * ADC.c
 *
 * Created: 16/05/2023 13:27:32
 *  Author: Matej
 */ 


#include "ADC.h"
#include "housekeeping.h"
#include <avr/io.h>

uint8_t pot_pos = 0;

void ADCInit(void){
	
	ADMUX |= 1<<REFS0 | ADC4D;
	ADCSRA |= 1<<ADEN | 0b111;
	
	DDRD |= 1<<0;//motor sleep
	//PORTD |= 1<<PORTD0;
	
	DDRB |= 1<<DDB1;//motor en
	PORTB |= 1<<PORTB1;
	
	DDRB |= 1<<DDB0;//motor smer
	
	
	//adc_val_old = ADCRead();
}

uint8_t ADCRead(void){
	ADCSRA |= 1<<ADSC;
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA |= 1<<ADIF;
	ADCSRA &= ~(0<<ADIF);
	uint8_t val_low = ADCL;
	uint16_t val = ADCH<<8 | val_low;
	return (uint8_t)(val>>2);
}

void MovePot(uint8_t pos){
	//if(old_pos == pos) break;
	
	if(pos < adc_val_new){
		PORTB |= 1<<PORTB0;
	}
	if(pos > adc_val_new){
		PORTB &= ~(1<<PORTB0);
	}
	if(((pos-5) < adc_val_new) && ((pos+5) > adc_val_new)){
		PORTD &= ~(1<<PORTD0);
	}
	else PORTD |= 1<<PORTD0;
}