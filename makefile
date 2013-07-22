#
#       !!!! Do NOT edit this makefile with an editor which replace tabs by spaces !!!!    
#
##############################################################################################
# 
# On command line:
#
# make all = Create project
#
# make clean = Clean project files.
#
# To rebuild project do "make clean" and "make all".
#

##############################################################################################
# Start of default section
#

TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
BIN  = $(CP) -O ihex 

MCU  = cortex-m4

# List all default C defines here, like -D_DEBUG=1
DDEFS =

# List all default ASM defines here, like -D_DEBUG=1
DADEFS = 

# List all default directories to look for include files here
DINCDIR = 

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS = 

#
# End of default section
##############################################################################################

##############################################################################################
# Start of user section
#

# 
# Define project name and Ram/Flash mode here
PROJECT        = ThunderCryer
RUN_FROM_FLASH = 1
USE_HARD_FPU   = 1
HEAP_SIZE      = 64256
STACK_SIZE     = 1024

#
# Define linker script file here
#
ifeq ($(RUN_FROM_FLASH), 0)
LDSCRIPT = ./stm32f4xx_ram.ld
FULL_PRJ = $(PROJECT)_ram
else
LDSCRIPT = ./stm32f4xxxg_flash.ld
FULL_PRJ = $(PROJECT)_rom
endif

#
# Define FPU settings here
#
ifeq ($(USE_HARD_FPU), 0)
FPU =
else
FPU = -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__FPU_USED=1
endif
#
# Define cpp options here
#
CPPOPT = -fno-rtti -fno-exceptions

# List all user C define here, like -D_DEBUG=1
UDEFS = -DUSE_USB_OTG_FS

# Define ASM defines here
UADEFS = 

# List C source files here

##############################################################################################
# Start of C Code Section
#

            
FREERTOS_SRC = ./FreeRTOS/Source/croutine.c \
               ./FreeRTOS/Source/list.c \
               ./FreeRTOS/Source/queue.c \
               ./FreeRTOS/Source/tasks.c \
               ./FreeRTOS/Source/timers.c \
               ./FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c


              
HELIX_SRC =       ./HelixMP3/mp3dec.c \
                  ./HelixMP3/mp3tabs.c \
                  ./HelixMP3/real/bitstream.c \
                  ./HelixMP3/real/buffers.c \
                  ./HelixMP3/real/dct32.c \
                  ./HelixMP3/real/dequant.c \
                  ./HelixMP3/real/dqchan.c \
                  ./HelixMP3/real/huffman.c \
                  ./HelixMP3/real/hufftabs.c \
                  ./HelixMP3/real/imdct.c \
                  ./HelixMP3/real/polyphase.c \
                  ./HelixMP3/real/scalfact.c \
                  ./HelixMP3/real/stproc.c \
                  ./HelixMP3/real/subband.c \
                  ./HelixMP3/real/trigtabs.c

USB_HOST_SRC =    ./STM32_USB_HOST_Library/Class/MSC/src/usbh_msc_core.c \
                  ./STM32_USB_HOST_Library/Class/MSC/src/usbh_msc_fatfs.c \
                  ./STM32_USB_HOST_Library/Core/src/usbh_core.c \
                  ./STM32_USB_HOST_Library/Core/src/usbh_hcs.c \
                  ./STM32_USB_HOST_Library/Core/src/usbh_ioreq.c \
                  ./STM32_USB_HOST_Library/Core/src/usbh_stdreq.c \
                  ./STM32_USB_HOST_Library/FatFs/src/ff.c \
                  ./STM32_USB_HOST_Library/FatFs/src/option/syscall.c \
                  ./STM32_USB_HOST_Library/USB_MSC/fattime.c \
                  ./STM32_USB_HOST_Library/USB_MSC/usb_bsp.c \
                  ./STM32_USB_HOST_Library/USB_MSC/usbh_usr.c \
                  ./STM32_USB_OTG_Driver/src/usb_core.c \
                  ./STM32_USB_OTG_Driver/src/usb_hcd.c \
                  ./STM32_USB_OTG_Driver/src/usb_hcd_int.c \
                  ./STM32F4-Discovery/stm32f4_discovery_audio_codec.c \
                  ./STM32F4-Discovery/stm32f4_discovery_lis302dl.c \
                  ./STM32F4-Discovery/stm32f4_discovery.c

                  
STM32DRV_SRC =    ./STM32F4xx_StdPeriph_Driver/src/misc.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c \
                  ./STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c
#
# End of C Code Section
##############################################################################################

##############################################################################################
# Start of Cpp Code section
#
                  
USB_CPP_SRC =     ./STM32_USB_HOST_Library/USB_MSC/CFileHandler.cpp \
                  ./STM32_USB_HOST_Library/USB_MSC/CUSBMassStorage.cpp

MENUE_SRC =       ./Menue/CMenue.cpp

FREERTOS_EC_SRC = ./FreeRTOS_EC/Source/AManagedTask.cpp \
                  ./FreeRTOS_EC/Source/ASyncObject.cpp \
                  ./FreeRTOS_EC/Source/ATimer.cpp \
                  ./FreeRTOS_EC/Source/ABinarySemaphore.cpp \
                  ./FreeRTOS_EC/Source/CCountingSemaphore.cpp \
                  ./FreeRTOS_EC/Source/CFreeRTOS.cpp \
                  ./FreeRTOS_EC/Source/CMessageTask.cpp \
                  ./FreeRTOS_EC/Source/CMutex.cpp \
                  ./FreeRTOS_EC/Source/CQueue.cpp \
                  ./FreeRTOS_EC/Source/CRecursiveMutex.cpp \
                  ./FreeRTOS_EC/Source/CTask.cpp 
                  
GRAPHIC_LCD_SRC = ./GraphicLCD/CGraphicLCD.cpp \
                  ./GraphicLCD/CT6963CDriver.cpp \
                  ./GraphicLCD/CT6963GPIOInterface.cpp \
                  ./GraphicLCD/SansSerif12.cpp \
                  ./GraphicLCD/SansSerif26.cpp
                  
GUI_SRC =         ./GUI/CGUI.cpp \
                  ./GUI/CGUIActor.cpp \
                  ./GUI/CGUIActorList.cpp \
                  ./GUI/CGUIAlarmSubPage.cpp \
                  ./GUI/CGUIButton.cpp \
                  ./GUI/CGUINumberSwitch.cpp \
                  ./GUI/CGUIPage.cpp 
                  
CLOCK_SRC = ./Clock/CRTCHandler.cpp \
            ./Clock/CRTCTime.cpp \
            ./Clock/CTime.cpp
#
# End of Cpp Code Section
##############################################################################################

CC_SRC       = $(STM32DRV_SRC) $(USB_HOST_SRC) $(HELIX_SRC) $(FREERTOS_SRC)
CPPC_SRC     = $(USB_CPP_SRC) $(GRAPHIC_LCD_SRC) $(CLOCK_SRC)
#$(GUI_SRC)


# List ASM source files here
ASRC = ./startup_stm32f4xx.s

# List all user directories here
UINCDIR = ./ ./Clock ./CMSIS/Device ./CMSIS/Include ./FreeRTOS/Source/Include \
          ./FreeRTOS/Source/portable/GCC/ARM_CM4F \
          ./FreeRTOS_EC/Source ./GraphicLCD ./GUI ./HelixMP3/pub \
          ./HelixMP3/real ./Menue ./STM32_USB_HOST_Library/Class/MSC/inc \
          ./STM32_USB_HOST_Library/Core/inc ./STM32_USB_HOST_Library/FatFs/src \
          ./STM32_USB_HOST_Library/USB_MSC ./STM32_USB_OTG_Driver/inc \
          ./STM32F4-Discovery ./STM32F4xx_StdPeriph_Driver/inc

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS = 

# Define optimisation level here
OPT = -O0 
#OPT = -O2 -falign-functions=16 -fno-inline -fomit-frame-pointer

#
# End of user defines
##############################################################################################


INCDIR  = $(patsubst %,-I%,$(DINCDIR) $(UINCDIR))
LIBDIR  = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

ifeq ($(RUN_FROM_FLASH), 0)
DEFS    = $(DDEFS) $(UDEFS) -DRUN_FROM_FLASH=0 -DVECT_TAB_SRAM
else
DEFS    = $(DDEFS) $(UDEFS) -DRUN_FROM_FLASH=1
endif

ADEFS   = $(DADEFS) $(UADEFS) -D__HEAP_SIZE=$(HEAP_SIZE) -D__STACK_SIZE=$(STACK_SIZE)
OBJS    = $(ASRC:.s=.o) $(CC_SRC:.c=.o) $(CPPC_SRC:.cpp=.o)
LIBS    = $(DLIBS) $(ULIBS)
MCFLAGS = -mthumb -mcpu=$(MCU) $(FPU)

ASFLAGS  = $(MCFLAGS) $(OPT) -g -gdwarf-2 -Wa,-amhls=$(<:.s=.lst) $(ADEFS)

CPFLAGS  = $(MCFLAGS) $(OPT) -gdwarf-2 -Wall -Wstrict-prototypes -fverbose-asm 
CPFLAGS += -ffunction-sections -fdata-sections -Wa,-ahlms=$(<:.c=.lst) $(DEFS)

CPPFLAGS = $(MCFLAGS) $(OPT) -gdwarf-2 -Wall -fverbose-asm 
CPPFLAGS += $(CPPOPT) -ffunction-sections -fdata-sections $(DEFS)

LDFLAGS  = $(MCFLAGS) -nostartfiles -T$(LDSCRIPT) -Wl,-Map=$(FULL_PRJ).map,--cref,--gc-sections,--no-warn-mismatch $(LIBDIR)

# Generate dependency information
CPFLAGS += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

#
# makefile rules
#

all: $(OBJS) $(FULL_PRJ).elf $(FULL_PRJ).hex


%.o : %.c
	@echo Compiling $(<F)
	@$(CC) -c $(CPFLAGS) -I . $(INCDIR) $< -o $@
	
%.o : %.cpp
	@echo Compiling $(<F)
	@$(CPPC) -c $(CPPFLAGS) -I. $(INCDIR) $< -o $@

%.o : %.s
	$(AS) -c $(ASFLAGS) $< -o $@

%elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
  
%hex: %elf
	$(BIN) $< $@

clean:
	-rm -f $(OBJS)
	-rm -f $(FULL_PRJ).elf
	-rm -f $(FULL_PRJ).map
	-rm -f $(FULL_PRJ).hex
	-rm -f $(SRC:.c=.c.bak)
	-rm -f $(SRC:.c=.lst)
	-rm -f $(ASRC:.s=.s.bak)
	-rm -f $(ASRC:.s=.lst)
	-rm -fR .dep

# 
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***