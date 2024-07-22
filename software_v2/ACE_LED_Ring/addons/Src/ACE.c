/*
 * ACE.c
 *
 *  Created on: Jul 15, 2024
 *      Author: Matej
 */

#include "ACE.h"
#include "main.h"

void ACEInit(ace_t *ace){
	LL_GPIO_SetOutputPin(ACE_EN_GPIO_Port, ACE_EN_Pin);


	ace->ace_val_old = readACEQuick();
	ace->ace_max = 127;
	ace->ace_led_changed = 1;

}

/*uint8_t readACEValRaw(void){
	uint8_t raw_val = 0;
	PORTD &= ~(1<<ACE_EN);

	raw_val = isBitSet(ACE_P8_PORT , ACE_P8)<<7|isBitSet(ACE_P7_PORT , ACE_P7)<<6|isBitSet(ACE_P6_PORT , ACE_P6)<<5|
	isBitSet(ACE_P5_PORT , ACE_P5)<<4|isBitSet(ACE_P4_PORT , ACE_P4)<<3|isBitSet(ACE_P3_PORT , ACE_P3)<<2|
	isBitSet(ACE_P2_PORT , ACE_P2)<<1|isBitSet(ACE_P1_PORT, ACE_P1);

	PORTD |= 1<<ACE_EN;

	return raw_val;
}*/

//uint8_t ACETransform(uint8_t val){
//	return encoderMap[val];
//}

uint8_t readACEQuick(void){
	LL_GPIO_ResetOutputPin(ACE_EN_GPIO_Port, ACE_EN_Pin);

	asm("NOP\n\t"
		"NOP\n\t"
		"NOP\n\t");

	uint8_t val = encoderMap[ !LL_GPIO_IsInputPinSet(ACE8_GPIO_Port, ACE8_Pin) << 7
							 |!LL_GPIO_IsInputPinSet(ACE7_GPIO_Port, ACE7_Pin) << 6
							 |!LL_GPIO_IsInputPinSet(ACE6_GPIO_Port, ACE6_Pin) << 5
							 |!LL_GPIO_IsInputPinSet(ACE5_GPIO_Port, ACE5_Pin) << 4
							 |!LL_GPIO_IsInputPinSet(ACE4_GPIO_Port, ACE4_Pin) << 3
							 |!LL_GPIO_IsInputPinSet(ACE3_GPIO_Port, ACE3_Pin) << 2
							 |!LL_GPIO_IsInputPinSet(ACE2_GPIO_Port, ACE2_Pin) << 1
							 |!LL_GPIO_IsInputPinSet(ACE1_GPIO_Port, ACE1_Pin)
							 ];

	LL_GPIO_SetOutputPin(ACE_EN_GPIO_Port, ACE_EN_Pin);
	return val;
}

void absoluteToRelative(ace_t *ace){

	int delta = 0;

	if(ace->ace_val_old > 117 && ace->ace_val_new < 10) delta = ace->ace_val_new- ace->ace_val_old + 128;		//corrections for zero crossing
	else if(ace->ace_val_old < 15 && ace->ace_val_new > 110) delta = ace->ace_val_new - ace->ace_val_old - 128;
	else delta = ace->ace_val_new - ace->ace_val_old;

	ace->ace_val = ace->ace_val + delta;

	ace->ace_val_old = ace->ace_val_new;

	if (ace->ace_val > ace->ace_max)ace->ace_val = ace->ace_max;
	if (ace->ace_val < 0)ace->ace_val = 0;

	if(delta){
		ace->ace_changed = 1;
		ace->ace_led_changed = 1;
	}
}
