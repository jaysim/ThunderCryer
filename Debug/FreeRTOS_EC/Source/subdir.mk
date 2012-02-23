################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FreeRTOS_EC/Source/AManagedTask.cpp \
../FreeRTOS_EC/Source/ASyncObject.cpp \
../FreeRTOS_EC/Source/ATimer.cpp \
../FreeRTOS_EC/Source/CBinarySemaphore.cpp \
../FreeRTOS_EC/Source/CCountingSemaphore.cpp \
../FreeRTOS_EC/Source/CFreeRTOS.cpp \
../FreeRTOS_EC/Source/CMessageTask.cpp \
../FreeRTOS_EC/Source/CMutex.cpp \
../FreeRTOS_EC/Source/CQueue.cpp \
../FreeRTOS_EC/Source/CRecursiveMutex.cpp \
../FreeRTOS_EC/Source/CTask.cpp 

CPP_DEPS += \
./FreeRTOS_EC/Source/AManagedTask.d \
./FreeRTOS_EC/Source/ASyncObject.d \
./FreeRTOS_EC/Source/ATimer.d \
./FreeRTOS_EC/Source/CBinarySemaphore.d \
./FreeRTOS_EC/Source/CCountingSemaphore.d \
./FreeRTOS_EC/Source/CFreeRTOS.d \
./FreeRTOS_EC/Source/CMessageTask.d \
./FreeRTOS_EC/Source/CMutex.d \
./FreeRTOS_EC/Source/CQueue.d \
./FreeRTOS_EC/Source/CRecursiveMutex.d \
./FreeRTOS_EC/Source/CTask.d 

OBJS += \
./FreeRTOS_EC/Source/AManagedTask.o \
./FreeRTOS_EC/Source/ASyncObject.o \
./FreeRTOS_EC/Source/ATimer.o \
./FreeRTOS_EC/Source/CBinarySemaphore.o \
./FreeRTOS_EC/Source/CCountingSemaphore.o \
./FreeRTOS_EC/Source/CFreeRTOS.o \
./FreeRTOS_EC/Source/CMessageTask.o \
./FreeRTOS_EC/Source/CMutex.o \
./FreeRTOS_EC/Source/CQueue.o \
./FreeRTOS_EC/Source/CRecursiveMutex.o \
./FreeRTOS_EC/Source/CTask.o 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_EC/Source/%.o: ../FreeRTOS_EC/Source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C++ Compiler'
	arm-none-eabi-g++ -DHELIX_CONFIG_FIXEDPOINT -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\CMSIS\Device\STM32F4xx\Include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\CMSIS\Include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\FreeRTOS\Source\include" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32F4-Discovery" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\STM32F4xx_StdPeriph_Driver\inc" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\FreeRTOS_EC\Source" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\GraphicLCD" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Menue" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Fatfs" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\Fatfs\FatFs\src" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\HelixMP3\pub" -I"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock\HelixMP3\real" -O0 -ffunction-sections -fdata-sections -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -std=gnu++0x -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


