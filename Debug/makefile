################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include STM32F4xx_StdPeriph_Driver/src/subdir.mk
-include STM32F4-Discovery/subdir.mk
-include Menue/subdir.mk
-include GraphicLCD/subdir.mk
-include FreeRTOS_EC/Source/subdir.mk
-include FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include FreeRTOS/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
ThunderCryer.hex \

SECONDARY_LIST += \
ThunderCryer.lst \

SECONDARY_SIZE += \
ThunderCryer.siz \


# All Target
all: ThunderCryer.elf secondary-outputs

# Tool invocations
ThunderCryer.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Yagarto Windows GCC C++ Linker'
	arm-none-eabi-g++ -T"stm32_flash.ld" -Xlinker --gc-sections -L"C:\Dokumente und Einstellungen\brand\Desktop\thundercyer-the-alarm-clock" -Wl,-Map,ThunderCryer.map -mcpu=cortex-m4 -mthumb -g3 -gdwarf-2 -o "ThunderCryer.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

ThunderCryer.hex: ThunderCryer.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex ThunderCryer.elf  "ThunderCryer.hex"
	@echo 'Finished building: $@'
	@echo ' '

ThunderCryer.lst: ThunderCryer.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S ThunderCryer.elf > "ThunderCryer.lst"
	@echo 'Finished building: $@'
	@echo ' '

ThunderCryer.siz: ThunderCryer.elf
	@echo 'Invoking: ARM Yagarto Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley ThunderCryer.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ASM_DEPS)$(SECONDARY_SIZE)$(S_UPPER_DEPS)$(C++_DEPS)$(EXECUTABLES)$(CC_DEPS)$(CPP_DEPS)$(SECONDARY_LIST)$(CXX_DEPS)$(OBJS)$(C_DEPS)$(SECONDARY_FLASH)$(C_UPPER_DEPS) ThunderCryer.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
