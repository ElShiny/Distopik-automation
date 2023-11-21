################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/modbus/rtu/mbcrc.c \
../Core/modbus/rtu/mbrtu.c 

OBJS += \
./Core/modbus/rtu/mbcrc.o \
./Core/modbus/rtu/mbrtu.o 

C_DEPS += \
./Core/modbus/rtu/mbcrc.d \
./Core/modbus/rtu/mbrtu.d 


# Each subdirectory must supply rules for building sources it contributes
Core/modbus/rtu/%.o Core/modbus/rtu/%.su Core/modbus/rtu/%.cyclo: ../Core/modbus/rtu/%.c Core/modbus/rtu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/include" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/functions" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/rtu" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/port" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/ascii" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-modbus-2f-rtu

clean-Core-2f-modbus-2f-rtu:
	-$(RM) ./Core/modbus/rtu/mbcrc.cyclo ./Core/modbus/rtu/mbcrc.d ./Core/modbus/rtu/mbcrc.o ./Core/modbus/rtu/mbcrc.su ./Core/modbus/rtu/mbrtu.cyclo ./Core/modbus/rtu/mbrtu.d ./Core/modbus/rtu/mbrtu.o ./Core/modbus/rtu/mbrtu.su

.PHONY: clean-Core-2f-modbus-2f-rtu

