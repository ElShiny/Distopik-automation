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


void ACEInit(ace_t *ace){
	
	DDRB &= ~(0x03); //set ace pins on portx to input
	DDRC &= ~(0x87);
	DDRD = 0x08; //set EN to output
	
	PORTD |= 1<<ACE_EN;
		
	ace->ace_val_old = readACEQuick();
	
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

void absoluteToRelative(ace_t *ace){
	
	int delta = 0;
	
	if(ace->ace_val_old > 117 && ace->ace_val_new < 10) delta = ace->ace_val_new- ace->ace_val_old + 128;		//corrections for zero crossing
	else if(ace->ace_val_old < 15 && ace->ace_val_new > 110) delta = ace->ace_val_new - ace->ace_val_old - 128;
	else delta = ace->ace_val_new - ace->ace_val_old;
	
	ace->ace_val = ace->ace_val + delta;	
	
	ace->ace_val_old = ace->ace_val_new;
	
	if(delta) ace->ace_changed = 1;
}