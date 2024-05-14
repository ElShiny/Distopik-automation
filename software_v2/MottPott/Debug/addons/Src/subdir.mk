################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../addons/Src/nanomodbus.c 

OBJS += \
./addons/Src/nanomodbus.o 

C_DEPS += \
./addons/Src/nanomodbus.d 


# Each subdirectory must supply rules for building sources it contributes
addons/Src/%.o addons/Src/%.su addons/Src/%.cyclo: ../addons/Src/%.c addons/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32C031xx -c -I../Core/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc -I../Drivers/STM32C0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32C0xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/MottPott/addons" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/MottPott/addons/Inc" -I"C:/Users/Matej/Documents/GitHub/Distopik-automation/software_v2/MottPott/addons/Src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-addons-2f-Src

clean-addons-2f-Src:
	-$(RM) ./addons/Src/nanomodbus.cyclo ./addons/Src/nanomodbus.d ./addons/Src/nanomodbus.o ./addons/Src/nanomodbus.su

.PHONY: clean-addons-2f-Src

