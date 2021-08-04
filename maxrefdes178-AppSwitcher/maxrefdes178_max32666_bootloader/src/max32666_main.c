/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <board.h>
#include <core1.h>
#include <dma.h>
#include <icc_regs.h>
#include <mxc_delay.h>
#include <mxc_sys.h>
#include <nvic_table.h>
#include <stdint.h>
#include <string.h>
#include <tmr.h>

#include "max32666_debug.h"
#include "max32666_i2c.h"
#include "max32666_max20303.h"
#include "max32666_sdcard.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_version.h"

#include <errno.h>
#include <unistd.h>


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "main"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//static const mxc_gpio_cfg_t core1_swd_pin = MAX32666_CORE1_SWD_PIN;
extern void *_app_isr[];
extern int _app_start;
extern int _app_end;
extern int _boot_mem_start;
extern int _boot_mode;
extern int _boot_mem_end;
extern int _boot_mem_len;

//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void run_application(void);

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int main(void)
{
    // Set PORT1 and PORT2 rail to VDDIO
    MXC_GPIO0->vssel =  0x00;
    MXC_GPIO1->vssel =  0x00;

    PR_INFO("maxrefdes178_max32666 core0 v%d.%d.%d [%s]", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD, S_BUILD_TIMESTAMP);

    /*
     * TODO: Process files in SDCard to perform firmware upgrade
     */

    Console_Shutdown();
    run_application();

    return E_NO_ERROR;
}


void hal_sys_jump_fw(void *fw_entry);
static void run_application(void)
{
    hal_sys_jump_fw((void(*)())_app_isr[1]);
}
