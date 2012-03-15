################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/croutine.c \
../FreeRTOS/Source/list.c \
../FreeRTOS/Source/queue.c \
../FreeRTOS/Source/tasks.c \
../FreeRTOS/Source/timers.c 

OBJS += \
./FreeRTOS/Source/croutine.o \
./FreeRTOS/Source/list.o \
./FreeRTOS/Source/queue.o \
./FreeRTOS/Source/tasks.o \
./FreeRTOS/Source/timers.o 

C_DEPS += \
./FreeRTOS/Source/croutine.d \
./FreeRTOS/Source/list.d \
./FreeRTOS/Source/queue.d \
./FreeRTOS/Source/tasks.d \
./FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/%.o: ../FreeRTOS/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -DHELIX_CONFIG_FIXEDPOINT -DMEDIA_USB_KEY -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -DUSE_USB_OTG_FS -DUSE_USB_OTG_HS -DUSE_EMBEDDED_PHY -DUSE_DEFAULT_TIMEOUT_CALLBACK -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\CMSIS\Include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\GraphicLCD" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Menue" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\HelixMP3\pub" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\HelixMP3\real" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32_USB_HOST_Library\Class\MSC\inc" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32_USB_HOST_Library\Core\inc" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32_USB_OTG_Driver\inc" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Audio_MP3" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Audio_MP3\FatFs\src" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Audio_MP3\USB_MSC" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\FreeRTOS\Source\portable\GCC\ARM_CM4F" -O0 -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


