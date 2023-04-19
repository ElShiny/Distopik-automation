/*
 * housekeeping.c
 *
 * Created: 17/04/2023 23:07:25
 *  Author: Matej
 */ 


#include "ACE.h"
#include "led_drv.h"
#include "housekeeping.h"
#include "settings.h"
#include <avr/io.h>
#include <avr/interrupt.h>


uint8_t cnt;
volatile uint8_t hskp_en = 0;

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
		ace_val_new = readACEQuick();
		absoluteToRelative(&ace_val_old, &ace_val_new, &ace_val);
	}
	
	else if(cnt >= 10 && hskp_en){//setting leds
		if(led_settings.mode == 0){
			setDEMOLEDRgb(ace_val);
		}
		else if(led_settings.mode == 1){
			setLEDArray(&led_settings);
		}
		cnt = 1;
	}
	enableTimer();
	
	cnt++;
	tick++;
}