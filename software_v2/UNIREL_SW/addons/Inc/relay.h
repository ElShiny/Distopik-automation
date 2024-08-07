/*
 * relay.h
 *
 *  Created on: Jul 30, 2024
 *      Author: Matej
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

#include "stdint.h"


#define MCP_ADR(X) ((0x04<<4) | (X<<1))// 0-5
#define I2C_WRITE 0
#define I2C_READ 1

#define MCP_IODIRA 0x0
#define MCP_IODIRB 0x1
#define MCP_GPIOA 0x12
#define MCP_GPIOB 0x13
#define MCP_OLATA 0x14
#define MCP_OLATB 0x15





uint8_t relay_check_expanders(void);
void relay_mcp_init(uint8_t mcp_found);

#endif /* INC_RELAY_H_ */
