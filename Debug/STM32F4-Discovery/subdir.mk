################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32F4-Discovery/stm32f4_discovery.c \
../STM32F4-Discovery/stm32f4_discovery_audio_codec.c \
../STM32F4-Discovery/stm32f4_discovery_lis302dl.c 

OBJS += \
./STM32F4-Discovery/stm32f4_discovery.o \
./STM32F4-Discovery/stm32f4_discovery_audio_codec.o \
./STM32F4-Discovery/stm32f4_discovery_lis302dl.o 

C_DEPS += \
./STM32F4-Discovery/stm32f4_discovery.d \
./STM32F4-Discovery/stm32f4_discovery_audio_codec.d \
./STM32F4-Discovery/stm32f4_discovery_lis302dl.d 


# Each subdirectory must supply rules for building sources it contributes
STM32F4-Discovery/%.o: ../STM32F4-Discovery/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\CMSIS\Include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\GraphicLCD" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Menue" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Fatfs" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Fatfs\FatFs\src" -O0 -ffunction-sections -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


