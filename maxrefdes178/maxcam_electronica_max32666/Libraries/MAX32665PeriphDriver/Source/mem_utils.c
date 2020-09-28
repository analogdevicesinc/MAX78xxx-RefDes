/* ***************************************************************************
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
 * $Date: 2018-10-16 00:49:29 +0300 (Tue, 16 Oct 2018) $
 * $Revision: 38520 $
 *
 ************************************************************************** */

#include "mxc_config.h"
#include "mxc_errors.h"
#include "mxc_assert.h"
#include "mem_utils.h"


/* ************************************************************************ */
void memset32(uint32_t * dst, uint32_t value, unsigned int len)
{
    MXC_ASSERT((len & 3) == 0);
    while (len) {
        *dst = value;
        dst++;
        len -= 4;
    }
}
/* ************************************************************************ */
void memcpy32(uint32_t * dst, uint32_t * src, unsigned int len)
{
    MXC_ASSERT((len & 3) == 0);
    while (len) {
        *dst = *src;
        dst++;
        src++;
        len -= 4;
    }
}

/* ************************************************************************ */
int memcmp32(uint32_t * dst, uint32_t * src, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < (len >> 2); i++) {
        if (dst[i] != src[i]) {
            return E_INVALID;
        }
    }
    return E_NO_ERROR;
}
