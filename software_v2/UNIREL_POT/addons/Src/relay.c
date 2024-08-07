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
volatile relay_drv_t hrel1;

uint8_t relay_check_expanders(volatile relay_drv_t *relay){
	uint8_t arr[] = {0};
	uint8_t return_val = 0;

	for(int i = 0; i<6; i++){
		int ret = HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), arr, 1, 100);
		if(ret == 0x0){
			return_val |= 1 << i;
		}
	}

	relay->mcps_connected = return_val;
	return return_val;
}

void relay_mcp_init(volatile relay_drv_t *relay){
	uint8_t iodir_arr[] = {MCP_IODIRA, 0x0, 0x0};
	uint8_t port_arr[] = {MCP_GPIOA, 0x00, 0x00};

	for(int i = 0; i<6; i++){
		if(relay->mcps_connected & (1<<i)){
			HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), iodir_arr, 3, 100);
			HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(i), port_arr, 3, 100);
			HAL_Delay(100);
		}
	}
}

uint8_t swap_bits(uint8_t val){
	uint8_t rotVal = 0;

    for (int i = 0; i < 8; i++){
        rotVal |= ((val & (1<<i)) >> i) << (7-i);
    }
    return rotVal;
}


void relay_return_values(volatile relay_drv_t *relay, USHORT *input_buffer){
	for(int i = 0; i<10; i++) input_buffer[RELAY_RETURN_VALUES + i] = relay->line_value[i];
	input_buffer[MCPS_CONNECTED] = relay->mcps_connected;
}

void relay_settings_parser(volatile relay_drv_t *relay){

	if(relay->values_changed == 0) return;

	uint8_t tmp_mcps = relay->mcps_connected & relay->mcps_enabled;

	for(int i = 0; i < 10; i++){
		if((relay->lines_enabled & (1<<i)) && (tmp_mcps & (1<<(i/2))) ){
			relay_write_line(i, (relay->line_value[i] & relay->line_relay_mask[i]));
		}
	}

	relay->values_changed = 0;

}

void relay_write_line(uint8_t line, uint8_t value){
	int mcp_addr = 0;
	int reg_addr = 0;
	int temp_val = 0;

	switch (line) {
		case 0:
			mcp_addr = 0x0; reg_addr = MCP_GPIOA;
			temp_val = swap_bits(value);
			break;
		case 1:
			mcp_addr = 0x0; reg_addr = MCP_GPIOB;
			temp_val = swap_bits(value);
			break;
		case 2:
			mcp_addr = 0x1; reg_addr = MCP_GPIOA;
			temp_val = swap_bits(value);
			break;
		case 3:
			mcp_addr = 0x1; reg_addr = MCP_GPIOB;
			temp_val = swap_bits(value);
			break;
		case 4:
			mcp_addr = 0x2; reg_addr = MCP_GPIOA;
			temp_val = swap_bits(value);
			break;
		case 5:
			mcp_addr = 0x2; reg_addr = MCP_GPIOB;
			temp_val = value;
			break;
		case 6:
			mcp_addr = 0x3; reg_addr = MCP_GPIOB;
			temp_val = value;
			break;
		case 7:
			mcp_addr = 0x3; reg_addr = MCP_GPIOA;
			temp_val = value;
			break;
		case 8:
			mcp_addr = 0x4; reg_addr = MCP_GPIOB;
			temp_val = value;
			break;
		case 9:
			mcp_addr = 0x4; reg_addr = MCP_GPIOA;
			temp_val = value;
			break;
		default:
			return;
			break;
	}
	uint8_t out_arr[2] = {reg_addr, temp_val};

	HAL_I2C_Master_Transmit(&hi2c1, MCP_ADR(mcp_addr), out_arr, 2, 3);

}



uint8_t compare_arrays(uint8_t *a, USHORT *b, uint8_t len) {

  for(int i = 0; i < len; i++) {
    if (a[i] != b[i]) return 0;
  }
  return 1;
}



