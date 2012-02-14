################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stm32f4xx_it.c \
../syscalls.c \
../system_stm32f4xx.c \
../ustime.c 

CPP_SRCS += \
../main.cpp 

CPP_DEPS += \
./main.d 

OBJS += \
./main.o \
./stm32f4xx_it.o \
./syscalls.o \
./system_stm32f4xx.o \
./ustime.o 

C_DEPS += \
./stm32f4xx_it.d \
./syscalls.d \
./system_stm32f4xx.d \
./ustime.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C++ Compiler'
	arm-none-eabi-g++ -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\CMSIS\Device\STM32F4xx\Include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\CMSIS\Include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\FreeRTOS\Source\include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\STM32F4-Discovery" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\STM32F4xx_StdPeriph_Driver\inc" -Os -ffunction-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu++0x -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\CMSIS\Device\STM32F4xx\Include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\CMSIS\Include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\FreeRTOS\Source\include" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\STM32F4-Discovery" -I"D:\Elektronik\WorspaceEclipse\ThunderCryer\STM32F4xx_StdPeriph_Driver\inc" -Os -ffunction-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


