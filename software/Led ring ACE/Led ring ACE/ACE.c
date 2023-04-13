/*
 * ACE.c
 *
 * Created: 03/04/2023 15:28:37
 *  Author: ElShiny
 */ 

#include "ACE.h"
#include "led_drv.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int ace_val = 0;
uint8_t ace_val_new = 0;
uint8_t ace_val_old = 0;
int cnt = 0;
uint8_t ace_changed = 0;


void ACEInit(void){
	
	DDRB &= ~(0x03); //set ace pins on portx to input
	DDRC &= ~(0x87);
	DDRD = 0x08; //set EN to output
	
	PORTD |= 1<<ACE_EN; 
	
	TCCR0A = 1<<CTC0|1<<CS02|1<<CS00; //timer enable
	OCR0A = 78;			//78 is 10 ms delay
	TIMSK0 = 1<<OCIE0A; //timer interrupt
	TCNT0 = 0;			//empty timer counter
	
	ace_val_old = readACEQuick();
	
}

void enableTimer(void){
	TIMSK0 |= 1<<OCIE0A;
}

void disableTimer(void){
	TIMSK0 &= ~(1<<OCIE0A);
}


uint8_t readACEValRaw(void){
	uint8_t raw_val = 0;
	PORTD &= ~(1<<ACE_EN);
	
	asm("nop\n\t"
		"nop\n\t"
		"nop\n\t");
	raw_val = isBitSet(ACE_P8_PORT , ACE_P8)<<7|isBitSet(ACE_P7_PORT , ACE_P7)<<6|isBitSet(ACE_P6_PORT , ACE_P6)<<5|
	isBitSet(ACE_P5_PORT , ACE_P5)<<4|isBitSet(ACE_P4_PORT , ACE_P4)<<3|isBitSet(ACE_P3_PORT , ACE_P3)<<2|
	isBitSet(ACE_P2_PORT , ACE_P2)<<1|isBitSet(ACE_P1_PORT, ACE_P1);
	
	PORTD |= 1<<ACE_EN;
	
	return raw_val;
}

uint8_t ACETransform(uint8_t val){
	return encoderMap[val];
}

uint8_t readACEQuick(void){
	PORTD &= ~(1<<ACE_EN);
	asm("nop\n\t"
		"nop\n\t"
		"nop\n\t");
	uint8_t val = encoderMap[isBitSet(ACE_P8_PORT , ACE_P8)<<7|isBitSet(ACE_P7_PORT , ACE_P7)<<6|isBitSet(ACE_P6_PORT , ACE_P6)<<5|
	isBitSet(ACE_P5_PORT , ACE_P5)<<4|isBitSet(ACE_P4_PORT , ACE_P4)<<3|isBitSet(ACE_P3_PORT , ACE_P3)<<2|
	isBitSet(ACE_P2_PORT , ACE_P2)<<1|isBitSet(ACE_P1_PORT, ACE_P1)];
	
	PORTD |= 1<<ACE_EN;
	return val;
}

uint8_t isBitSet(uint8_t reg, uint8_t pin){
	if (reg & pin) return 0;
	else return 1;
}

void absoluteToRelative(uint8_t *old_val, uint8_t *new_val, int *save){
	
	int delta = 0;
	
	if(*old_val > 117 && *new_val < 10) delta = *new_val - *old_val + 128;		//corrections for zero crossing
	else if(*old_val < 15 && *new_val > 110) delta = *new_val - *old_val - 128;
	else delta = *new_val - *old_val;
	
	*save = *save + delta;	
	
	*old_val = *new_val;
	
	if(delta) ace_changed = 1;
}


ISR(TIMER0_COMPA_vect){
	
	if(cnt == 0){//reading ace values
		ace_val_new = readACEQuick();
		absoluteToRelative(&ace_val_old, &ace_val_new, &ace_val);
		cnt ++;
	}
	
	else if(cnt == 1){//setting leds
		int led = ((ace_val)*15)>>6;
		int bck_clr;
		
		for(int i = 0; i<30; i++){
			if(i<=led && led<= 30){setLED(i, 0b110000); bck_clr = 0;}
			else if(i<=led-30 && 30<=led && led<=60){setLED(i, 0b001100);bck_clr = 0b110000;}
			else if(i<=led-60 && 60<=led && led<=90){setLED(i, 0b000011);bck_clr = 0b001100;}
			else setLED(i, bck_clr);
		}
		cnt = 0;
	}


}