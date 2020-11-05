/**
 * @file flc.h
 * @brief      Flash Controler driver.
 * @details    This driver can be used to operate on the embedded flash memory.
 */
/* ****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2020-07-20 23:20:09 +0300 (Mon, 20 Jul 2020) $
 * $Revision: 54317 $
 *
 *************************************************************************** */

/* **** Includes **** */
#include <string.h>
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "flc.h"
#include "flc_regs.h"
#include "mcr_regs.h" // For ECCEN registers.

/**
 * @ingroup flc
 * @{
 */

/* **** Definitions **** */

/* **** Globals **** */

/* **** Functions **** */
//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
static int busy_flc(mxc_flc_regs_t *flc)
{
    return (flc->cn & (MXC_F_FLC_CN_WR | MXC_F_FLC_CN_ME | MXC_F_FLC_CN_PGE));
}
//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
static int prepare_flc(mxc_flc_regs_t *flc)
{
    /* Check if the flash controller is busy */
    if (busy_flc(flc)) {
        return E_BUSY;
    }
    
    // Set flash clock divider to generate a 1MHz clock from the APB clock
    flc->clkdiv = SystemCoreClock / 1000000;
    
    /* Clear stale errors */
    if (flc->intr & MXC_F_FLC_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_INTR_AF;
    }
    
    /* Unlock flash */
    flc->cn = (flc->cn & ~MXC_F_FLC_CN_UNLOCK) | MXC_S_FLC_CN_UNLOCK_UNLOCKED;
    
    return E_NO_ERROR;
}
//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
// Length is number of 32-bit words
static int verify_data(uint32_t address, uint32_t length, uint32_t * data)
{
    volatile uint32_t * ptr;
    
    
    for (ptr = (uint32_t*)address; ptr < (((uint32_t*)(address)) + length); ptr++, data++) {
        if (*ptr != *data) {
            return E_BAD_STATE;
        }
    }
    
    return E_NO_ERROR;
}
//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    // IAR memory section declaration for the in-system flash programming functions to be loaded in RAM.
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int FLC_Init(const sys_cfg_t *sys_cfg)
{
    return SYS_FLC_Init(sys_cfg);
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    // IAR memory section declaration for the in-system flash programming functions to be loaded in RAM.
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int FLC_Busy(void)
{
    uint32_t flc_cn = 0;
    int i;
    mxc_flc_regs_t *flc;
    for (i = 0; i < MXC_FLC_INSTANCES; i++) {
        flc = MXC_FLC_GET_FLC(i);
        flc_cn = busy_flc(flc);
        if (flc_cn != 0) {
            break;
        }
    }
    return flc_cn;
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int FLC_MassEraseInst(int inst)
{
    int err;
    mxc_flc_regs_t *flc;
    flc = MXC_FLC_GET_FLC(inst);
    if ((err = prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }

    /* Write mass erase code */
    flc->cn = (flc->cn & ~MXC_F_FLC_CN_ERASE_CODE) | MXC_S_FLC_CN_ERASE_CODE_ERASEALL;

    /* Issue mass erase command */
    flc->cn |= MXC_F_FLC_CN_ME;

    /* Wait until flash operation is complete */
    while (busy_flc(flc));
    /* Lock flash */
    flc->cn &= ~MXC_F_FLC_CN_UNLOCK;

    /* Check access violations */
    if (flc->intr & MXC_F_FLC_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_INTR_AF;
        return E_BAD_STATE;
    }

    // Flush cache
    SYS_Flash_Operation();
    return E_NO_ERROR;  
}


//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int FLC_MassErase(void)
{
    int err;

    if((err = FLC_MassEraseInst(0)) != E_NO_ERROR) {
        return err;
    }

    return FLC_MassEraseInst(1);
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int FLC_PageErase(uint32_t address)
{
    int err;
    uint32_t addr;
    mxc_flc_regs_t *flc = NULL;
    
    // Get FLC Instance
    if ((err = SYS_FLC_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    if ((err = SYS_FLC_GetPhysicalAddress(address, &addr)) < E_NO_ERROR) {
        return err;
    }
    if ((err = prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }
    
    // Align address on page boundary
    address = address - (address % MXC_FLASH_PAGE_SIZE);
    /* Write page erase code */
    flc->cn = (flc->cn & ~MXC_F_FLC_CN_ERASE_CODE) | MXC_S_FLC_CN_ERASE_CODE_ERASEPAGE;
    /* Issue page erase command */
    flc->addr = addr;
    flc->cn |= MXC_F_FLC_CN_PGE;
    
    /* Wait until flash operation is complete */
    while (FLC_Busy());
    
    /* Lock flash */
    flc->cn &= ~MXC_F_FLC_CN_UNLOCK;
    
    /* Check access violations */
    if (flc->intr & MXC_F_FLC_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_INTR_AF;
        return E_BAD_STATE;
    }
    
    // Flush the cache
    SYS_Flash_Operation();
    
    return E_NO_ERROR;
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int FLC_MultiPageErase(uint32_t start, uint32_t end)
{
    int retval;
    uint32_t addr;
    
    // Align start and end on page boundaries
    start = start - (start % MXC_FLASH_PAGE_SIZE);
    end = end - (end % MXC_FLASH_PAGE_SIZE);
    
    for (addr = start; addr <= end; addr += MXC_FLASH_PAGE_SIZE) {
        retval = FLC_PageErase(addr);
        if (retval != E_NO_ERROR)  {
            return retval;
        }
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
int FLC_Erase(uint32_t start, uint32_t end, uint32_t *buffer, unsigned length)
{
    int retval;
    uint32_t start_align, start_len, end_align, end_len;
    
    MXC_ASSERT(buffer);
    
    // Align start and end on page boundaries, calculate length of data to buffer
    start_align = start - (start % MXC_FLASH_PAGE_SIZE);
    start_len = (start % MXC_FLASH_PAGE_SIZE);
    end_align = end - (end % MXC_FLASH_PAGE_SIZE);
    end_len = MXC_FLASH_PAGE_SIZE - (end % MXC_FLASH_PAGE_SIZE);
    
    // Make sure the length of buffer is sufficient
    if ((length < start_len) || (length < end_len)) {
        return E_BAD_PARAM;
    }
    
    
    // Start and end address are in the same page
    if (start_align == end_align) {
        if (length < (start_len + end_len)) {
            return E_BAD_PARAM;
        }
        
        // Buffer first page data and last page data, erase and write
        memcpy(buffer, (void*)start_align, start_len);
        memcpy(&buffer[start_len/sizeof(uint32_t)], (void*)end, end_len);
        retval = FLC_PageErase(start_align);
        if (retval != E_NO_ERROR) {
            return retval;
        }
        
        retval = FLC_Write(start_align, start_len, buffer);
        if (retval != E_NO_ERROR) {
            return retval;
        }
        retval = FLC_Write(end, end_len, &buffer[start_len/sizeof(uint32_t)]);
        if (retval != E_NO_ERROR) {
            return retval;
        }
        
        return E_NO_ERROR;
    }
    
    // Buffer, erase, and write the data in the first page
    memcpy(buffer, (void*)start_align, start_len);
    retval = FLC_PageErase(start_align);
    if (retval != E_NO_ERROR) {
        return retval;
    }
    
    retval = FLC_Write(start_align, start_len, buffer);
    if (retval != E_NO_ERROR) {
        return retval;
    }
    
    // Buffer, erase, and write the data in the last page
    memcpy(buffer, (void*)end, end_len);
    retval = FLC_PageErase(end_align);
    if (retval != E_NO_ERROR) {
        return retval;
    }
    
    retval = FLC_Write(end, end_len, buffer);
    if (retval != E_NO_ERROR) {
        return retval;
    }
    
    // Erase the remaining pages. MultiPageErase will not erase if start is greater than end.
    return FLC_MultiPageErase((start_align + MXC_FLASH_PAGE_SIZE), (end_align - MXC_FLASH_PAGE_SIZE));
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
// make sure to disable ICC with ICC_Disable(); before Running this function
int FLC_Write32(uint32_t address, uint32_t data)
{
    int err, i=0;
    uint32_t addr, byte;
    mxc_flc_regs_t *flc = NULL;
    uint32_t emptyFlash = 0xffffffff;
    volatile uint32_t * ptr;
    uint32_t current_data[4] = {0,0,0,0};
    if ((MXC_MCR->eccen & MXC_F_MCR_ECCEN_FL0ECCEN)
            || (MXC_MCR->eccen & MXC_F_MCR_ECCEN_FL1ECCEN)) {
            
        return E_BAD_STATE;
    }
    // Address checked if it is byte addressable
    if (address & 0x3) {
        return E_BAD_PARAM;
    }
    
    if ((err=verify_data(address,1,&emptyFlash))!=E_NO_ERROR) {
        return err;
    }
    
    // Get byte idx within 128-bit word
    byte = (address & 0xf);
    // Align address to 128-bit word
    address = address & 0xfffffff0;
    
    // Get FLC Instance
    if ((err = SYS_FLC_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    if ((err = SYS_FLC_GetPhysicalAddress(address, &addr)) < E_NO_ERROR) {
        return err;
    }
    if ((err = prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }
    
    // Get current data stored in flash
    for (ptr = (uint32_t*)address; ptr < (uint32_t*)(address + 16); ptr++, i++) {
        current_data[i] = *ptr;
    }
    
    // write the data
    flc->addr = addr;
    if (byte < 4) {
        current_data[0] = data;
    } else if (byte < 8) {
        current_data[1] = data;
    } else if (byte < 12) {
        current_data[2] = data;
    } else {
        current_data[3] = data;
    }
    return FLC_Write128(address,current_data);
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
// make sure to disable ICC with ICC_Disable(); before Running this function
int FLC_Write128(uint32_t address, uint32_t *data)
{
    int err;
    mxc_flc_regs_t *flc = NULL;
    uint32_t addr;
    
    // Address checked if it is 128-bit aligned
    if (address & 0xF) {
        return E_BAD_PARAM;
    }
    // Get FLC Instance
    if ((err = SYS_FLC_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    if ((err = SYS_FLC_GetPhysicalAddress(address, &addr)) < E_NO_ERROR) {
        return err;
    }
    if ((err = prepare_flc(flc)) != E_NO_ERROR) {
        return err;
    }
    
    // write 128-bits
    flc->cn &= ~MXC_F_FLC_CN_WDTH;
    
    // write the data
    flc->addr = addr;
    flc->data[0] = data[0];
    flc->data[1] = data[1];
    flc->data[2] = data[2];
    flc->data[3] = data[3];
    flc->cn |= MXC_F_FLC_CN_WR;
    
    /* Wait until flash operation is complete */
    while (busy_flc(flc));
    
    /* Lock flash */
    flc->cn &= ~MXC_F_FLC_CN_UNLOCK;
    
    /* Check access violations */
    if (flc->intr & MXC_F_FLC_INTR_AF) {
        flc->intr &= ~MXC_F_FLC_INTR_AF;
        return E_BAD_STATE;
    }
    
    // Flush the cache
    SYS_Flash_Operation();
    if ((err=verify_data(address,4,data))!=E_NO_ERROR) {
        return err;
    }
    
    return E_NO_ERROR;
}

//******************************************************************************
#ifdef __IAR_SYSTEMS_ICC__
    #pragma section=".flashprog"
#else
    __attribute__((section(".flashprog")))
#endif
// make sure to disable ICC with ICC_Disable(); before Running this function
int FLC_Write(uint32_t address, uint32_t length, uint32_t *buffer)
{
    int err;
    uint32_t bytes_written;
    uint8_t current_data[4];
    uint32_t *current_data_32 = (uint32_t *)current_data;

    // Align the address to a word boundary and read/write if we have to
    if (address & 0x3) {

        // Figure out how many bytes we have to write to round up the address
        bytes_written = 4 - (address & 0x3);

        // Save the data currently in the flash
        memcpy(current_data, (void*)(address & (~0x3)), 4);

        // Modify current_data to insert the data from buffer
        memcpy(&current_data[4-bytes_written], buffer, bytes_written);

        // Write the modified data
        if ((err = FLC_Write32(address - (address % 4), *current_data_32)) != E_NO_ERROR) {
            return err;
        }        

        address += bytes_written;
        length -= bytes_written;

        //Align the uint32_t buffer with the new address
        uint8_t *buffer_unaligned = (uint8_t *)buffer;
        buffer_unaligned += bytes_written;
        buffer = (uint32_t *)buffer_unaligned;
    }

    // Align the address to a 4-word (128bit) boundary
    while ( (length >= 4) && ((address & 0xF) != 0) ) {
        if ((err = FLC_Write32(address, *buffer)) != E_NO_ERROR) {
            return err;
        }

        address += 4;
        length -= 4;
        buffer += 1;
    }

    if (length >= 16) {
        while (length >= 16) {
            if ((err = FLC_Write128(address, buffer)) != E_NO_ERROR) {
                return err;
            }

            address += 16;
            length -= 16;
            buffer += 4;
    }

    }

    while (length >= 4) {
        if ((err = FLC_Write32(address, *buffer)) != E_NO_ERROR) {
            return err;
        }

        address += 4;
        length -= 4;
        buffer += 1;
    }

    if (length > 0) {
        // Save the data currently in the flash
        memcpy(current_data, (void*)(address), 4);

        // Modify current_data to insert the data from buffer
        memcpy(current_data, buffer, length);

        if ((err = FLC_Write32(address, *current_data_32)) != E_NO_ERROR) {
            return err;
        }
    }


    return E_NO_ERROR;
}

int FLC_EnableInt(uint32_t mask)
{ 
    mask &= (MXC_F_FLC_INTR_DONEIE |  MXC_F_FLC_INTR_AFIE);
    if (!mask) {
        /* No bits set? Wasn't something we can enable. */
        return E_BAD_PARAM;
    }
   
    /* Apply enables and write back, preserving the flags */
    MXC_FLC0->intr |= mask;
    
    return E_NO_ERROR;
}

int FLC_DisableInt(uint32_t mask)
{   
    mask &= (MXC_F_FLC_INTR_DONEIE |  MXC_F_FLC_INTR_AFIE);
    if (!mask) {
        /* No bits set? Wasn't something we can disable. */
        return E_BAD_PARAM;
    }

    /* Apply disables and write back, preserving the flags */
    MXC_FLC0->intr &= ~mask;
    
    return E_NO_ERROR;
}

int FLC_GetFlags(void)
{
    return (MXC_FLC0->intr & (MXC_F_FLC_INTR_DONE | MXC_F_FLC_INTR_AF));
}

int FLC_ClearFlags(uint32_t mask)
{
    mask &= (MXC_F_FLC_INTR_DONE | MXC_F_FLC_INTR_AF);
    if (!mask) {
        /* No bits set? Wasn't something we can clear. */
        return E_BAD_PARAM;
    }
    
    /* Both flags are write zero clear */
    MXC_FLC0->intr ^= mask;
    
    return E_NO_ERROR;
}

int FLC_InfoBlockUnlocked(uint32_t address)
{
    /* Check to see if info block is unlocked */
    /* Flash at offset 0x30 will appear erased */
    uint32_t* infoData = (uint32_t*)(address + 0x30);

    if(*infoData == 0xFFFFFFFF) {
        return E_NO_ERROR;
    }

    return E_UNINITIALIZED;
}

int FLC_UnlockInfoBlock(uint32_t address)
{
    int err;
    mxc_flc_regs_t *flc = NULL;
    
    if ((address < MXC_INFO_MEM_BASE) || (address >= (MXC_INFO_MEM_BASE +(MXC_INFO_MEM_SIZE * 2)))) {
        return E_BAD_PARAM;
    }
    if ((err = SYS_FLC_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    if (FLC_InfoBlockUnlocked(address) == E_NO_ERROR) {
        return E_NO_ERROR;
    }
    flc->acntl = 0x3a7f5ca3;
    flc->acntl = 0xa1e34f20;
    flc->acntl = 0x9608b2c1;

    SYS_Flash_Operation();

    return E_NO_ERROR;
}

int FLC_LockInfoBlock(uint32_t address)
{
    int err;
    mxc_flc_regs_t *flc = NULL;
    
    if ((address < MXC_INFO_MEM_BASE) || (address >= (MXC_INFO_MEM_BASE +(MXC_INFO_MEM_SIZE * 2)))) {
        return E_BAD_PARAM;
    }
    if ((err = SYS_FLC_GetByAddress(&flc, address)) != E_NO_ERROR) {
        return err;
    }
    flc->acntl = 0xDEADBEEF;

    SYS_Flash_Operation();

    return E_NO_ERROR;
}
/**@} end of group flc */

