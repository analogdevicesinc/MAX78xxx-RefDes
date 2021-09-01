/*******************************************************************************
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
 * $Date: 2019-05-30 14:18:04 -0500 (Thu, 30 May 2019) $
 * $Revision: 43593 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "mxc_errors.h"
#include "sdhc_regs.h"
#include "led.h"
#include "tmr.h"
#include "uart.h"
#include "gpio.h"

#include "audio_processor.h"
#include "maxrefdes178_definitions.h"
#include "max32666_data.h"
#include "max32666_qspi_master.h"
#include "max32666_expander.h"
#include "max32666_pmic.h"
#include "max32666_i2c.h"
#include "max32666_debug.h"

/***** Definitions *****/

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define S_MODULE_NAME   "main"

/***** Globals *****/


/******************************************************************************/
int main(void) {

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

    MXC_DMA_Init(MXC_DMA0);

    int ret;
    ret = i2c_master_init();
    if (ret != E_NO_ERROR) {
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    ret = expander_init();
    if (ret != E_NO_ERROR) {
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }
    expander_select_debugger(DEBUGGER_SELECT_MAX78000_AUDIO);

    ret = pmic_init();
    if (ret != E_NO_ERROR) {
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }
    pmic_led_red(1);

    qspi_packet_type_e pkt_type_rx = 0;

    printf("\n\n***** " TOSTRING(TARGET) " SDHC FAT Filesystem Example *****\n");

    if(initSDHC() != E_NO_ERROR) {
    	printf("Error initializing SDHC!\n");
    	return -1;
    }

    if(qspi_master_init() != E_NO_ERROR) {
    	printf("Error initializing QSPI!\n");
    	return -1;
    }

    //Get information about audio file
    myFiles[0] = "techno_background.wav";
    myFiles[1] = "rock.wav";
    myFiles[2] = "piano_loops.wav";
    myFiles[3] = "bass-loops.wav";

    WavFile_t fileStat;
    fileStat.fileNo = 0;

    while(1) {
    	if(qspi_master_audio_rx_worker(&pkt_type_rx) == E_NO_ERROR) {
    		if(pkt_type_rx == QSPI_PACKET_TYPE_AUDIO_CLASSIFICATION_RES) {
    			if(device_status.classification_audio.classification == CLASSIFICATION_DETECTED) {
    				if(strcmp(device_status.classification_audio.result, "GO") == 0) {
    					sendAudioStats(&fileStat);
    				}
    				else if(strcmp(device_status.classification_audio.result, "STOP") == 0) {
						if(fileStat.playing) {
							closeFile(&fileStat);
						}
					}
    				else if(strcmp(device_status.classification_audio.result, "LEFT") == 0) {
    					if(fileStat.playing) {
    						closeFile(&fileStat);
    					}

						if(fileStat.fileNo == 0) {
							fileStat.fileNo = NUM_FILES - 1;
						}
						else {
							fileStat.fileNo--;
						}

    					sendAudioStats(&fileStat);
    				}
    				else if(strcmp(device_status.classification_audio.result, "RIGHT") == 0) {
    					if(fileStat.playing) {
    						closeFile(&fileStat);
    					}

    					fileStat.fileNo = (fileStat.fileNo + 1) % NUM_FILES;

    					sendAudioStats(&fileStat);
    				}
    			}
    		}
    		else if(pkt_type_rx == QSPI_PACKET_TYPE_AUDIO_DATA_REQ && fileStat.playing) {
    			sendAudioData(&fileStat);
    			printf("Sending more data.\n");
    		}
    	}
    }

    //Unmount disk
    umount();

    //read file
    return 0;
}

