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
# $Id: sources_wsf.mk 53529 2020-05-16 06:01:32Z nathan.goldstick $
#
################################################################################


ifeq "$(CORDIO_DIR)" ""
$(error "CORDIO_DIR must be specified")
endif

ifneq "$(COMPILER)" "RCS"
ifeq "$(MFLOAT_FLAGS)" "hardfp"
WSF_LIB_VARIANT=hardfp
else
ifeq "$(MFLOAT_FLAGS)" "hard"
WSF_LIB_VARIANT=hardfp
else
WSF_LIB_VARIANT=softfp
endif
endif
endif

WSF_DIR             := ${CORDIO_DIR}/wsf
WSF_BUILD_DIR       := ${WSF_DIR}/build
WSF_LIB_DIR         := ${WSF_BUILD_DIR}/${COMPILER}/library
ifeq "$(WSF_LIB_VARIANT)" ""
WSF_LIB             := ${WSF_LIB_DIR}/libwsf.a
else
WSF_LIB             := ${WSF_LIB_DIR}/libwsf_$(WSF_LIB_VARIANT).a
endif

WSF_INC_DIRS = \
	$(CORDIO_DIR) \
	$(WSF_DIR)/include \
	$(WSF_DIR)/include/util \
	$(WSF_DIR)/sources \
	$(WSF_DIR)/sources/util \
	$(WSF_DIR)/sources/port \
	$(WSF_DIR)/sources/port/baremetal 

WSF_H_FILES += $(shell find $(WSF_INC_DIRS) -name '*.h')

WSF_C_FILES = \
	$(WSF_DIR)/sources/util/crc32.c \
	$(WSF_DIR)/sources/util/print.c \
	$(WSF_DIR)/sources/util/bda.c \
	$(WSF_DIR)/sources/util/bstream.c \
	$(WSF_DIR)/sources/util/fcs.c \
	$(WSF_DIR)/sources/util/terminal.c \
	$(WSF_DIR)/sources/util/wstr.c \
	$(WSF_DIR)/sources/util/calc128.c \
	$(WSF_DIR)/sources/util/prand.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_assert.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_detoken.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_msg.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_trace.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_efs.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_timer.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_heap.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_buf.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_bufio.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_nvm.c \
	$(WSF_DIR)/sources/port/baremetal/wsf_queue.c 
