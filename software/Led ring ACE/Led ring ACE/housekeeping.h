/*
 * housekeeping.h
 *
 * Created: 17/04/2023 23:07:05
 *  Author: Matej
 */ 


#ifndef HOUSEKEEPING_H_
#define HOUSEKEEPING_H_

#include <stdint.h>


volatile uint16_t tick;
extern uint8_t ace_val_new;
extern uint8_t ace_val_old;
extern volatile uint8_t hskp_en;




void timersInit(void);
void enableTimer(void);
void disableTimer(void);
void enableHSKP(void);
void disableHSKP(void);
uint16_t getTick(void);




#endif /* HOUSEKEEPING_H_ */