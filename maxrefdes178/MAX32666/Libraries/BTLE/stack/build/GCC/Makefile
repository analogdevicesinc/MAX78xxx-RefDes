################################################################################
# Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
# $Id: Makefile 51261 2020-01-29 22:06:24Z nathan.goldstick $
#
################################################################################

# This is the name of the build output file
PROJECT_NAME=libstack

# Specify the project variant.
ifeq "$(MFLOAT_FLAGS)" "hardfp"
PROJECT_VARIANT=hardfp
else
ifeq "$(MFLOAT_FLAGS)" "hard"
PROJECT_VARIANT=hardfp
else
PROJECT_VARIANT=softfp
endif
endif

# Use these to specify the project.
ifeq "$(PROJECT_VARIANT)" ""
PROJECT=$(PROJECT_NAME)
else
PROJECT=$(PROJECT_NAME)_$(PROJECT_VARIANT)
endif

# Specify the target processor
ifeq "$(TARGET)" ""
TARGET=MAX32665
endif

# Select 'GCC' or 'IAR' compiler
COMPILER=GCC

# Specify the board used
ifeq "$(BOARD)" ""
BOARD=EvKit_V1
endif

# Create Target name variables
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

# Specify variant specific CFLAGS
ifeq "$(MFLOAT_FLAGS)" "hardfp"
PROJECT_CFLAGS+=-mfloat-abi=hardfp
else
ifeq "$(MFLOAT_FLAGS)" "hard"
PROJECT_CFLAGS+=-mfloat-abi=hardfp
else
PROJECT_CFLAGS+=-mfloat-abi=softfp
endif
endif

ifeq "$(BUILD_DIR)" ""
BUILD_DIR=./library
endif

# Strip this library during link stage.
STRIP_LIBRARIES?=
ifneq "$(ECLIPSE)" "1"
export FIXDBGPATHS_MATCH='^\.\./\.\./'
export FIXDBGPATHS_REPLACE=
endif

# This is the path to the CMSIS root directory
ifeq "$(MAXIM_PATH)" ""
LIBS_DIR=../../../../../Libraries
NDALIBS_DIR=../../../../../NDALibraries
else
LIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/Libraries)
NDALIBS_DIR=/$(subst \,/,$(subst :,,$(MAXIM_PATH))/Firmware/$(TARGET_UC)/NDALibraries)
endif
CMSIS_ROOT=$(LIBS_DIR)/CMSIS

# Include Cordio BTLE Library
ifeq "$(CORDIO_DIR)" ""
CORDIO_DIR=$(LIBS_DIR)/BTLE
endif

# Include the Peripheral Driver Library
PERIPH_DRIVER_DEFS_ONLY=1
PERIPH_DRIVER_DIR=$(LIBS_DIR)/$(TARGET)PeriphDriver
include ${PERIPH_DRIVER_DIR}/periphdriver.mk

# Include BTLE definitions and headers
BTLE_DEFS_ONLY=1
include $(CORDIO_DIR)/btle.mk

# Include sources
include $(CORDIO_DIR)/stack/build/sources_stack.mk

# Map variable names
SRCS  += $(STACK_C_FILES)
VPATH += $(dir $(SRCS))
IPATH += $(STACK_INC_DIRS)

# Use absolute paths if building within eclipse environment.
ifeq "$(ECLIPSE)" "1"
SRCS := $(abspath $(SRCS))
endif

# Only building libraries.
MAKECMDGOALS=lib

# Include the rules for building for this target
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk
