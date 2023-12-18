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
#include <mxc_delay.h>
#include "max32666_debug.h"
#include "max32666_sdcard.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_utility.h"
#include "max32666_qspi_master.h"
#include "max32666_pmic.h"
#include "max32666_lcd.h"
#include "max32666_data.h" 
#include "max32666_fonts.h"
#include "max32666_embeddings.h"
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
FIL file,embed,database,record_embed;       //FFat File Object
FRESULT err;    //FFat Result (Struct)
FILINFO fno;    //FFat File Information Object
DIR dir;        //FFat Directory Object
TCHAR message[MESSAGE_MAX_LEN], directory[MESSAGE_MAX_LEN], cwd[MESSAGE_MAX_LEN], filename[MESSAGE_MAX_LEN], volume_label[24], volume = '0';
TCHAR *FF_ERRORS[ERROR_MAX_LEN];
DWORD clusters_free = 0, sectors_free = 0, sectors_total = 0, volume_sn = 0, seekoffset= 0;
UINT bytes_written = 0, bytes_read = 0, mounted = 0, embeddings_read = 0, bytes_read_embeddings = 0 ;
BYTE work[WORKING_BUFFER_LEN];
uint8_t kernel_buffer[25608];
uint32_t total_bytes;
uint32_t total_bytes_embeddings;
uint32_t capture_number = 0;
mxc_gpio_cfg_t SDPowerEnablePin = MAX32666_SD_EN_PIN;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int sdcard_mount(void);

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

    f_getcwd(cwd, sizeof(cwd));                         //Set the Current working directory

    return err;
}

static uint8_t *read_weights_from_SD(uint32_t datasize)
{
    // Read from SD file
    if ((err = f_read(&file, (uint8_t *)kernel_buffer, datasize, &bytes_read)) !=
        FR_OK) {
        printf("ERROR reading file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
 
    total_bytes += bytes_read;
    // Adjust position in file
    f_lseek(&file, total_bytes);

    return &kernel_buffer[0];
}

int cnn_2_load_weights_from_SD(void)
{
    uint8_t *ptr;
    if ((err = f_open(&file, "weights_2.bin", FA_READ)) != FR_OK) {
        pmic_led_red(1);
        fonts_putString(1, 14, "FaceID weights could not be found!", &Font_11x18, RED, 1, BLACK, lcd_data.buffer);
        lcd_drawImage(lcd_data.buffer);
        MXC_Delay(MXC_DELAY_MSEC(1000));


        printf("ERROR opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }

    // Set beginning of the file
    f_lseek(&file, 0);

    // Copy weights from SD file to intermediate kernel buffer in SRAM
    
    ptr = read_weights_from_SD(24304); 
     
    qspi_master_send_video((uint8_t *)ptr,24304,QSPI_PACKET_TYPE_WEIGHTS_KERNELS0);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(23440);       
    qspi_master_send_video((uint8_t *)ptr,23440,QSPI_PACKET_TYPE_WEIGHTS_KERNELS1);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(23440);       
    qspi_master_send_video((uint8_t *)ptr,23440,QSPI_PACKET_TYPE_WEIGHTS_KERNELS2);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(23440);       
    qspi_master_send_video((uint8_t *)ptr,23440,QSPI_PACKET_TYPE_WEIGHTS_KERNELS3);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(23440);       
    qspi_master_send_video((uint8_t *)ptr,23440,QSPI_PACKET_TYPE_WEIGHTS_KERNELS4);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(23440);       
    qspi_master_send_video((uint8_t *)ptr,23440,QSPI_PACKET_TYPE_WEIGHTS_KERNELS5);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(23440);       
    qspi_master_send_video((uint8_t *)ptr,23440,QSPI_PACKET_TYPE_WEIGHTS_KERNELS6);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(23440);       
    qspi_master_send_video((uint8_t *)ptr,23440,QSPI_PACKET_TYPE_WEIGHTS_KERNELS7);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(24592);
 
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS8);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(24592);       
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS9);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(24592); 
   
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS10);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(24592);       
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS11);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(24592);       
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS12);
    MXC_Delay(1000);
    ptr = read_weights_from_SD(24592);       
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS13);
    MXC_Delay(5000);
    ptr = read_weights_from_SD(24592);            
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS14);
    MXC_Delay(5000);
    ptr = read_weights_from_SD(24592);            
    qspi_master_send_video((uint8_t *)ptr,24592,QSPI_PACKET_TYPE_WEIGHTS_KERNELS15);
    MXC_Delay(1000);
    
    total_bytes = 0;
    if ((err = f_close(&file)) != FR_OK) {
        printf("ERROR closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }

    //printf("File Closed\n");

    return 0;
}
int find_names_number(char names[][7], int *number){
    if ((err = f_open(&database, "database.bin", FA_OPEN_ALWAYS| FA_READ)) != FR_OK) {
        printf("ERROR opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
    if ((err = f_read(&database, (uint8_t *)number, 4, &bytes_read)) != FR_OK) {
        printf("ERROR reading file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
    f_lseek(&database, 4); 
    if ((err = f_read(&database, names[DEFAULT_EMBS_NUM], *number*7, &bytes_read)) != FR_OK) {
        printf("ERROR reading file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }    
    if ((err = f_close(&database)) != FR_OK) {
        printf("ERROR closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }    
    return 0;
}
int record_embeddings(char *name, uint8_t *embeddings){

    uint32_t db_number = 0;
    uint32_t cnn_location = 0;
   // f_lseek(&file, write_offset);
    if ((err = f_open(&database, "database.bin", FA_OPEN_APPEND | FA_READ)) != FR_OK) {
        printf("ERROR opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
    f_lseek(&database, 0); 
    if ((err = f_read(&database, &db_number, 4, &bytes_read_embeddings)) !=
        FR_OK) {
        printf("ERROR reading file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
    if ((err = f_close(&database)) != FR_OK) {
        printf("ERROR closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
    cnn_location = db_number + DEFAULT_EMBS_NUM;
    int block_id = (cnn_location) / 9;
    int block_offset = (cnn_location) % 9;
    int write_offset = 8 - block_offset; // reverse order for each block;

    if ((err = f_open(&record_embed, "weights_3.bin", FA_WRITE | FA_OPEN_APPEND)) != FR_OK) {
        printf("ERROR opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }

    for(int i = 0; i<64 ;i++){
        seekoffset = ((i*257*4 +  write_offset + block_id * 9)/4)*4 + (3-(i*257*4 +  write_offset + block_id * 9) % 4) ;
        f_lseek(&record_embed, seekoffset);
        f_write(&record_embed, embeddings++, 1, &embeddings_read);
    }

    if ((err = f_close(&record_embed)) != FR_OK) {
        printf("ERROR closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    } 
       
    if ((err = f_open(&database, "database.bin", FA_OPEN_APPEND | FA_WRITE)) != FR_OK) {
        printf("ERROR opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }

    f_lseek(&database, 0);
    db_number++;     
    if ((err = f_write(&database, &db_number, 4, &embeddings_read)) != FR_OK) {
        printf("Error writing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }
    f_lseek(&database, 4 + ((db_number-1)*7));
    if ((err = f_write(&database, (char *)name, 7, &embeddings_read)) != FR_OK) {
        printf("Error writing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    } 
      
    if ((err = f_close(&database)) != FR_OK) {
        printf("ERROR closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
    return 0;
}

static uint8_t *read_embeddings_from_SD(uint32_t datasize)
{
    // Read from SD file
    if ((err = f_read(&embed, (uint8_t *)kernel_buffer, datasize, &bytes_read_embeddings)) !=
        FR_OK) {
        printf("ERROR reading file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }
 
    total_bytes_embeddings += bytes_read_embeddings;
    // Adjust position in file
    f_lseek(&embed, total_bytes_embeddings);
    //printf("%d bytes read\n", bytes_read);

    return &kernel_buffer[0];
}

int sending_embeddings(){
    uint8_t *ptr;
    if ((err = f_open(&embed, "weights_3.bin", FA_READ)) != FR_OK) {
        printf("ERROR opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
    }

    // Set beginning of the file
    f_lseek(&embed, 0);
    ptr = read_embeddings_from_SD(24672); 
    qspi_master_send_video((uint8_t *)ptr,24672,QSPI_PACKET_TYPE_DOT_PRODUCT_WEIGHTS_0);
    MXC_Delay(1000);
    ptr = read_embeddings_from_SD(24672); 
    qspi_master_send_video((uint8_t *)ptr,24672,QSPI_PACKET_TYPE_DOT_PRODUCT_WEIGHTS_1);
    MXC_Delay(1000);
    ptr = read_embeddings_from_SD(16448); 
    qspi_master_send_video((uint8_t *)ptr,16448,QSPI_PACKET_TYPE_DOT_PRODUCT_WEIGHTS_2);
    MXC_Delay(1000);

    total_bytes_embeddings = 0;

    if ((err = f_close(&embed)) != FR_OK) {
        printf("ERROR closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1)
            ;
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
    if((err = sdcard_mount()) != FR_OK) {
        PR_ERROR("Error opening SD Card: %s", FF_ERRORS[err]);
        return err;
    }



    return 0;
}
