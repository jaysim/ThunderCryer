################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F4-Discovery/stm32f4_discovery.c 

OBJS += \
./STM32F4-Discovery/stm32f4_discovery.o 

C_DEPS += \
./STM32F4-Discovery/stm32f4_discovery.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F4-Discovery/%.o: ../STM32F4-Discovery/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\CMSIS\Device\STM32F4xx\Include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\CMSIS\Include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\FreeRTOS\Source\include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\STM32F4-Discovery" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\STM32F4xx_StdPeriph_Driver\inc" -Os -ffunction-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

