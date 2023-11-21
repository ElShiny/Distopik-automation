################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/modbus/mb.c 

OBJS += \
./Core/modbus/mb.o 

C_DEPS += \
./Core/modbus/mb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/modbus/%.o Core/modbus/%.su Core/modbus/%.cyclo: ../Core/modbus/%.c Core/modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/include" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/functions" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/rtu" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/port" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/ascii" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-modbus

clean-Core-2f-modbus:
	-$(RM) ./Core/modbus/mb.cyclo ./Core/modbus/mb.d ./Core/modbus/mb.o ./Core/modbus/mb.su

.PHONY: clean-Core-2f-modbus

