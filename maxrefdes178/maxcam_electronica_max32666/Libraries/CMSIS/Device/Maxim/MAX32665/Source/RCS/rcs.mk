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
 # $Date: 2020-08-10 21:49:50 +0300 (Mon, 10 Aug 2020) $ 
 # $Revision: 54774 $
 #
 ###############################################################################

# The build directory
ifeq "$(BUILD_DIR)" ""
BUILD_DIR=$(CURDIR)/build
endif

# Create output object file names
SRCS_NOPATH := $(foreach NAME,$(SRCS),$(basename $(notdir $(NAME))).c)
OBJS_NOPATH := $(SRCS_NOPATH:.c=.o)
OBJS        := $(OBJS_NOPATH:%.o=$(BUILD_DIR)/%.o)

################################################################################
# Goals

# The default goal, which causes the example to be built.
.DEFAULT_GOAL :=
.PHONY: all
all: mkbuildir
all: ${BUILD_DIR}/${PROJECT}.hzx
all: ${BUILD_DIR}/${PROJECT}.hex
all: ${BUILD_DIR}/${PROJECT}data.txt
all: ${BUILD_DIR}/${PROJECT}code.txt
all: ${BUILD_DIR}/${PROJECT}map.h
all: ${BUILD_DIR}/${PROJECT}.lst

# The goal to build as a library
.PHONY: lib
lib: mkbuildir
lib: ${BUILD_DIR}/${PROJECT}.a

# The goal to create the target directory.
.PHONY: mkbuildir
mkbuildir:
	@mkdir -p ${BUILD_DIR}

# The goal to clean out all the build products.
.PHONY: clean
clean:
	@rm -rf ${BUILD_DIR} ${wildcard *~}

${BUILD_DIR}/${PROJECT}.hzx: ${LIBS} ${OBJS}
${BUILD_DIR}/${PROJECT}.a: ${OBJS}

# Create a goal to exercise the library build dependencies
.PHONY: FORCE
FORCE:

# The command for calling the compiler.
CC=hcc

# The command for calling the assembler. 
AS=has

# The command for calling the library archiver.
AR=har

# The command for calling the linker.
LD=hld

# The command for creating hex file.
HEX=hex

# The command for creating list file.
LST=hls

# The flags passed to the assembler.
AFLAGS=	-we \
		-DSTACK_START=$(STACK_START)
		
ifneq "$(HEAP_SIZE)" ""
AFLAGS+=-D__HEAP_SIZE__=$(HEAP_SIZE)
endif
ifneq "$(STACK_SIZE)" ""
AFLAGS+=-D__STACK_SIZE=$(STACK_SIZE)
endif
AFLAGS+=$(PROJ_AFLAGS)

ifeq "$(MXC_OPTIMIZE_CFLAGS)" ""
# Optimize at level level which must be between -9 and +9. 
# Negative values of level optimize code space at the expense of speed, 
# whereas positive values of level optimize for speed at the expense of code space. 
# The ‘+’ sign for positive optimization levels is accepted but not required.
MXC_OPTIMIZE_CFLAGS = -O0
endif

# The flags passed to the compiler.
# MAXQ1103 closest equilavent to ME14 SDMA
CFLAGS= \
		$(MXC_OPTIMIZE_CFLAGS) \
		-we \
		-D__CROSSWORKS \
		-D__CROSSWORKS_MAXQ \
		-D__CROSSWORKS_MAXQ30 \
		-D__TARGET_PROCESSOR=MAXQ1103 \
		-D__CROSSWORKS_MAJOR_VERSION=3 \
		-D__CROSSWORKS_MINOR_VERSION=1 \
		-DOTP_ADDR=$(OTP_ADDR) \
		-DSHARED_ADDR=$(SHARED_ADDR) \
		-J$(CMSIS_ROOT)/Include/RCS \
		-msd \
		-Orl \
		-mmpy
		
ifneq "$(TARGET)" ""
CFLAGS+=-DTARGET=$(shell echo $(TARGET) | tr -d '[:alpha:]')
endif

ifneq "$(TARGET_REV)" ""
CFLAGS+=-DTARGET_REV=$(TARGET_REV)
endif

# Exclude debug for 'release' builds
ifneq (${MAKECMDGOALS},release)
ifneq (${DEBUG},0)
CFLAGS+=-g
endif
endif

CFLAGS+=$(PROJ_CFLAGS)

# Optimization flags for the linker
# -Ocm \
# -Oph \
# -Oxj \
# -Osf \
# -Ojc \
# -Ojt \
# -Obl \

# The flags passed to the linker.
# Need to place the ISR section behind the code to force a long jump in the handler
LDFLAGS= \
		-we \
		-D___vfprintf=___vfprintf_int \
		-D___vfscanf=___vfscanf_int \
		-D___vfprintf_c=___vfprintf_c_int \
		-D___vfscanf_c=___vfscanf_c_int \
		-e$(ENTRY) \
		-GSTARTUP=L \
		-GCODE=L \
		-GISR=L \
		-GCONST=L \
		-GIDATA0=L \
		-GUDATA0=L \
		-GLIBC_CODE=L \
		-G.NOINIT=L \
		-GLIBC_IDATA0=L \
		-GLIBC_UDATA0=L \
		-GLIBC_CONST=L \
		-G.abs=L \
		-TSTARTUP,CODE,ISR,CONST,LIBC_CODE=$(CODE_MEMRANGE) \
		-T=$(CODE_MEMRANGE) \
		-TLIBC_IDATA0,LIBC_UDATA0,LIBC_CONST,IDATA0,UDATA0,NOINIT,.abs=$(DATA_MEMRANGE) \
		-T=$(DATA_MEMRANGE) \
		-M${BUILD_DIR}/${PROJECT}.map \
		-g

LDFLAGS+=$(PROJ_LDFLAGS)

# Exclude debug for 'release' builds
ifneq (${MAKECMDGOALS},release)
ifneq (${DEBUG},0)
LDFLAGS+=-g
endif
endif

STD_LIBS=	\
			${LIBPATH}/libm_sf.hza \
			${LIBPATH}/libm_sd.hza \
			${LIBPATH}/libdebugio_sd.hza \
			${LIBPATH}/libdebugio_sf.hza \
			${LIBPATH}/libc_user_libc_sd.hza \
			${LIBPATH}/libc_sf.hza \
			${LIBPATH}/libc_sd.hza \
			${LIBPATH}/libc_user_libc_sf.hza \
			${LIBPATH}/libvfprintf_sd.hzo

# Add the include file paths to AFLAGS and CFLAGS.
AFLAGS+=${patsubst %,-I%,$(IPATH)}
CFLAGS+=${patsubst %,-I%,$(IPATH)}

################################################################################
# The rule for building the object file from each C source file.
${BUILD_DIR}/%.o: %.c
	@if [ 'x${VERBOSE}' != x ];                                               	\
	then 																		\
	    echo ${CC} ${CFLAGS} -o ${@} ${<};     	                                \
	elif [ 'x${QUIET}' != x ];                                                  \
	then																		\
		:;																		\
	else																		\
	    echo "  CC    ${<}";                                                   	\
	fi 																	
	@${CC} ${CFLAGS} -o ${@} ${<}

# The rule for building the object file from each assembly source file.
${BUILD_DIR}/%.o: %.asm
	@if [ 'x${VERBOSE}' = x ];                                                  \
	then                                                                        \
	    echo "  AS    ${<}";                                                    \
	elif [ 'x${QUIET}' != x ];                                                  \
	then 																		\
		:;																		\
	else 																		\
	    echo ${AS} ${AFLAGS} -o ${@} ${<};   									\
	fi
	@${AS} ${AFLAGS} -o ${@} ${<}

# The rule for linking the application.
${BUILD_DIR}/%.hzx:
	@if [ 'x${VERBOSE}' = x ];                                                  \
	then                                                                        \
	    echo "  LD    ${@}";                                                    \
	else                                                                        \
	    echo ${LD}                                                              \
	      $(filter %.o, ${^})                                 					\
	      $(addsuffix .hza, $(basename $(filter %.a, ${^})))                    \
	      ${PROJ_LIBS}                                                          \
	      ${STD_LIBS}                                                           \
	      ${LDFLAGS}                                            				\
	      -o ${@}; 																\
	fi

	@for file in $(filter %.a, ${^}); do mv $$file $${file%.a}.hza; done # Rename .a to .hza
	@${LD}																		\
	      $(filter %.o, ${^})                                 					\
	      $(addsuffix .hza, $(basename $(filter %.a, ${^})))                    \
	      ${PROJ_LIBS}                                                          \
	      ${STD_LIBS}                                                           \
	      ${LDFLAGS}                                            				\
	      -o ${@}                                               				
	@for file in $(filter %.a, ${^}); do mv $${file%.a}.hza $$file; done # Rename back to .a

# The rule for creating an object library.
${BUILD_DIR}/%.a:
	@if [ 'x${VERBOSE}' = x ];                                                  \
	 then                                                                       \
	    echo "  AR    ${@}";                                                    \
	elif [ 'x${QUIET}' != x ];                                                  \
	then	 																	\
		:;																		\
	else 																		\
	    echo ${AR} -c -v ${@} ${^};             								\
	fi
	@${AR} -c -v ${@} ${^}

# Create S-Record output file
%.lst: %.hzx
	@if [ 'x${VERBOSE}' = x ];                                                  \
	 then                                                                       \
	     echo "Creating ${@}";                                                  \
	 else                                                                       \
	     echo ${LST} ${<} -o ${@}; 												\
	 fi
	@$(LST) ${<} -o ${@}

# Create Hex output file
%.hex: %.hzx
	@if [ 'x${VERBOSE}' = x ];                                                  \
	 then                                                                       \
	     echo "Creating ${@}";                                                  \
	 else                                                                       \
	     echo ${HEX} ${<} -o ${BUILD_DIR}/${PROJECT} -Fhex -P0x00;    			\
	 fi
	@${HEX} ${<} -o ${BUILD_DIR}/${PROJECT} -Fhex -P0x00;

# Map file created when hzx is generated.
# List dependency here.
%.map: %.hzx
	@echo "Update map timestamp."
	@[ -f "${@}" ] && touch "${@}" -r "${<}"

# Create data text output file
# List the sections out to place them in order
%data.txt: %.hzx
	@if [ 'x${VERBOSE}' = x ];                                                  \
	 then                                                                       \
	     echo "Creating ${@}";                                                  \
	 else                                                                       \
	     echo ${HEX} ${<} -o ${BUILD_DIR}/${PROJECT}data -Ftxt -P0x00 -TCONST       \
	     -TLIBC_IDATA0 -TLIBC_UDATA0 -TLIBC_CONST -TIDATA0; \
	 fi
	@${HEX} ${<} -o ${BUILD_DIR}/${PROJECT}data -Ftxt -P0x00 -TCONST -TLIBC_IDATA0 \
	-TLIBC_UDATA0 -TLIBC_CONST -TIDATA0;

# Create code text output file
# List the sections out to place them in order
%code.txt: %.hzx
	@if [ 'x${VERBOSE}' = x ];                                                  \
	 then                                                                       \
	     echo "Creating ${@}";                                                  \
	 else                                                                       \
	     echo ${HEX} ${<} -o ${BUILD_DIR}/${PROJECT}code -Ftxt -P0x00 -TSTARTUP 	  \
	     -TCODE -TISR -TLIBC_CODE; \
	 fi
	@${HEX} ${<} -o ${BUILD_DIR}/${PROJECT}code -Ftxt -P0x00 -TSTARTUP -TCODE -TISR \
	-TLIBC_CODE;

# Create code text output file
# List the sections out to place them in order
%map.h: %.map
	@SDMA_MEMSTART=`echo $(DATA_MEMRANGE) | cut -f 1 -d -`; \
	SDMA_MEMSTART_DEC=`printf "%d" $$SDMA_MEMSTART`; \
	SDMA_LL_CFG=0x"`grep '__ll_cfg' $< | head -1 | cut -f 1 -d ' '`"; \
	SDMA_LL_CFG_DEC=`printf "%d" $$SDMA_LL_CFG`; \
	SDMA_LL_CFG_OFFSET=`expr $$SDMA_LL_CFG_DEC - $$SDMA_MEMSTART_DEC`; \
	SDMA_BB_CFG=0x"`grep '__bb_cfg' $< | head -1 | cut -f 1 -d ' '`"; \
	SDMA_BB_CFG_DEC=`printf "%d" $$SDMA_BB_CFG`; \
	SDMA_BB_CFG_OFFSET=`expr $$SDMA_BB_CFG_DEC - $$SDMA_MEMSTART_DEC`; \
	echo "#define SDMA_LL_CFG_OFFSET $$SDMA_LL_CFG_OFFSET" >> $@; \
	echo "#define SDMA_BB_CFG_OFFSET $$SDMA_BB_CFG_OFFSET" >> $@

################################################################################
.PHONY: debug
debug:
	@echo CYGWIN = ${CYGWIN}
	@echo
	@echo BUILD_DIR = ${BUILD_DIR}
	@echo
	@echo SRCS = ${SRCS}
	@echo
	@echo SRCS_NOPATH = ${SRCS_NOPATH}
	@echo
	@echo OBJS_NOPATH = ${OBJS_NOPATH}
	@echo
	@echo OBJS = ${OBJS}
	@echo
	@echo LIBS = ${LIBS}
	@echo
	@echo VPATH = ${VPATH}
	@echo
	@echo IPATH = ${IPATH}
