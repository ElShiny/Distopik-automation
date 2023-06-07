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



void timersInit(hskp_t *hskp){
	
	TCCR0A = 1<<CTC0|1<<CS02; //timer enable
	OCR0A = 30;			//31 is 1 ms delay
	TIMSK0 = 1<<OCIE0A; //timer interrupt
	TCNT0 = 0;			//empty timer counter
	hskp->en = 1;
}


void enableTimer(void){
	TIMSK0 |= 1<<OCIE0A;
}

void disableTimer(void){
	TIMSK0 &= ~(1<<OCIE0A);
}

void enableHSKP(hskp_t *hskp){
	hskp->en = 1;
}

void disableHSKP(hskp_t *hskp){
	hskp->en = 0;
}

uint16_t getTick(hskp_t *hskp){
	return hskp->tick;
}


ISR(TIMER0_COMPA_vect, ISR_NOBLOCK){
		
	disableTimer();
	if(housekp.cnt == 1 && housekp.en && led_settings.led_en){//reading ace values
		ace_rot.ace_val_new = readACEQuick();
		absoluteToRelative(&ace_rot);
	}
	
	else if(housekp.cnt >= 10 && housekp.en){//setting leds
		if((led_settings.mode == 0)&&ace_rot.ace_led_changed){
			set_ring(&led_settings, &ace_rot, ace_rot.ace_val);
		}
		else if(led_settings.mode == 1){
			setLEDArray(&led_settings);
		}
		ace_rot.ace_led_changed = 0;
		housekp.cnt = 0;
	}
	enableTimer();
	
	housekp.cnt++;
	housekp.tick++;
}