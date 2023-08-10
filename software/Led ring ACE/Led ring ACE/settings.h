/*
 * settings.h
 *
 * Created: 14/04/2023 15:27:45
 *  Author: Matej
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdio.h>
#include <avr/wdt.h>
#include "ACE.h"
#include "SPI.h"
#include "led_drv.h"

#define ACE_LED_RING 1

#define MAX_TIMEOUT 500

enum setting_states_e{WAIT_BLOCK, WAIT_INSTR, FINISHED_INSTR, STATE_ERROR};

int recieveStateMachine(spi_t *spi, ace_t *ace, led_drv_t *led_set, buffer_t *buffer, hskp_t *hskp);
int parseData(spi_t *spi, ace_t *ace, led_drv_t *led_set, buffer_t *buffer, hskp_t *hskp, uint8_t cmd);
uint16_t uint4TOuint16(uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3);
int32_t assembleU16(buffer_t *buffer, hskp_t *hskp, int tick );
void errorHandler(void);

enum setting_states_e current_state;




#define soft_reset()        \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;)                 \
	{                       \
	}                       \
} while(0)

#endif /* SETTINGS_H_ */