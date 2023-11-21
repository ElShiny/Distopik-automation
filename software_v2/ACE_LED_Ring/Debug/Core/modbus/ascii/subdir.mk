################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/modbus/ascii/mbascii.c 

OBJS += \
./Core/modbus/ascii/mbascii.o 

C_DEPS += \
./Core/modbus/ascii/mbascii.d 


# Each subdirectory must supply rules for building sources it contributes
Core/modbus/ascii/%.o Core/modbus/ascii/%.su Core/modbus/ascii/%.cyclo: ../Core/modbus/ascii/%.c Core/modbus/ascii/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/include" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/functions" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/rtu" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/port" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/ascii" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-modbus-2f-ascii

clean-Core-2f-modbus-2f-ascii:
	-$(RM) ./Core/modbus/ascii/mbascii.cyclo ./Core/modbus/ascii/mbascii.d ./Core/modbus/ascii/mbascii.o ./Core/modbus/ascii/mbascii.su

.PHONY: clean-Core-2f-modbus-2f-ascii

