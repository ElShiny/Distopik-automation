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

#define MAX_TIMEOUT 5000

typedef enum {PARSE_READY, PARSING_INSTR, IS_BYTE, IS_BUFFER, READING_LENGTH, READING_START,
	 READING_START_VAL, READING_VAL, READING_END, PARSE_END}setting_states_e;
	 
setting_states_e parsing_state;

int parseSPI(spi_t *spi, ace_t *ace, led_drv_t *led_set, buffer_t *buffer, hskp_t *hskp);
void errorHandler(void);




#define soft_reset()        \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;)                 \
	{                       \
	}                       \
} while(0)

#endif /* SETTINGS_H_ */