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
#include "max32666_memory.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_utility.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME     "sdcard"

#define MESSAGE_MAX_LEN    256
#define ERROR_MAX_LEN      20
#define WORKING_BUFFER_LEN 4096


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
TCHAR message[MESSAGE_MAX_LEN], directory[MESSAGE_MAX_LEN], cwd[MESSAGE_MAX_LEN], filename[MESSAGE_MAX_LEN], volume_label[24], volume = '0';
TCHAR *FF_ERRORS[ERROR_MAX_LEN];
DWORD clusters_free = 0, sectors_free = 0, sectors_total = 0, volume_sn = 0;
UINT bytes_written = 0, bytes_read = 0, mounted = 0;
BYTE work[WORKING_BUFFER_LEN];
mxc_gpio_cfg_t SDPowerEnablePin = MAX32666_SD_EN_PIN;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int umount(uint8_t external_flash);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------


int memory_mount(uint8_t external_flash)
{
	fs = &fs_obj;
	if(external_flash){
	    if((err = f_mount(fs, "1:", 1)) != FR_OK) {           //Mount the default drive to fs now
	        f_mount(NULL, "", 0);
	    }
	    else {
	        mounted = 1;
	    }
	}
	else{
	    if((err = f_mount(fs, "", 1)) != FR_OK) {           //Mount the default drive to fs now
	        f_mount(NULL, "", 0);
	    }
	    else {
	        mounted = 1;
	    }
	}
    f_getcwd(cwd, sizeof(cwd));                         //Set the Current working directory
    return err;
}

static int umount(uint8_t external_flash)
{
	if(external_flash){
	    if((err = f_mount(NULL, "1:", 0)) != FR_OK){          //Unmount the external flash drive from its mount point
	        PR_ERROR("Error unmounting volume: %s", FF_ERRORS[err]);
	    }
	    else {
	        PR_INFO("Memory device unmounted.");
	        mounted = 0;
	    }
	}
	else{
		if((err = f_mount(NULL, "", 0)) != FR_OK){          //Unmount the default drive from its mount point
			PR_ERROR("Error unmounting volume: %s", FF_ERRORS[err]);
		}
		else {
			PR_INFO("Memory device unmounted.");
			mounted = 0;
		}
	}
    return err;
}

static int is_msbl_valid(const char *img)
{
	int ret = 0;
	char magic[8] = {0, };
	unsigned int rSize = 0;
	const unsigned int magicLen = 4; // strlen("msbl")

	ret = f_open(&file, (const TCHAR*)img, FA_READ);

	if (ret == FR_OK) {
		// no need to check return value to get more read ability
		f_read(&file, magic, magicLen, &rSize);

		// First 4bytes is magic, it shall be msbl
		if (strncmp(magic, "msbl", magicLen) == 0) {
			ret = 0;
		} else {
			ret = -1;
		}
	}

	f_close(&file);

	return ret;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"

int get_dirs(char dir_list[MAX32666_BL_MAX_DIR_NUMBER][MAX32666_BL_MAX_DIR_LEN], int *dir_count, uint8_t external_flash)
{
    *dir_count = 0;
    if(external_flash){
    	err = f_opendir(&dir, "1:");                       /* Open the directory */
    }
    else{
    	err = f_opendir(&dir, "/");
    }
    if (err == FR_OK) {
        for (;;) {
            err = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (err != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                if (strcmp(fno.fname, "System Volume Information") == 0) {
                    continue;
                }
                if (*dir_count == MAX32666_BL_MAX_DIR_NUMBER) {
                    break;
                }
                strncpy(dir_list[*dir_count], fno.fname, MAX32666_BL_MAX_DIR_LEN - 1);
                dir_list[*dir_count][MAX32666_BL_MAX_DIR_LEN - 1] = '\0';
                *dir_count += 1;
            } else {                                       /* It is a file. */
                PR_DEBUG("file %s", fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return 0;
}

int get_fw_paths(char *dir_path, char *max32666_msbl_path, char *max78000_video_msbl_path, char *max78000_audio_msbl_path, uint8_t external_flash)
{
	int ret = 0;
    int max32666_found = 0;
    int max78000_video_found = 0;
    int max78000_audio_found = 0;
    if(external_flash){
    	char flash_path[100] = "1:";
    	strcat(flash_path,dir_path);
    	strcpy(dir_path,flash_path);
    }
    err = f_opendir(&dir, dir_path);                       /* Open the directory */
    if (err == FR_OK) {
        for (;;) {
            err = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (err != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                PR_DEBUG("dir %s", fno.fname);
            } else {                                       /* It is a file. */
                PR_INFO("file %s", fno.fname);

                // Check extension
                if (strncmp(fno.fname + strlen(fno.fname) - strlen(MAX32666_BL_MAX32666_FW_EXTENSION), MAX32666_BL_MAX32666_FW_EXTENSION, strlen(MAX32666_BL_MAX32666_FW_EXTENSION)) != 0) {
                    continue;
                }

                // Check fw name
                if (strncmp(fno.fname, MAX32666_BL_MAX32666_FW_NAME, strlen(MAX32666_BL_MAX32666_FW_NAME)) == 0) {
                    max32666_found = 1;
                    strcpy(max32666_msbl_path, dir_path);
                    max32666_msbl_path[strlen(dir_path)] = '/';
                    strncpy(max32666_msbl_path + strlen(dir_path) + 1, fno.fname, MAX32666_BL_MAX_FW_PATH_LEN - 2 - strlen(dir_path));
                    max32666_msbl_path[MAX32666_BL_MAX_FW_PATH_LEN - 1] = '\0';
                } else if  (strncmp(fno.fname, MAX32666_BL_MAX78000_VIDEO_FW_NAME, strlen(MAX32666_BL_MAX78000_VIDEO_FW_NAME)) == 0) {
                    max78000_video_found = 1;
                    strcpy(max78000_video_msbl_path, dir_path);
                    max78000_video_msbl_path[strlen(dir_path)] = '/';
                    strncpy(max78000_video_msbl_path + strlen(dir_path) + 1, fno.fname, MAX32666_BL_MAX_FW_PATH_LEN - 2 - strlen(dir_path));
                    max78000_video_msbl_path[MAX32666_BL_MAX_FW_PATH_LEN - 1] = '\0';
                } else if  (strncmp(fno.fname, MAX32666_BL_MAX78000_AUDIO_FW_NAME, strlen(MAX32666_BL_MAX78000_AUDIO_FW_NAME)) == 0) {
                    max78000_audio_found = 1;
                    strcpy(max78000_audio_msbl_path, dir_path);
                    max78000_audio_msbl_path[strlen(dir_path)] = '/';
                    strncpy(max78000_audio_msbl_path + strlen(dir_path) + 1, fno.fname, MAX32666_BL_MAX_FW_PATH_LEN - 2 - strlen(dir_path));
                    max78000_audio_msbl_path[MAX32666_BL_MAX_FW_PATH_LEN - 1] = '\0';
                }
            }
        }
        f_closedir(&dir);
    }

    if (max32666_found && max78000_video_found && max78000_audio_found) {
        ret = 0;
    } else {
        ret = -1;
    }

    // check msbl format
    if (ret == 0) {
    	ret = is_msbl_valid(max32666_msbl_path);
    }

    if (ret == 0) {
    	ret = is_msbl_valid(max78000_video_msbl_path);
    }

    if (ret == 0) {
    	ret = is_msbl_valid(max78000_audio_msbl_path);
    }

    return ret;
}
#pragma GCC diagnostic pop

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

    // Enable Power To Card
    GPIO_CLR(SDPowerEnablePin);
    MXC_GPIO_Config(&SDPowerEnablePin);

    // Initialize SDHC peripheral
    cfg.bus_voltage = MAX32666_SD_BUS_VOLTAGE;
    cfg.block_gap = 0;
    cfg.clk_div = MAX32666_SD_CLK_DIV; // Maximum divide ratio, frequency must be >= 400 kHz during Card Identification phase
    if(MXC_SDHC_Init(&cfg) != E_NO_ERROR) {
        PR_ERROR("Unable to initialize SDHC driver.");
        return 1;
    }

    // wait for card to be inserted
    for (int i = 0; !MXC_SDHC_Card_Inserted() && (i < 100000); i++);

    // Check if card is inserted
    if (!MXC_SDHC_Card_Inserted()) {
        PR_ERROR("Card is not inserted.");
        return 3;
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

    if((err = memory_mount(0)) != FR_OK) {
        PR_ERROR("Error opening SD Card: %s", FF_ERRORS[err]);
        return err;
    }
    PR_INFO("SD Card Opened!\n");

    return 0;
}

int uninit(uint8_t external_flash)
{
    if((err = umount(external_flash)) != FR_OK) {
        PR_ERROR("Error umounting SD Card: %s", FF_ERRORS[err]);
        return err;
    }

    return 0;
}
