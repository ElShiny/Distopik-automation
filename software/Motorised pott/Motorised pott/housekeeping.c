/*
 * housekeeping.c
 *
 * Created: 17/04/2023 23:07:25
 *  Author: Matej
 */ 



#include "housekeeping.h"
#include "settings.h"
#include "ADC.h"
#include <avr/io.h>
#include <avr/interrupt.h>


uint8_t cnt = 0;
volatile uint8_t hskp_en = 0;
volatile uint8_t adc_changed = 0;
uint8_t adc_val_new = 0;
uint8_t adc_val_old = 0;

void timersInit(void){
	
	TCCR0A = 1<<CTC0|1<<CS02; //timer enable
	OCR0A = 30;			//31 is 1 ms delay
	TIMSK0 = 1<<OCIE0A; //timer interrupt
	TCNT0 = 0;			//empty timer counter
	hskp_en = 1;
}


void enableTimer(void){
	TIMSK0 |= 1<<OCIE0A;
}

void disableTimer(void){
	TIMSK0 &= ~(1<<OCIE0A);
}

void enableHSKP(void){
	hskp_en = 1;
}

void disableHSKP(void){
	hskp_en = 0;
}

uint16_t getTick(void){
	return tick;
}


ISR(TIMER0_COMPA_vect, ISR_NOBLOCK){
		
	disableTimer();
	if(cnt == 1 && hskp_en){//reading ace values
		adc_val_new = ADCRead();
		if(adc_val_new != adc_val_old){
			adc_changed = 1;
			adc_val_old = adc_val_new;
			}
	}
	
	else if(cnt >= 10 && hskp_en){//setting leds
		MovePot(pot_pos);
		cnt = 0;
	}
	enableTimer();
	
	cnt++;
	tick++;
}