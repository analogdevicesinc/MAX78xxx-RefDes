/* *****************************************************************************
 *
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
 * $Date: 2018-08-29 01:03:02 +0300 (Wed, 29 Aug 2018) $
 * $Revision: 37424 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include "emcc.h"

/* ************************************************************************** */
uint32_t EMCC_ID(emcc_cache_id_t id)
{
    switch (id) {
        case EMCC_CACHE_ID_RELNUM:
            return (((MXC_EMCC->cache_id) & MXC_F_EMCC_CACHE_ID_RELNUM)) >> MXC_F_EMCC_CACHE_ID_RELNUM_POS;

        case EMCC_CACHE_ID_PARTNUM:
            return (((MXC_EMCC->cache_id) & MXC_F_EMCC_CACHE_ID_PARTNUM)) >> MXC_F_EMCC_CACHE_ID_PARTNUM_POS;

        case EMCC_CACHE_ID_CCHID:
        default:
            return (((MXC_EMCC->cache_id) & MXC_F_EMCC_CACHE_ID_CCHID)) >> MXC_F_EMCC_CACHE_ID_CCHID_POS;
    }
}

/* ************************************************************************** */
uint32_t EMCC_Cache_Size(void)
{
    return (MXC_EMCC->memcfg & MXC_F_EMCC_MEMCFG_CCHSZ) >> MXC_F_EMCC_MEMCFG_CCHSZ_POS;
}

/* ************************************************************************** */
uint32_t EMCC_Mem_Size(void)
{
    return (MXC_EMCC->memcfg & MXC_F_EMCC_MEMCFG_MEMSZ) >> MXC_F_EMCC_MEMCFG_MEMSZ_POS;
}

/* ************************************************************************** */
void EMCC_Enable(void)
{
    MXC_EMCC->cache_ctrl |= MXC_F_EMCC_CACHE_CTRL_CACHE_EN;
}

/* ************************************************************************** */
void EMCC_Disable(void)
{
    MXC_EMCC->cache_ctrl &= ~MXC_F_EMCC_CACHE_CTRL_CACHE_EN;
}

/* ************************************************************************** */
void EMCC_Flush(void)
{
    EMCC_Disable();
    EMCC_Enable();
}

/* ************************************************************************** */
void EMCC_Write_Alloc_Enable(void)
{
    /* When a cache line is allocated on write operations, this is called
    "write-allocate". However, there can be performance problems with
    "write-allocate" because software frequently operates memset() on large
    portions of memory. This can "pollute" the cache with unwanted cache lines.
    To avoid this issue, the write-allocate feature is disable by default. The
    write- allocate enable bit is in CACHE_CTRL[1]. */
    MXC_EMCC->cache_ctrl |= MXC_F_EMCC_CACHE_CTRL_WRITE_ALLOC_EN;
}

/* ************************************************************************** */
void EMCC_Write_Alloc_Disable(void)
{
    MXC_EMCC->cache_ctrl &= ~MXC_F_EMCC_CACHE_CTRL_WRITE_ALLOC_EN;
}

/* ************************************************************************** */
void EMCC_Critical_Word_First_Enable(void)
{
    MXC_EMCC->cache_ctrl &= ~MXC_F_EMCC_CACHE_CTRL_CWFST_DIS;
}

/* ************************************************************************** */
void EMCC_Critical_Word_First_Disable(void)
{
    MXC_EMCC->cache_ctrl |= MXC_F_EMCC_CACHE_CTRL_CWFST_DIS;
}

/* ************************************************************************** */
uint32_t EMCC_Ready(void)
{
    /* Cache Ready flag. Cleared by hardware when at any time the cache as a
    whole is invalidated ( including a system reset). When this bit is 0, the
    cache is effectively in bypass mode (data fetches will come from main memory
    or from the line fill buffer). Set by hardware when the invalidate operation
    is complete and the cache is ready. */
    return MXC_EMCC->cache_ctrl & MXC_F_EMCC_CACHE_CTRL_CACHE_RDY;
}

/* ************************************************************************** */
void EMCC_Invalidate_All(void)
{
    /* Invalidate All Cache Contents. Any time this register location is written
    (regardless of the data value), the cache controller immediately begins
    invalidating the entire contents of the cache memory. The cache will be in
    bypass mode until the invalidate operation is complete. System software can
    examine the Cache Ready bit (CACHE_CTRL.CACHE_RDY) to determine when the
    invalidate operation is complete. Note that it is not necessary to disable
    the cache controller prior to beginning this operation. Reads from this
    register always return 0. */
    MXC_EMCC->invalidate |= MXC_F_EMCC_INVALIDATE_IA;
}
