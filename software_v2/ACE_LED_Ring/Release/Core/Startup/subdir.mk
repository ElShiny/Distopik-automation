################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32c031g6ux.s 

OBJS += \
./Core/Startup/startup_stm32c031g6ux.o 

S_DEPS += \
./Core/Startup/startup_stm32c031g6ux.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -c -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/include" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/functions" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/rtu" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/port" -I"C:/Users/Matej/Desktop/Distopik-automation/software_v2/ACE_LED_Ring/Core/modbus/ascii" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32c031g6ux.d ./Core/Startup/startup_stm32c031g6ux.o

.PHONY: clean-Core-2f-Startup

