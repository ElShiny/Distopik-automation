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
#include <stdlib.h>


void timersInit(hskp_t *hskp){
	
	TCCR0A = 1<<CTC0|1<<CS02; //timer enable
	OCR0A = 30;			//31 is 1 ms delay
	TIMSK0 = 1<<OCIE0A; //timer interrupt
	TCNT0 = 0;			//empty timer counter
	hskp->en = 1;
}

void PWMInit(pwm_t *pwm){
	
	TCCR1A = 1<<COM1A1|1<<WGM11;
	TCCR1B = 1<<WGM13|1<<WGM12|1<<CS10;
	ICR1 = 150;
	OCR1A = 75;
	MotorEn(1);
	pwm->en = 1;
	pwm->first_time = 1;
}

void PWMDisable(pwm_t *pwm){
	TCCR1B &= ~(1<<CS10);
	pwm->en = 0;
}

void PWMEnable(pwm_t *pwm){
	TCCR1B |= 1<<CS10;
	pwm->en = 1;
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
	if(housekp.cnt == 1 && housekp.en){//reading ace values
		adc_rot.adc_val_new = ADCRead();
		if(abs(adc_rot.adc_val_new - adc_rot.adc_val_old) > 2){
			//if(adc_rot.spi_changed == 0)
			adc_rot.adc_changed = 1;
			adc_rot.adc_val_old = adc_rot.adc_val_new;
			}
	}
	
	else if(housekp.cnt >= 10 && housekp.en){//setting leds
		MovePot(&adc_rot, &pwm, (adc_rot.pot_pos << 2));
		housekp.cnt = 0;
	}
	enableTimer();
	
	housekp.cnt++;
	housekp.tick++;
}