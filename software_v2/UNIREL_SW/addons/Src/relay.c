/*
 * relay.c
 *
 *  Created on: Jul 30, 2024
 *      Author: Matej
 */

#include "relay.h"
#include "i2c.h"
#include "mb.h"
#include "port.h"
#include "mbport.h"
#include "settings.h"

extern USHORT   usRegInputBuf[REG_INPUT_NREGS];

uint8_t relay_check_expanders(void){
	uint8_t arr[] = {0};
	uint8_t return_val = 0;

	for(int i = 0; i<6; i++){
		int ret = HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), arr, 1, 100);
		if(ret == 0x0){
			return_val |= 1 << i;
		}
	}

	return return_val;
}

void relay_mcp_init(uint8_t mcp_found){

	uint8_t iodir_arr[] = {MCP_IODIRA, 0x0, 0x0};
	uint8_t port_arr[] = {MCP_GPIOA, 0x0, 0x0};

	uint8_t arr1[] = {MCP_GPIOA, 0xff, 0xff};

	for(int i = 0; i<2; i++){
		if(mcp_found & (1<<i)){
			HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), iodir_arr, 3, 100);
			HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), port_arr, 3, 100);
			HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), arr1, 3, 100);
		}
	}

	for(int i = 0; i<2; i++){
		if(mcp_found & (1<<i)){
			HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), arr1, 3, 100);
		}
		HAL_Delay(500);
	}

	//uint8_t arr1[] = {MCP_GPIOA, 0xff, 0xff};


}

