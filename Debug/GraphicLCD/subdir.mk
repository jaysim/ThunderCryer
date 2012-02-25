################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GraphicLCD/CGraphicLCD.cpp \
../GraphicLCD/CT6963CDriver.cpp \
../GraphicLCD/CT6963GPIOInterface.cpp \
../GraphicLCD/SansSerif12.cpp \
../GraphicLCD/SansSerif26.cpp 

OBJS += \
./GraphicLCD/CGraphicLCD.o \
./GraphicLCD/CT6963CDriver.o \
./GraphicLCD/CT6963GPIOInterface.o \
./GraphicLCD/SansSerif12.o \
./GraphicLCD/SansSerif26.o 

CPP_DEPS += \
./GraphicLCD/CGraphicLCD.d \
./GraphicLCD/CT6963CDriver.d \
./GraphicLCD/CT6963GPIOInterface.d \
./GraphicLCD/SansSerif12.d \
./GraphicLCD/SansSerif26.d 


# Each subdirectory must supply rules for building sources it contributes
GraphicLCD/%.o: ../GraphicLCD/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C++ Compiler'
	arm-none-eabi-g++ -DHELIX_CONFIG_FIXEDPOINT -I"E:\Desktop\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"E:\Desktop\thundercyer-the-alarm-clock" -I"E:\Desktop\thundercyer-the-alarm-clock\CMSIS\Include" -I"E:\Desktop\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"E:\Desktop\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"E:\Desktop\thundercyer-the-alarm-clock\GraphicLCD" -I"E:\Desktop\thundercyer-the-alarm-clock\Menue" -I"E:\Desktop\thundercyer-the-alarm-clock\Fatfs" -I"E:\Desktop\thundercyer-the-alarm-clock\Fatfs\FatFs\src" -I"E:\Desktop\thundercyer-the-alarm-clock\HelixMP3\pub" -I"E:\Desktop\thundercyer-the-alarm-clock\HelixMP3\real" -O0 -ffunction-sections -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu++0x -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


