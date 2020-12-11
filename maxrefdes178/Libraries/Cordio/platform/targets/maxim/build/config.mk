###################################################################################################
#
# Build make targets
#
# Copyright (c) 2019-2020 Packetcraft, Inc.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###################################################################################################

#--------------------------------------------------------------------------------------------------
#     Project
#--------------------------------------------------------------------------------------------------

# Chip
ifeq ($(RISCV_CORE),)
CPU             := cortex-m4
else
CPU             := riscv
endif
MFLOAT          := softfp
MFPU            := fpv4-sp-d16
CHIP            := max32655
COMPILER        := GCC

CHIP_UC         := $(shell echo $(CHIP) | tr a-z A-Z)
CHIP_LC         := $(shell echo $(CHIP) | tr A-Z a-z)
TARGET          := $(CHIP_UC)
TARGET_NUM      := $(shell echo $(TARGET) | tr -dc '0-9')
TARGET_REV      := 0x4131

# Board
BOARD           := EvKit_V1
BSP_DIR         := $(ROOT_DIR)/..
LIBS_DIR        := $(BSP_DIR)
CMSIS_ROOT      := $(LIBS_DIR)/CMSIS

# Options
BAUD            := 1000000
HWFC            := 0

#--------------------------------------------------------------------------------------------------
#     Configuration
#--------------------------------------------------------------------------------------------------

# Peripherals
CFG_DEV         += BB_CLK_RATE_HZ=1000000
CFG_DEV         += LCTR_CONN_NO_TIFS_REASSEMBLY=1
CFG_DEV         += $(CHCI_TR)
CFG_DEV         += UART_BAUD=$(BAUD)
CFG_DEV         += UART_HWFC=$(HWFC)
CFG_DEV         += BB_ENABLE_INLINE_ENC_TX=1
CFG_DEV         += BB_ENABLE_INLINE_DEC_RX=1
CFG_DEV         += HCI_UART=2
CFG_DEV         += TERMINAL_UART=0
CFG_DEV         += CONSOLE_UART=0 # TODO: Deconflict this definition
CFG_DEV         += TARGET=$(TARGET_NUM)
CFG_DEV         += TARGET_REV=$(TARGET_REV)
CFG_DEV         += PAL_MAX_RTC_COUNTER_VAL=0xFFFFFFFF
CFG_DEV         += LL_WW_RX_DEVIATION_USEC=0
ifneq ($(RISCV_CORE),)
CFG_DEV         += uECC_PLATFORM=uECC_arch_other
endif

# Board
CFG_DEV         += BOARD=$(BOARD)

#--------------------------------------------------------------------------------------------------
#     Sources
#--------------------------------------------------------------------------------------------------

# Linker file
ifeq ($(LD_FILE),)
ifeq ($(RISCV_CORE),)
LD_FILE         := $(BSP_DIR)/CMSIS/Device/Maxim/$(CHIP_UC)/Source/$(COMPILER)/$(CHIP).ld
else
LD_FILE         := $(BSP_DIR)/CMSIS/Device/Maxim/$(CHIP_UC)/Source/$(COMPILER)/$(CHIP)_riscv.ld
endif
endif

#--------------------------------------------------------------------------------------------------
#     Compilation flags
#--------------------------------------------------------------------------------------------------

ifeq ($(RISCV_CORE),)
# ARM flags
# Compiler flags
C_FLAGS         += -mcpu=$(CPU) -mthumb -mlittle-endian
C_FLAGS         += -mfloat-abi=$(MFLOAT) -mfpu=$(MFPU) 

A_FLAGS         += -mcpu=$(CPU) -mthumb -mlittle-endian
A_FLAGS         += -mfloat-abi=$(MFLOAT) -mfpu=$(MFPU) -MD                                                                

# Linker flags
LD_FLAGS        += -mthumb -mcpu=$(CPU)
LD_FLAGS        += -mfloat-abi=$(MFLOAT) -mfpu=$(MFPU) --entry=Reset_Handler

else
#RISCV flags
C_FLAGS 	+= -march=rv32imc -mabi=ilp32 -c
LD_FLAGS 	+= -march=rv32imafdc
endif
