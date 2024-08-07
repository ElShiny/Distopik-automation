################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/modbus/port/port.c \
../addons/modbus/port/portevent.c \
../addons/modbus/port/portserial.c \
../addons/modbus/port/porttimer.c 

OBJS += \
./addons/modbus/port/port.o \
./addons/modbus/port/portevent.o \
./addons/modbus/port/portserial.o \
./addons/modbus/port/porttimer.o 

C_DEPS += \
./addons/modbus/port/port.d \
./addons/modbus/port/portevent.d \
./addons/modbus/port/portserial.d \
./addons/modbus/port/porttimer.d 


# Each subdirectory must supply rules for building sources it contributes
addons/modbus/port/%.o addons/modbus/port/%.su addons/modbus/port/%.cyclo: ../addons/modbus/port/%.c addons/modbus/port/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/ascii" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/functions" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/include" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/port" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/rtu" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/tcp" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/Inc" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/Src" -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-addons-2f-modbus-2f-port

clean-addons-2f-modbus-2f-port:
	-$(RM) ./addons/modbus/port/port.cyclo ./addons/modbus/port/port.d ./addons/modbus/port/port.o ./addons/modbus/port/port.su ./addons/modbus/port/portevent.cyclo ./addons/modbus/port/portevent.d ./addons/modbus/port/portevent.o ./addons/modbus/port/portevent.su ./addons/modbus/port/portserial.cyclo ./addons/modbus/port/portserial.d ./addons/modbus/port/portserial.o ./addons/modbus/port/portserial.su ./addons/modbus/port/porttimer.cyclo ./addons/modbus/port/porttimer.d ./addons/modbus/port/porttimer.o ./addons/modbus/port/porttimer.su

.PHONY: clean-addons-2f-modbus-2f-port

