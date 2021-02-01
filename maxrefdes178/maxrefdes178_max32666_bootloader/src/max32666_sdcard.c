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
/*
 * @brief   read and write sdhc
 * @details This example uses the sdhc and ffat to read/write the file system on
 *          an SD card. The Fat library used supports long filenames (see ffconf.h)
 *          the max length is 256 characters.
 *
 *          You must connect an sd card to the sd card slot.
 */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <ff.h>
#include <gpio.h>
#include <mxc_sys.h>
#include <sdhc_lib.h>
#include <sdhc_regs.h>
#include <stdlib.h>
#include <string.h>

#include "max32666_debug.h"
#include "max32666_sdcard.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_utility.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME     "sdcard"

#define MESSAGE_MAX_LEN    256
#define ERROR_MAX_LEN      20


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
FATFS *fs;      //FFat Filesystem Object
FATFS fs_obj;
FIL file;       //FFat File Object
FRESULT err;    //FFat Result (Struct)
FILINFO fno;    //FFat File Information Object
DIR dir;        //FFat Directory Object
TCHAR message[MESSAGE_MAX_LEN], filename[MESSAGE_MAX_LEN], volume_label[24], volume = '0';
TCHAR *FF_ERRORS[ERROR_MAX_LEN];
DWORD clusters_free = 0, sectors_free = 0, sectors_total = 0, volume_sn = 0;
UINT bytes_written = 0, bytes_read = 0, mounted = 0;
mxc_gpio_cfg_t SDPowerEnablePin = MAX32666_SD_EN_PIN;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int sdcard_mount(void);
static int sdcard_umount(void);
static int sdcard_example(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
static int sdcard_mount(void)
{
    fs = &fs_obj;
    if((err = f_mount(fs, "", 1)) != FR_OK) {           //Mount the default drive to fs now
        PR_ERROR("Error opening SD card: %s", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
    }
    else {
        PR_INFO("SD card mounted.");
        mounted = 1;
    }

    return err;
}

static int sdcard_umount(void)
{
    if((err = f_mount(NULL, "", 0)) != FR_OK){          //Unmount the default drive from its mount point
        PR_ERROR("Error unmounting volume: %s", FF_ERRORS[err]);
    }
    else {
        PR_INFO("SD card unmounted.");
        mounted = 0;
    }

    return err;
}

static int sdcard_example(void)
{
    //open SD Card
    if((err = sdcard_mount()) != FR_OK) {
        PR_ERROR("Error opening SD Card: %s", FF_ERRORS[err]);
        return err;
    }
    printf("SD Card Opened!\n");

    PR_INFO("Attempting to read back file...");
    if((err = f_open(&file, "HelloMaxim.txt", FA_READ)) != FR_OK){
        PR_ERROR("Error opening file: %s", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    if((err = f_read(&file, &message, bytes_written, &bytes_read)) != FR_OK){
        PR_ERROR("Error reading file: %s", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    PR_INFO("Read Back %d bytes", bytes_read);
    PR_INFO("Message: ");
    PR_INFO("%s", message);

    if((err = f_close(&file)) != FR_OK){
        PR_ERROR("Error closing file: %s", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }
    PR_INFO("File Closed!\n");

    //unmount SD Card
    if((err = sdcard_umount()) != FR_OK){
        PR_ERROR("Error unmounting volume: %s", FF_ERRORS[err]);
        return err;
    }

    return 0;
}

int sdcard_init(void)
{
    mxc_sdhc_cfg_t cfg;

    FF_ERRORS[0] = "FR_OK";
    FF_ERRORS[1] = "FR_DISK_ERR";
    FF_ERRORS[2] = "FR_INT_ERR";
    FF_ERRORS[3] = "FR_NOT_READY";
    FF_ERRORS[4] = "FR_NO_FILE";
    FF_ERRORS[5] = "FR_NO_PATH";
    FF_ERRORS[6] = "FR_INVLAID_NAME";
    FF_ERRORS[7] = "FR_DENIED";
    FF_ERRORS[8] = "FR_EXIST";
    FF_ERRORS[9] = "FR_INVALID_OBJECT";
    FF_ERRORS[10] = "FR_WRITE_PROTECTED";
    FF_ERRORS[11] = "FR_INVALID_DRIVE";
    FF_ERRORS[12] = "FR_NOT_ENABLED";
    FF_ERRORS[13] = "FR_NO_FILESYSTEM";
    FF_ERRORS[14] = "FR_MKFS_ABORTED";
    FF_ERRORS[15] = "FR_TIMEOUT";
    FF_ERRORS[16] = "FR_LOCKED";
    FF_ERRORS[17] = "FR_NOT_ENOUGH_CORE";
    FF_ERRORS[18] = "FR_TOO_MANY_OPEN_FILES";
    FF_ERRORS[19] = "FR_INVALID_PARAMETER";
    srand(12347439);

    // Enable Power To Card
    MXC_GPIO_Config(&SDPowerEnablePin);
    GPIO_CLR(SDPowerEnablePin);

    // Initialize SDHC peripheral
    cfg.bus_voltage = MAX32666_SD_BUS_VOLTAGE;
    cfg.block_gap = 0;
    cfg.clk_div = MAX32666_SD_CLK_DIV; // Maximum divide ratio, frequency must be >= 400 kHz during Card Identification phase
    if(MXC_SDHC_Init(&cfg) != E_NO_ERROR) {
        PR_ERROR("Unable to initialize SDHC driver.");
        return 1;
    }

    // Check if card is inserted
    if (!MXC_SDHC_Card_Inserted()) {
        PR_ERROR("Card is not inserted.");
        return 1;
    } else {
        PR_INFO("Card inserted.");
    }

    // set up card to get it ready for a transaction
    if (MXC_SDHC_Lib_InitCard(MAX32666_SD_INIT_RETRY) == E_NO_ERROR) {
        PR_INFO("Card Initialized.");
    } else {
        PR_ERROR("No card response! Remove card, reset EvKit, and try again.");
        return -1;
    }

    if (MXC_SDHC_Lib_Get_Card_Type() == CARD_SDHC) {
        PR_INFO("Card type: SDHC");
    } else {
        PR_INFO("Card type: MMC/eMMC");
    }

    /* Configure for fastest possible clock, must not exceed 52 MHz for eMMC */
    if (SystemCoreClock > 96000000)  {
        PR_INFO("SD clock ratio (at card) 4:1");
        MXC_SDHC_Set_Clock_Config(1);
    } else {
        PR_INFO("SD clock ratio (at card) 2:1");
        MXC_SDHC_Set_Clock_Config(0);
    }

if (1) { // TODO: remove
    if(sdcard_example() != E_NO_ERROR) {
        PR_ERROR("SD CARD example failed");
        return 1;
    }
}

    return 0;
}
