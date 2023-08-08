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
#include "ADC.h"
#include "SPI.h"


#define MOTORISED_POTT 2

#define MAX_TIMEOUT 500

enum setting_states_e{PARSE_READY, PARSING_INSTR, IS_BYTE, IS_BUFFER, READING_LENGTH, READING_START,
	 READING_START_VAL, READING_VAL, READING_END, PARSE_END};

int parseSPI(spi_t *spi, adc_t *adc, buffer_t *buffer, hskp_t *hskp);
void errorHandler(void);

enum setting_states_e parsing_state;


#define soft_reset()        \
do                          \
{                           \
	wdt_enable(WDTO_15MS);  \
	for(;;)                 \
	{                       \
	}                       \
} while(0)

#endif /* SETTINGS_H_ */