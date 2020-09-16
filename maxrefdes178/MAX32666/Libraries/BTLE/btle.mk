################################################################################
# Copyright (C) 2014 Maxim Integrated Products, Inc., All Rights Reserved.
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
# $Id: btle.mk 55337 2020-09-02 19:54:13Z nathan.goldstick $
#
################################################################################

################################################################################
# This file can be included in a project makefile to build the library for the 
# project.
################################################################################

ifeq "$(TARGET)" ""
$(error "TARGET must be specified")
endif
TARGET_UC:=$(shell echo $(TARGET) | tr a-z A-Z)
TARGET_LC:=$(shell echo $(TARGET) | tr A-Z a-z)

# Specify the library variant.
ifneq "$(COMPILER)" "RCS"
ifeq "$(MFLOAT_FLAGS)" "hardfp"
LIBRARY_VARIANT=hardfp
else
ifeq "$(MFLOAT_FLAGS)" "hard"
LIBRARY_VARIANT=hardfp
else
LIBRARY_VARIANT=softfp
endif
endif
endif

PHY_DIR             := ${CORDIO_DIR}/phy
PHY_BUILD_DIR       := ${PHY_DIR}/build
PHY_LIB_DIR         := ${PHY_BUILD_DIR}/${COMPILER}/library
ifeq "$(LIBRARY_VARIANT)" ""
PHY_LIB             := ${PHY_LIB_DIR}/libphy.a
else
PHY_LIB             := ${PHY_LIB_DIR}/libphy_$(LIBRARY_VARIANT).a
endif

LINKLAYER_DIR       := ${CORDIO_DIR}/link_layer
LINKLAYER_BUILD_DIR := ${LINKLAYER_DIR}/build
LINKLAYER_LIB_DIR   := ${LINKLAYER_BUILD_DIR}/${COMPILER}/library
ifeq "$(LIBRARY_VARIANT)" ""
LINKLAYER_LIB       := ${LINKLAYER_LIB_DIR}/liblinklayer.a
else
LINKLAYER_LIB       := ${LINKLAYER_LIB_DIR}/liblinklayer_$(LIBRARY_VARIANT).a
endif

STACK_DIR           := ${CORDIO_DIR}/stack
STACK_BUILD_DIR     := ${STACK_DIR}/build
STACK_LIB_DIR       := ${STACK_BUILD_DIR}/${COMPILER}/library
ifeq "$(LIBRARY_VARIANT)" ""
STACK_LIB           := ${STACK_LIB_DIR}/libstack.a
else
STACK_LIB           := ${STACK_LIB_DIR}/libstack_$(LIBRARY_VARIANT).a
endif

WSF_DIR             := ${CORDIO_DIR}/wsf
WSF_BUILD_DIR       := ${WSF_DIR}/build
WSF_LIB_DIR         := ${WSF_BUILD_DIR}/${COMPILER}/library
ifeq "$(LIBRARY_VARIANT)" ""
WSF_LIB             := ${WSF_LIB_DIR}/libwsf.a
else
WSF_LIB             := ${WSF_LIB_DIR}/libwsf_$(LIBRARY_VARIANT).a
endif

# Add to libraries list
ifeq ($(BTLE_DEFS_ONLY),)
LIBS += $(LINKLAYER_LIB)
LIBS += $(STACK_LIB)
LIBS += $(WSF_LIB)
LIBS += $(PHY_LIB)
endif

# Add Cordio include path
IPATH += ${CORDIO_DIR}

# Add Link Layer include paths
IPATH += ${LINKLAYER_DIR}/platform/common/include
IPATH += ${LINKLAYER_DIR}/controller/include/ble
IPATH += ${LINKLAYER_DIR}/controller/include/common
IPATH += ${LINKLAYER_DIR}/controller/sources/common/bb
IPATH += ${LINKLAYER_DIR}/controller/sources/ble/include

# Add Stack include paths
IPATH += ${STACK_DIR}/platform/$(TARGET_LC)
IPATH += ${STACK_DIR}/platform/include
IPATH += ${STACK_DIR}/ble-host/sources/stack/cfg
IPATH += ${STACK_DIR}/ble-host/sources/stack/dm
IPATH += ${STACK_DIR}/ble-host/include
IPATH += ${STACK_DIR}/ble-host/sources/stack/hci
IPATH += ${STACK_DIR}/ble-host/sources/hci/dual_chip
IPATH += ${STACK_DIR}/ble-profiles/include/app
IPATH += ${STACK_DIR}/ble-profiles/sources/apps
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/app
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/cycling
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/datc
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/dats
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/fit
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/gluc
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/hidapp
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/medc
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/meds
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/sensor
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/tag
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/uribeacon
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/watch
IPATH += ${STACK_DIR}/ble-profiles/sources/apps/wdxs
IPATH += ${STACK_DIR}/ble-profiles/sources/services
IPATH += ${STACK_DIR}/ble-profiles/sources/profiles
IPATH += ${STACK_DIR}/ble-profiles/sources/profiles/include

# Add WSF include paths
IPATH += ${WSF_DIR}/include
IPATH += ${WSF_DIR}/include/util

# Add phy include paths.
IPATH += $(PHY_DIR)/dbb/control/pan
IPATH += $(PHY_DIR)/dbb/prot/ble
IPATH += $(PHY_DIR)/dbb/prot/ble/pan2g5
IPATH += $(PHY_DIR)/dbb/prot/ble/pan2g5/afe
IPATH += $(PHY_DIR)/dbb/prot/ble/pan2g5/afe/max32665
IPATH += $(PHY_DIR)/dbb/prot/ble/pan2g5/afe/max32665/include
IPATH += $(PHY_DIR)/dbb/prot/ble/pan2g5/include
IPATH += $(PHY_DIR)/dbb/prot/shared
IPATH += $(PHY_DIR)/debug
IPATH += $(PHY_DIR)/include
IPATH += $(PHY_DIR)/llc/ble
IPATH += $(PHY_DIR)/llc/shared

# Add sources
BTLE_SRCS := $(notdir $(foreach dir, $(CORDIO_DIR), $(wildcard $(dir)/*.c)))

# Add configurable sources
BTLE_SRCS += hci_drv_sdma.c
BTLE_SRCS += hci_drv.c
BTLE_SRCS += hci_vs.c
BTLE_SRCS += hci_tr.c
BTLE_SRCS += lctr_main_tester.c
BTLE_SRCS += init_ctr.c
BTLE_SRCS += init_ctr_ext.c
BTLE_SRCS += init_ext.c
BTLE_SRCS += init.c

# Add btle source paths.
VPATH     += ${STACK_DIR}/platform/${TARGET_LC}
VPATH     += ${LINKLAYER_DIR}/controller/sources/ble/lctr
VPATH     += ${LINKLAYER_DIR}/controller/sources/ble/init

# Filter sources based on configuration
FILTERED_SRCS :=
ifeq ($(ENABLE_LL_TESTER),)
FILTERED_SRCS+=lctr_main_tester.c
else
PROJ_CFLAGS+=-DLL_ENABLE_TESTER=TRUE
endif

ifeq ($(ENABLE_SDMA),0)
FILTERED_SRCS+=hci_drv_sdma.c
else
ifeq ($(ENABLE_SDMA),)
FILTERED_SRCS+=hci_drv_sdma.c
else
PROJ_CFLAGS+=-DENABLE_SDMA=TRUE
FILTERED_SRCS+=hci_drv.c

# Fixed data memory size for all of the images
PROJ_AFLAGS +=-D__SDMA_DATA_SIZE=0x7BC0

ifeq ($(SDMA_ADV),)
ifeq ($(SDMA_SCN),)
SDMA_CODE_NAME=${STACK_DIR}/platform/$(TARGET_LC)/sdma_code.inc
endif
endif

ifneq ($(SDMA_ADV),)
# Adv/Slave only
PROJ_CFLAGS+=-DSDMA_ADV=TRUE
SDMA_CODE_NAME=${STACK_DIR}/platform/$(TARGET_LC)/sdma_adv_code.inc
endif

ifneq ($(SDMA_SCN),)
# Scan/Master only
PROJ_CFLAGS+=-DSDMA_SCN=TRUE
SDMA_CODE_NAME=${STACK_DIR}/platform/$(TARGET_LC)/sdma_scn_code.inc

endif

FILESIZE=$(shell ls -la $(SDMA_CODE_NAME) | tr -s " " | cut -d " " -f5)

# 81 characters per line, 16 bytes per line, round to the upper 16 byte boundary
CODESIZE=$$(( ($(FILESIZE) / 81 + 1 ) * 16 ))
PROJ_AFLAGS +=-D__SDMA_CODE_SIZE=$(CODESIZE)

endif
endif

ifeq ($(BTLE_DEFS_ONLY),)
SRCS+=$(filter-out $(FILTERED_SRCS),$(BTLE_SRCS))
endif

# Add definitions
PROJ_CFLAGS+=-DLHCI_ENABLE_VS=TRUE
PROJ_CFLAGS+=-DBB_CLK_RATE_HZ=1600000
PROJ_CFLAGS+=-DLCTR_CONN_NO_TIFS_REASSEMBLY=1
PROJ_CFLAGS+=-DBB_ENABLE_INLINE_ENC_TX=1
PROJ_CFLAGS+=-DBB_ENABLE_INLINE_DEC_RX=1
PROJ_CFLAGS+=-DFORCE_PMU_WAKEUP=1
PROJ_CFLAGS+=-DWDX_FTD_ATT_DEFAULT_PAYLOAD_LEN=256

# Software AES will use 4.3kB of data and 1.4kB of code
PROJ_CFLAGS+=-DENABLE_SOFT_AES=1

# Build libraries
export TARGET
export COMPILER
export CMSIS_ROOT
export PROJ_CFLAGS
export IPATH

ifeq ($(BTLE_DEFS_ONLY),)
include ${LINKLAYER_BUILD_DIR}/sources_ll.mk
${LINKLAYER_LIB}: ${LINKLAYER_C_FILES} ${LINKLAYER_H_FILES}
	$(MAKE) -C ${LINKLAYER_BUILD_DIR}/${COMPILER} lib

include ${STACK_BUILD_DIR}/sources_stack.mk
${STACK_LIB}: ${STACK_C_FILES} ${STACK_H_FILES}
	$(MAKE) -C ${STACK_BUILD_DIR}/${COMPILER} lib

include ${WSF_BUILD_DIR}/sources_wsf.mk
${WSF_LIB}: ${WSF_C_FILES} ${WSF_H_FILES}
	$(MAKE) -C ${WSF_BUILD_DIR}/${COMPILER} lib

include ${PHY_BUILD_DIR}/sources_phy.mk
${PHY_LIB}: ${PHY_C_FILES} ${PHY_H_FILES}
	$(MAKE) -C ${PHY_BUILD_DIR}/${COMPILER} lib
	
clean.stack:
	@rm -rf $(STACK_LIB_DIR)/*

clean.linklayer:
	@rm -rf $(LINKLAYER_LIB_DIR)/*

clean.wsf:
	@rm -rf $(WSF_LIB_DIR)/*

clean.phy:
	@rm -rf $(PHY_LIB_DIR)/*

clean.ble: clean.stack clean.linklayer clean.wsf clean.phy

endif
