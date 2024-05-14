/*
 * modbuc.c
 *
 *  Created on: Apr 12, 2024
 *      Author: Matej
 */


#include "modbus.h"


#define LIGHTMODBUS_IMPL
#include <lightmodbus/lightmodbus.h>
#include <assert.h>


//create a slave instance

ModbusErrorInfo errorInfo;
ModbusSlave slave;

void modbusInit(){

	errorInfo = modbusSlaveInit(
			&slave,
			registerCallback,
			exceptionCallback,
			modbusDefaultAllocator,
			modbusSlaveDeafultFunctions,
			modbusSlaveDefaultFunctionsCount);

	if (modbusIsOk(errorInfo)) {
		Error_Handler();
	}



}



ModbusError registerCallback(
	const ModbusSlave *slave,
	const ModbusRegisterCallbackArgs *args,
	ModbusRegisterCallbackResult *result)
{
	return MODBUS_OK;
}

ModbusError exceptionCallback(const ModbusSlave *slave,  uint8_t function, ModbusExceptionCode code)
{
	//printf("Slave exception %s (function %d)\n", modbusExceptionCodeStr(code), function);
	return MODBUS_OK;
}

//modbus receive timer init
void modbusTimerInit() {

	//start timer
	HAL_TIM_Base_Start_IT(&htim14);

}

//modbus uart interrupt transmit
void modbusUartTransmitIT() {



}




