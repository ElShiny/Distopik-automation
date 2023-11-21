################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/modbus/port/port.c \
../Core/modbus/port/portevent.c \
../Core/modbus/port/portserial.c \
../Core/modbus/port/porttimer.c 

OBJS += \
./Core/modbus/port/port.o \
./Core/modbus/port/portevent.o \
./Core/modbus/port/portserial.o \
./Core/modbus/port/porttimer.o 

C_DEPS += \
./Core/modbus/port/port.d \
./Core/modbus/port/portevent.d \
./Core/modbus/port/portserial.d \
./Core/modbus/port/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/modbus/port/%.o Core/modbus/port/%.su Core/modbus/port/%.cyclo: ../Core/modbus/port/%.c Core/modbus/port/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/include" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/functions" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/rtu" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/port" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/ascii" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-modbus-2f-port

clean-Core-2f-modbus-2f-port:
	-$(RM) ./Core/modbus/port/port.cyclo ./Core/modbus/port/port.d ./Core/modbus/port/port.o ./Core/modbus/port/port.su ./Core/modbus/port/portevent.cyclo ./Core/modbus/port/portevent.d ./Core/modbus/port/portevent.o ./Core/modbus/port/portevent.su ./Core/modbus/port/portserial.cyclo ./Core/modbus/port/portserial.d ./Core/modbus/port/portserial.o ./Core/modbus/port/portserial.su ./Core/modbus/port/porttimer.cyclo ./Core/modbus/port/porttimer.d ./Core/modbus/port/porttimer.o ./Core/modbus/port/porttimer.su

.PHONY: clean-Core-2f-modbus-2f-port

