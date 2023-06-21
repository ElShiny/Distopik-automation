/*
 * housekeeping.h
 *
 * Created: 17/04/2023 23:07:05
 *  Author: Matej
 */ 


#ifndef HOUSEKEEPING_H_
#define HOUSEKEEPING_H_

#include <stdint.h>




typedef struct{
	volatile uint32_t tick;
	volatile uint8_t en;
	volatile uint8_t cnt;
	}hskp_t;

hskp_t housekp;




void timersInit(hskp_t *hskp);
void enableTimer(void);
void disableTimer(void);
void enableHSKP(hskp_t *hskp);
void disableHSKP(hskp_t *hskp);
uint16_t getTick(hskp_t *hskp);




#endif /* HOUSEKEEPING_H_ */