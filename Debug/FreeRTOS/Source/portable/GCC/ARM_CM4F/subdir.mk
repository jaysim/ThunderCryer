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
	arm-none-eabi-gcc -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\GraphicLCD" -O0 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


