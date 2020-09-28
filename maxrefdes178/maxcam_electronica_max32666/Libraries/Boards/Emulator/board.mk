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
 # $Date: 2019-05-14 18:44:17 +0300 (Tue, 14 May 2019) $
 # $Revision: 43355 $
 #
 ###############################################################################

ifeq "$(BOARD_DIR)" ""
$(error BOARD_DIR must be set)
endif

PROJ_CFLAGS+=-DEMULATOR=1 -DBSP=\"EMULATOR\"
# NOTE: HIRC_FREQ was once called CRYPTO_FREQ, but HIRC name matches the actual registers.
# NOTE: HIRC_FREQ is divided by 2 in emulator (CLKCN.psc register) and may not be able to be changed.
#       This means the system clock/HCLK will most likely always be 40MHz no matter what you do to the registers.
# NOTE: The APB clock is called PeripheralClock and is managed/calculated in system_max3665.c
PROJ_CFLAGS+=-DHIRC_FREQ=40000000

ifeq "$(LINKERFILE)" ""
LINKERFILE=$(CMSIS_ROOT)/Device/Maxim/MAX32665/Source/GCC/max32665_emulator.ld
endif

# Source files for this test (add path to VPATH below)
SRCS += board.c
SRCS += stdio.c
SRCS += rom_stub.c
SRCS += led.c
SRCS += pb.c
SRCS += mx25.c

# Where to find BSP source files
VPATH += $(BOARD_DIR)/Source
VPATH += $(BOARD_DIR)/../Source

# Where to find BSP header files
IPATH += $(BOARD_DIR)/Include
IPATH += $(BOARD_DIR)/../Include
