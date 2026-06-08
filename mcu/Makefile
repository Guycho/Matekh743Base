##########################################################################################################################
# Makefile for Matek H743-Mini — GCC bare-metal build
##########################################################################################################################

#####################################
# target
#####################################
TARGET = matek743mini

#####################################
# building variables
#####################################
DEBUG = 1
OPT   = -Og
MCU_DEBUG_LOG ?= 0

######################################
# paths
######################################
BUILD_DIR = build

#####################################
# source
#####################################
# C sources
C_SOURCES = \
Core/Src/main.c \
Core/Src/freertos.c \
Core/Src/stm32h7xx_it.c \
Core/Src/stm32h7xx_hal_msp.c \
Core/Src/stm32h7xx_hal_timebase_tim.c \
Core/Src/system_stm32h7xx.c \
Core/Src/syscalls.c \
Core/Src/sysmem.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/Common/mpu_wrappers_v2.c \
Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
Middlewares/Third_Party/FreeRTOS/Source/list.c \
Middlewares/Third_Party/FreeRTOS/Source/queue.c \
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
Middlewares/Third_Party/FreeRTOS/Source/timers.c \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
USB_DEVICE/App/usb_device.c \
USB_DEVICE/App/usbd_desc.c \
USB_DEVICE/App/usbd_cdc_if.c \
USB_DEVICE/Target/usbd_conf.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pcd_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c

# C++ sources
CXX_SOURCES = \
../common/Src/App.cpp \
../common/Src/led_task.cpp \
../common/Src/sensor_task.cpp \
../common/hal/src/AdcDmaReader.cpp \
../common/hal/src/DigitalInput.cpp \
../common/hal/src/DigitalOutput.cpp \
../common/hal/src/I2cDevice.cpp \
../common/hal/src/Led.cpp \
../common/lib/src/Dps310.cpp \
../common/lib/src/Hmc5883l.cpp \
../common/lib/src/Icm20602.cpp \
../common/lib/src/Icm426xx.cpp \
../common/lib/src/Lis3mdl.cpp \
../common/lib/src/Mpu6000.cpp \
../common/lib/src/Pid.cpp \
../common/lib/src/Qmc5883l.cpp \
../common/hal/src/PwmOutput.cpp \
../common/hal/src/SerialPort.cpp \
../common/hal/src/SpiDevice.cpp

# ASM sources (Adjust based on the actual startup file in mcu/Core/Startup)
# Since I don't see the exact startup file path in list_files for mcu/Core, 
# I'll use a wildcard or assume the standard H743 one if it exists.
# Based on reference, it's usually in Drivers/CMSIS/...
ASM_SOURCES = \
$(wildcard Drivers/CMSIS/Device/ST/STM32H7xx/Source/Templates/gcc/startup_stm32h743xx.s)

######################################
# binaries
######################################
PREFIX = arm-none-eabi-
CC  = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS  = $(PREFIX)gcc -x assembler-with-cpp
CP  = $(PREFIX)objcopy
SZ  = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

######################################
# CFLAGS
######################################
CPU       = -mcpu=cortex-m7
FPU       = -mfpu=fpv5-d16
FLOAT-ABI = -mfloat-abi=softfp
MCU       = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

C_DEFS = \
-DUSE_HAL_DRIVER \
-DSTM32H743xx \
-DUSE_PWR_LDO_SUPPLY

ifeq ($(MCU_DEBUG_LOG),1)
C_DEFS += -DMCU_DEBUG_LOG
endif

C_INCLUDES = \
-ICore/Inc \
-IDrivers/STM32H7xx_HAL_Driver/Inc \
-IDrivers/STM32H7xx_HAL_Driver/Inc/Legacy \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-IDrivers/CMSIS/Device/ST/STM32H7xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Core/Include \
-IDrivers/CMSIS/RTOS2/Include \
-I../common/Inc \
-I../common/hal/inc \
-I../common/lib/inc \
-I../common/config \
-IUSB_DEVICE/App \
-IUSB_DEVICE/Target \
-IMiddlewares/ST/STM32_USB_Device_Library/Core/Inc \
-IMiddlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc

AS_INCLUDES = $(C_INCLUDES)

ASFLAGS  = $(MCU) $(C_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS  += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CXXFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -fno-exceptions -fno-rtti

ifeq ($(DEBUG), 1)
CFLAGS   += -g -gdwarf-2
CXXFLAGS += -g -gdwarf-2
endif

CFLAGS   += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

######################################
# LDFLAGS
######################################
# Note: Using a generic H7 flash ld if a specific one isn't found in mcu/
LDSCRIPT = STM32H743XX_FLASH.ld
LIBS     = -lc -lm -lnosys
LDFLAGS  = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) \
            -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

######################################
# build the application
######################################
OBJECTS  = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	-rm -fR $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)
