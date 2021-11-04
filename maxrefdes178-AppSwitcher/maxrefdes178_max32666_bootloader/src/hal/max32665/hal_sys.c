/*******************************************************************************
* Copyright (C) 2020-2021 Maxim Integrated Products, Inc., All rights Reserved.
*
* This software is protected by copyright laws of the United States and
* of foreign countries. This material may also be protected by patent laws
* and technology transfer regulations of the United States and of foreign
* countries. This software is furnished under a license agreement and/or a
* nondisclosure agreement and may only be used or reproduced in accordance
* with the terms of those agreements. Dissemination of this information to
* any party or parties not specified in the license agreement and/or
* nondisclosure agreement is expressly prohibited.
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
*******************************************************************************
*/

#include <string.h>

#include <hal/hal_sys.h>

#include <max32665.h>
#include <gcr_regs.h>
#include <icc.h>
#include <flc.h>
#include <mxc_delay.h>

extern uint32_t SystemCoreClock;

void hal_sys_init(void)
{

}

void hal_sys_delay(unsigned long us)
{
    MXC_Delay(us);
}

void hal_sys_jump_fw(void *fw_entry)
{
    ((void(*)())fw_entry)();
}

void hal_sys_reset(void)
{
    NVIC_SystemReset();
}

void hal_sys_lock_swd(void)
{
}

uint32_t hal_sys_start_systick(void)
{
    // If the SysTick is not running, configure and start it
    SysTick_Config(0x00FFFFFF);
    return SysTick->LOAD;
}

void hal_sys_disable_systick(void)
{
    SysTick->CTRL = 0;
}

uint32_t hal_sys_get_systick_freq(void)
{
    // Determine is using internal (SystemCoreClock) or external (32768) clock
    if ( (SysTick->CTRL & SysTick_CTRL_CLKSOURCE_Msk) || !(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk)) {
        return SystemCoreClock;
    } else {
        // We should not reach here
        while (1);
        __builtin_unreachable();
        return 0;
    }
}

void hal_sys_icc_enable(void)
{
    MXC_ICC_Enable();
}

void hal_sys_icc_disable(void)
{
    MXC_ICC_Disable();
}

void hal_sys_unlock_infoblock(void)
{
}

void hal_sys_lock_infoblock(void)
{
}

void hal_sys_get_usn(uint8_t *dst, uint8_t size)
{
    memset(dst, 0, size);
    hal_sys_unlock_infoblock();
    uint32_t *raw_data = (uint32_t *)MXC_INFO_MEM_BASE;

    for (int i = 0; i < 2; i += 2) {
        *dst++ = (raw_data[i] >> 15) & 0xFF;
        *dst++ = (raw_data[i] >> 23) & 0xFF;

        *dst++ = (raw_data[i + 1] & 0x7F) | ((raw_data[i] >> 31) & 0x1);
        *dst++ = (raw_data[i + 1] >> 7) & 0xFF;
        *dst++ = (raw_data[i + 1] >> 15) & 0xFF;
        *dst++ = (raw_data[i + 1] >> 23) & 0xFF;
    }

    *dst = (raw_data[4] >> 15) & 0xFF;
    hal_sys_lock_infoblock();
}
