################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/modbus/rtu/mbcrc.c \
../addons/modbus/rtu/mbrtu.c 

OBJS += \
./addons/modbus/rtu/mbcrc.o \
./addons/modbus/rtu/mbrtu.o 

C_DEPS += \
./addons/modbus/rtu/mbcrc.d \
./addons/modbus/rtu/mbrtu.d 


# Each subdirectory must supply rules for building sources it contributes
addons/modbus/rtu/%.o addons/modbus/rtu/%.su addons/modbus/rtu/%.cyclo: ../addons/modbus/rtu/%.c addons/modbus/rtu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/ascii" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/functions" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/include" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/port" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/rtu" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/modbus/tcp" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/Inc" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_POT/addons/Src" -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-addons-2f-modbus-2f-rtu

clean-addons-2f-modbus-2f-rtu:
	-$(RM) ./addons/modbus/rtu/mbcrc.cyclo ./addons/modbus/rtu/mbcrc.d ./addons/modbus/rtu/mbcrc.o ./addons/modbus/rtu/mbcrc.su ./addons/modbus/rtu/mbrtu.cyclo ./addons/modbus/rtu/mbrtu.d ./addons/modbus/rtu/mbrtu.o ./addons/modbus/rtu/mbrtu.su

.PHONY: clean-addons-2f-modbus-2f-rtu

