################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/modbus/mb.c 

OBJS += \
./addons/modbus/mb.o 

C_DEPS += \
./addons/modbus/mb.d 


# Each subdirectory must supply rules for building sources it contributes
addons/modbus/%.o addons/modbus/%.su addons/modbus/%.cyclo: ../addons/modbus/%.c addons/modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/ascii" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/functions" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/include" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/port" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/rtu" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/tcp" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/Inc" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/Src" -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-addons-2f-modbus

clean-addons-2f-modbus:
	-$(RM) ./addons/modbus/mb.cyclo ./addons/modbus/mb.d ./addons/modbus/mb.o ./addons/modbus/mb.su

.PHONY: clean-addons-2f-modbus

