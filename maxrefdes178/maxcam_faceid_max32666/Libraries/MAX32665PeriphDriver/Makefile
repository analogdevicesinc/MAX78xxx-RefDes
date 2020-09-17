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
 # $Date: 2019-12-24 20:42:21 +0300 (Tue, 24 Dec 2019) $
 # $Revision: 50314 $
 #
 ###############################################################################

# This is the name of the build output file
PROJECT=PeriphDriver

ifeq "$(TARGET)" ""
$(error TARGET must be specified)
endif

ifeq "$(COMPILER)" ""
$(error COMPILER must be specified)
endif

ifeq "$(BUILD_DIR)" ""
BUILD_DIR=./Build
endif

# This is the path to the CMSIS root directory
ifeq "$(CMSIS_ROOT)" ""
CMSIS_ROOT=../CMSIS
endif

# This is used to enable support for Core 1
# Enabling the core will reserve space in RAM for its stack
ifneq "$(DUAL_CORE)" ""
SRCS += startup_core1.S
SRCS += system_core1.c
endif

# Source files for this test (add path to VPATH below)
SRCS += adc.c
SRCS += cipher.c
SRCS += hash.c
SRCS += crc.c
SRCS += dma.c
SRCS += dvs.c
SRCS += flc.c
SRCS += gpio.c
SRCS += htmr.c
SRCS += i2c.c
SRCS += icc.c
SRCS += lp.c
SRCS += maa.c
SRCS += mem_utils.c
SRCS += mxc_assert.c
SRCS += mxc_delay.c
SRCS += mxc_lock.c
SRCS += mxc_pins.c
SRCS += mxc_sys.c
SRCS += nvic_table.c
SRCS += owm.c
SRCS += pt.c
SRCS += rpu.c
SRCS += rtc.c
SRCS += sdhc.c
SRCS += sema.c
SRCS += simo.c
SRCS += spi17y.c
SRCS += spi.c
SRCS += spixr.c
SRCS += spixf.c
SRCS += spixfc.c
SRCS += srcc.c
SRCS += tmr.c
SRCS += tmr_utils.c
SRCS += trng.c
SRCS += uart.c
SRCS += wdt.c
SRCS += wut.c
# Where to find source files for this project
VPATH  = Source

# Where to find header files for this project
IPATH  = Include

# Include the rules for building for this target
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk

# Build this as a library
.DEFAULT_GOAL := lib
