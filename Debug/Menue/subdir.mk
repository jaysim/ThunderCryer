################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Menue/CMenue.cpp 

OBJS += \
./Menue/CMenue.o 

CPP_DEPS += \
./Menue/CMenue.d 


# Each subdirectory must supply rules for building sources it contributes
Menue/%.o: ../Menue/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C++ Compiler'
	arm-none-eabi-g++ -DHELIX_CONFIG_FIXEDPOINT -DMEDIA_USB_KEY -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -I"E:\Desktop\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"E:\Desktop\thundercyer-the-alarm-clock" -I"E:\Desktop\thundercyer-the-alarm-clock\CMSIS\Include" -I"E:\Desktop\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"E:\Desktop\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"E:\Desktop\thundercyer-the-alarm-clock\GraphicLCD" -I"E:\Desktop\thundercyer-the-alarm-clock\Menue" -I"E:\Desktop\thundercyer-the-alarm-clock\HelixMP3\pub" -I"E:\Desktop\thundercyer-the-alarm-clock\HelixMP3\real" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32_USB_HOST_Library\Class\MSC\inc" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32_USB_HOST_Library\Core\inc" -I"E:\Desktop\thundercyer-the-alarm-clock\STM32_USB_OTG_Driver\inc" -I"E:\Desktop\thundercyer-the-alarm-clock\Audio_MP3" -I"E:\Desktop\thundercyer-the-alarm-clock\Audio_MP3\FatFs\src" -I"E:\Desktop\thundercyer-the-alarm-clock\Audio_MP3\USB_MSC" -O0 -ffunction-sections -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu++0x -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


