/*
 * housekeeping.c
 *
 * Created: 17/04/2023 23:07:25
 *  Author: Matej
 */ 


#include "ACE.h"
#include "led_drv.h"
#include "housekeeping.h"
#include <avr/io.h>
#include <avr/interrupt.h>


uint8_t cnt;

void timersInit(void){
	
	TCCR0A = 1<<CTC0|1<<CS02; //timer enable
	OCR0A = 31;			//31 is 1 ms delay
	TIMSK0 = 1<<OCIE0A; //timer interrupt
	TCNT0 = 0;			//empty timer counter
}


void enableTimer(void){
	TIMSK0 |= 1<<OCIE0A;
}

void disableTimer(void){
	TIMSK0 &= ~(1<<OCIE0A);
}

uint16_t getTick(void){
	return tick;
}


ISR(TIMER0_COMPA_vect){
	
	if(cnt == 1){//reading ace values
		ace_val_new = readACEQuick();
		absoluteToRelative(&ace_val_old, &ace_val_new, &ace_val);
	}
	
	else if(cnt == 11){//setting leds
		setLEDRgb(ace_val);
		cnt = 0;
	}
	
	//cnt++;
	tick++;
}