################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/modbus/functions/mbfunccoils.c \
../addons/modbus/functions/mbfuncdiag.c \
../addons/modbus/functions/mbfuncdisc.c \
../addons/modbus/functions/mbfuncholding.c \
../addons/modbus/functions/mbfuncinput.c \
../addons/modbus/functions/mbfuncother.c \
../addons/modbus/functions/mbutils.c 

OBJS += \
./addons/modbus/functions/mbfunccoils.o \
./addons/modbus/functions/mbfuncdiag.o \
./addons/modbus/functions/mbfuncdisc.o \
./addons/modbus/functions/mbfuncholding.o \
./addons/modbus/functions/mbfuncinput.o \
./addons/modbus/functions/mbfuncother.o \
./addons/modbus/functions/mbutils.o 

C_DEPS += \
./addons/modbus/functions/mbfunccoils.d \
./addons/modbus/functions/mbfuncdiag.d \
./addons/modbus/functions/mbfuncdisc.d \
./addons/modbus/functions/mbfuncholding.d \
./addons/modbus/functions/mbfuncinput.d \
./addons/modbus/functions/mbfuncother.d \
./addons/modbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
addons/modbus/functions/%.o addons/modbus/functions/%.su addons/modbus/functions/%.cyclo: ../addons/modbus/functions/%.c addons/modbus/functions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/ascii" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/functions" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/include" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/port" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/rtu" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/tcp" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/Inc" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/Src" -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-addons-2f-modbus-2f-functions

clean-addons-2f-modbus-2f-functions:
	-$(RM) ./addons/modbus/functions/mbfunccoils.cyclo ./addons/modbus/functions/mbfunccoils.d ./addons/modbus/functions/mbfunccoils.o ./addons/modbus/functions/mbfunccoils.su ./addons/modbus/functions/mbfuncdiag.cyclo ./addons/modbus/functions/mbfuncdiag.d ./addons/modbus/functions/mbfuncdiag.o ./addons/modbus/functions/mbfuncdiag.su ./addons/modbus/functions/mbfuncdisc.cyclo ./addons/modbus/functions/mbfuncdisc.d ./addons/modbus/functions/mbfuncdisc.o ./addons/modbus/functions/mbfuncdisc.su ./addons/modbus/functions/mbfuncholding.cyclo ./addons/modbus/functions/mbfuncholding.d ./addons/modbus/functions/mbfuncholding.o ./addons/modbus/functions/mbfuncholding.su ./addons/modbus/functions/mbfuncinput.cyclo ./addons/modbus/functions/mbfuncinput.d ./addons/modbus/functions/mbfuncinput.o ./addons/modbus/functions/mbfuncinput.su ./addons/modbus/functions/mbfuncother.cyclo ./addons/modbus/functions/mbfuncother.d ./addons/modbus/functions/mbfuncother.o ./addons/modbus/functions/mbfuncother.su ./addons/modbus/functions/mbutils.cyclo ./addons/modbus/functions/mbutils.d ./addons/modbus/functions/mbutils.o ./addons/modbus/functions/mbutils.su

.PHONY: clean-addons-2f-modbus-2f-functions

