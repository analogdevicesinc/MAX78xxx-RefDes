/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2020-08-19 10:35:13 -0500 (Wed, 19 Aug 2020) $
 * $Revision: 54889 $
 *
 ******************************************************************************/


/* Allocate space if SDMA_SIZE is defined at the start of SRAM*/
SRAM_ORIGIN = 0x20000000;
SDMA_CODE_LEN = DEFINED(SDMA_CODE_SIZE) ? SDMA_CODE_SIZE : 0;
SDMA_DATA_LEN = DEFINED(SDMA_DATA_SIZE) ? SDMA_DATA_SIZE : 0;
SDMA_LEN = SDMA_CODE_LEN + SDMA_DATA_LEN;
OTP_LEN = DEFINED(SDMA_CODE_SIZE) ? 0x40 : 0;
SHARED_LEN = DEFINED(SDMA_CODE_SIZE) ? 1k : 0;
SHARED_LEN_HALF = SHARED_LEN / 2;

OTP_ADDR = SRAM_ORIGIN;
SHARED_ADDR = OTP_ADDR + OTP_LEN;
SDMA_DATA_ADDR = SHARED_ADDR + SHARED_LEN;
SDMA_CODE_ADDR = SDMA_DATA_ADDR + SDMA_DATA_LEN;
SDMA_CODE_END = SDMA_CODE_ADDR + SDMA_CODE_LEN;

/* Used ARM_SRAM_START address if defined */
SRAM_ADDR = DEFINED(ARM_SRAM_START) ? ARM_SRAM_START : (SDMA_CODE_ADDR + SDMA_CODE_LEN);

/* Give leftover SRAM to the SRAM memory region */
/* SRAM_SIZE must be defined */
SRAM_LEN = SRAM_SIZE + SRAM_ORIGIN - SRAM_ADDR;

/* SPID and SPIX Sections here are maximum possible sizes */
/* If used, they should be adjusted for the external Flash/RAM size */
MEMORY {
    SPIX (rx)  : ORIGIN = 0x08000000,                           LENGTH = 128M
    BOOTLOADER (rx) : ORIGIN = 0x10000000, LENGTH = 0x0010000   /* 16 kb for bootloader */
    MAINAPP (rwx) : ORIGIN = 0x10010000, LENGTH = 0x000EFFC0 /* Main application */
    BOOTMEM (rw) : ORIGIN = 0x100FFFC0, LENGTH = 0x00000040 /* 64 bytes of memory for bootloader */

    OTP (rwx)       : ORIGIN = OTP_ADDR,                        LENGTH = OTP_LEN
    ARM_SHARED(rwx) : ORIGIN = SHARED_ADDR,                     LENGTH = SHARED_LEN_HALF
    SDMA_SHARED(rwx): ORIGIN = SHARED_ADDR + SHARED_LEN_HALF,   LENGTH = SHARED_LEN_HALF
    SDMA_DATA (rwx) : ORIGIN = SDMA_DATA_ADDR,                  LENGTH = SDMA_DATA_LEN
    SDMA_CODE (rwx) : ORIGIN = SDMA_CODE_ADDR,                  LENGTH = SDMA_CODE_LEN
    SRAM (rwx) : ORIGIN = SRAM_ADDR,                            LENGTH = SRAM_LEN
    SPID (rw)  : ORIGIN = 0x80000000,                           LENGTH = 512M
}

_bl_start = ORIGIN(BOOTLOADER);
_bl_len = LENGTH(BOOTLOADER);

_app_isr = ORIGIN(MAINAPP);
_app_start = ORIGIN(MAINAPP);                   /* Start of application */
_app_len = LENGTH(MAINAPP);
_app_end = ORIGIN(MAINAPP) + LENGTH(MAINAPP);   /* End of application */

_boot_mem_start = ORIGIN(BOOTMEM);                  /* Start of non-volatile memory for bootloader */
_boot_mem_len = LENGTH(BOOTMEM);                    /* Length of non-volatile memory for bootloader */
_boot_mem_end = ORIGIN(BOOTMEM) + LENGTH(BOOTMEM);  /* End of non-volatile memory for bootloader */
_boot_mode = _boot_mem_start + 12; /* Tells the bootloader whether it should jump directly to app or flash a new app */


SECTIONS {
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)    /* program code */
        *(.rodata*)  /* read-only data: "const" */

        KEEP(*(.init))
        KEEP(*(.fini))

        /* C++ Exception handling */
        KEEP(*(.eh_frame*))
        _etext = .;
    } > BOOTLOADER

    . = ORIGIN(MAINAPP);

    .main_app :
    {
        _main_app = .;
        KEEP(*(.main_app))
        _emain_app = .;
    } > MAINAPP

    .boot_mem :
    {
        _boot_mem = .;
        KEEP(*(.bootmem))
        _eboot_mem = .;
    } > BOOTMEM


    /* This section will keep the SPIX data until loaded into the external device */
    /* Upon initialization of SPIX (user code needs to do this) */
    .xip_section :
    {
        KEEP(*(.xip_section*))
    } > SPIX AT>BOOTLOADER

    __load_start_xip = LOADADDR(.xip_section);
    __load_length_xip = SIZEOF(.xip_section);

    /* it's used for C++ exception handling      */
    /* we need to keep this to avoid overlapping */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > BOOTLOADER

    .data :
    {
        _data = ALIGN(., 4);
        *(.data*)           /*read-write initialized data: initialized global variable*/
        *(.spix_config*)    /* SPIX configuration functions need to be run from SRAM */
        *(.flashprog*)      /* Flash program */


        /* These array sections are used by __libc_init_array to call static C++ constructors */
        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        _edata = ALIGN(., 4);
    } > SRAM AT>BOOTLOADER
    __load_data = LOADADDR(.data);

    .otp (NOLOAD) :
    {
        _otp = ALIGN(., 4);
        *(.otp*)           /*Copy of OTP memory*/

        _eotp = ALIGN(., 4);
    } > OTP

    .arm_shared (NOLOAD) :
    {
        _arm_shared = ALIGN(., 4);
        *(.arm_shared*)           /*Shared memory section*/

    _earm_shared = ALIGN(., 4);
    } > ARM_SHARED

    .sdma_shared (NOLOAD) :
    {
        _sdma_shared = ALIGN(., 4);
        *(.sdma_shared*)           /*Shared memory section*/

        _esdma_shared = ALIGN(., 4);
    } > SDMA_SHARED

    .sdma_code :
    {
        _sdma_code = ALIGN(., 4);
        *(.sdma_code*)           /*SDMA Code*/

        _esdma_code = ALIGN(., 4);
    } > SDMA_CODE AT>BOOTLOADER
    __load_sdma_code = LOADADDR(.sdma_code);

    .sdma_data :
    {
        _sdma_data = ALIGN(., 4);
        *(.sdma_data*)           /*SDMA Data*/

        _esdma_data = ALIGN(., 4);
    } > SDMA_DATA AT>BOOTLOADER
    __load_sdma_data = LOADADDR(.sdma_data);

    .bss :
    {
        . = ALIGN(4);
        _bss = .;
        *(.bss*)     /*read-write zero initialized data: uninitialzed global variable*/
        *(COMMON)
        _ebss = ALIGN(., 4);
    } > SRAM

    /* Setup the stack for Core 1, it will only be used if the user code
     * includes a definition of Stack_Size_Core1, which defines the space
     * reserved above the main core's stack for core 1's stack */

    __StackTop_Core1 = ORIGIN(SRAM) + LENGTH(SRAM);
    __StackLimit_Core1 = DEFINED(Stack_Size_Core1) ? __StackTop_Core1 - Stack_Size_Core1 : __StackTop_Core1;

    /* Set stack top to end of RAM, and stack limit move down by Stack_Size.
     * If core 1 is used, set the stack to the bottom of Core 1's stack region */

    __StackTop = DEFINED(Stack_Size_Core1) ? __StackLimit_Core1 : ORIGIN(SRAM) + LENGTH(SRAM);
    __StackLimit = __StackTop - Stack_Size;

    .heap (COPY):
    {
        . = ALIGN(4);
        *(.heap*)
        __HeapLimit = ABSOLUTE(__StackLimit);
    } > SRAM

    PROVIDE(__stack = __StackTop);

    /* Check if data + heap + stack(s) exceeds RAM limit */
    ASSERT(__StackLimit >= _ebss, "region RAM overflowed with stack")
}
