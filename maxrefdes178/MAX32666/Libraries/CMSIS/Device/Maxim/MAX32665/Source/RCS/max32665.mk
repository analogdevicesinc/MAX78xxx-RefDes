################################################################################
 # Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 #
 # Permission is hereby granted, free of charge, to any person obtaining a
 # copy of this software and associated documentation files (the "Software"),
 # to deal in the Software without restriction, including without limitation
 # the rights to use, copy, modify, merge, publish, distribute, sublicense,
 # and/or sell copies of the Software, and to permit persons to whom the
 # Software is furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included
 # in all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 # OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 # MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 # IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 # OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 # ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 # OTHER DEALINGS IN THE SOFTWARE.
 #
 # Except as contained in this notice, the name of Maxim Integrated
 # Products, Inc. shall not be used except as stated in the Maxim Integrated
 # Products, Inc. Branding Policy.
 #
 # The mere transfer of this software does not imply any licenses
 # of trade secrets, proprietary technology, copyrights, patents,
 # trademarks, maskwork rights, or any other form of intellectual
 # property whatsoever. Maxim Integrated Products, Inc. retains all
 # ownership rights.
 #
 # $Date: 2020-07-28 01:00:21 +0300 (Tue, 28 Jul 2020) $ 
 # $Revision: 54384 $
 #
 ###############################################################################

ifeq "$(CMSIS_ROOT)" ""
$(error CMSIS_ROOT must be specified)
endif

CMSIS_ROOT := $(abspath $(CMSIS_ROOT))

# The build directory
ifeq "$(BUILD_DIR)" ""
BUILD_DIR=$(CURDIR)/build
endif

ifeq "$(STARTUPFILE)" ""
STARTUPFILE=max32665_startup.asm
endif

ifeq "$(ENTRY)" ""
ENTRY=entry
endif

# SDMA Memory map
# 0x2000000
# 		OTP
# 		Shared memory
#     Data
#
#      ^
#     Stack
# 0x20008000
#     Code
#
# SDMA_LEN

# Limit of the code memory
ifeq "$(SDMA_LEN)" ""
# 0x8000 for the data, shared memory, and OTP sections. Remainder for code
# Upper limmit isn't significant, GCC linker will trim the top section to match
# The length of the code section size
SDMA_LEN    = 0x38000 
endif

OTP_ADDR = 0x20000000

# Place data and code sections in different SRAM instances to decrease access time
DATA_MEM_LIM = 0x20008000

# Calculate the memory addresses for the different sections
OTP_ADDR_DEC=$(shell printf "%d" $(OTP_ADDR))
SDMA_LEN_DEC=$(shell printf "%d" $(SDMA_LEN))
SHARED_ADDR_DEC=$(shell expr $(OTP_ADDR_DEC) \+ 64)
MEMSTART_DEC=$(shell expr $(SHARED_ADDR_DEC) \+ 1024)
MEMEND_DEC=$(shell expr $(MEMSTART_DEC) \+ $(SDMA_LEN_DEC) \- 1)
DATA_MEM_LIM_DEC=$(shell expr $(shell printf "%d" $(DATA_MEM_LIM)) \- 1)

# Convert addresses back to hex
SHARED_ADDR=$(shell printf "0x%x" $(SHARED_ADDR_DEC))
MEMSTART=$(shell printf "0x%x" $(MEMSTART_DEC))
MEMEND=$(shell printf "0x%x" $(MEMEND_DEC))
STACK_START=$(shell printf "0x%x" $(DATA_MEM_LIM))
DATA_MEM_END=$(shell printf "0x%x" $(DATA_MEM_LIM_DEC))

DATA_MEMRANGE=$(MEMSTART)-$(DATA_MEM_END)
CODE_MEMRANGE=$(DATA_MEM_LIM)-$(MEMEND)

MEMRANGE=$(MEMSTART)-$(MEMEND)

export DATA_MEMRANGE
export CODE_MEMRANGE

# Default TARGET_REVISION
# "A1" in ASCII
ifeq "$(TARGET_REV)" ""
TARGET_REV=0x4131
endif

# Add target specific CMSIS source files
ifneq (${MAKECMDGOALS},lib)
SRCS += ${STARTUPFILE}
SRCS += max32665_sdma.c
endif

# Add target specific CMSIS source directories
VPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/RCS

# Add target specific CMSIS include directories
IPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Include
IPATH+=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/RCS

LIBPATH=$(CMSIS_ROOT)/Lib/RCS

# Include the rules and goals for building
include $(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/RCS/rcs.mk
