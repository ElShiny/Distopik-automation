################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/modbus/tcp/mbtcp.c 

OBJS += \
./addons/modbus/tcp/mbtcp.o 

C_DEPS += \
./addons/modbus/tcp/mbtcp.d 


# Each subdirectory must supply rules for building sources it contributes
addons/modbus/tcp/%.o addons/modbus/tcp/%.su addons/modbus/tcp/%.cyclo: ../addons/modbus/tcp/%.c addons/modbus/tcp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/ascii" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/functions" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/include" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/port" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/rtu" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/modbus/tcp" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/Inc" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/UNIREL_SW/addons/Src" -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-addons-2f-modbus-2f-tcp

clean-addons-2f-modbus-2f-tcp:
	-$(RM) ./addons/modbus/tcp/mbtcp.cyclo ./addons/modbus/tcp/mbtcp.d ./addons/modbus/tcp/mbtcp.o ./addons/modbus/tcp/mbtcp.su

.PHONY: clean-addons-2f-modbus-2f-tcp

