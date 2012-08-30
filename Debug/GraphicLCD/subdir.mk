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
	arm-none-eabi-g++ -DHELIX_CONFIG_FIXEDPOINT -DMEDIA_USB_KEY -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DUSE_DEFAULT_TIMEOUT_CALLBACK -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\CMSIS\Include" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\GraphicLCD" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\Menue" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\HelixMP3\pub" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\HelixMP3\real" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\STM32_USB_HOST_Library\Class\MSC\inc" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\STM32_USB_HOST_Library\Core\inc" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\STM32_USB_OTG_Driver\inc" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\Audio_MP3" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\Audio_MP3\FatFs\src" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\Audio_MP3\USB_MSC" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\FreeRTOS\Source\portable\GCC\ARM_CM4F" -I"D:\Elektronik\Projekte\thundercyer-the-alarm-clock\Clock" -O0 -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu++0x -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


