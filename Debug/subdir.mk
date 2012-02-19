################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CLedHeartBeatSTM32F4Disc.cpp \
../main.cpp 

C_SRCS += \
../stm32f4xx_it.c \
../syscalls.c \
../system_stm32f4xx.c \
../ustime.c 

S_UPPER_SRCS += \
../startup_stm32f4xx.S 

OBJS += \
./CLedHeartBeatSTM32F4Disc.o \
./main.o \
./startup_stm32f4xx.o \
./stm32f4xx_it.o \
./syscalls.o \
./system_stm32f4xx.o \
./ustime.o 

C_DEPS += \
./stm32f4xx_it.d \
./syscalls.d \
./system_stm32f4xx.d \
./ustime.d 

S_UPPER_DEPS += \
./startup_stm32f4xx.d 

CPP_DEPS += \
./CLedHeartBeatSTM32F4Disc.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C++ Compiler'
	arm-none-eabi-g++ -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\GraphicLCD" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\Menue" -O0 -ffunction-sections -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu++0x -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\GraphicLCD" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\Menue" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\CMSIS\Include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\GraphicLCD" -I"C:\Users\Alex\git\thundercyer-the-alarm-clock\Menue" -O0 -ffunction-sections -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


