################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c 

OBJS += \
./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o 

C_DEPS += \
./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/GCC/ARM_CM4F/%.o: ../FreeRTOS/Source/portable/GCC/ARM_CM4F/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -DHELIX_CONFIG_FIXEDPOINT -I"E:\Desktop\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"E:\Desktop\thundercyer-the-alarm-clock" -I"E:\Desktop\thundercyer-the-alarm-clock\CMSIS\Include" -I"E:\Desktop\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"E:\Desktop\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"E:\Desktop\thundercyer-the-alarm-clock\GraphicLCD" -I"E:\Desktop\thundercyer-the-alarm-clock\Menue" -I"E:\Desktop\thundercyer-the-alarm-clock\Fatfs" -I"E:\Desktop\thundercyer-the-alarm-clock\Fatfs\FatFs\src" -I"E:\Desktop\thundercyer-the-alarm-clock\HelixMP3\pub" -I"E:\Desktop\thundercyer-the-alarm-clock\HelixMP3\real" -O0 -ffunction-sections -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


