/*******************************************************************************
 * Copyright (C) 2020-2023 Maxim Integrated Products, Inc., All rights Reserved.
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
#include "spixf.h"
#include "max32666_debug.h"
#include "max32666_expander.h"
#include "max32666_fonts.h"
#include "max32666_i2c.h"
#include "max32666_lcd.h"
#include "max32666_pmic.h"
#include "max32666_sdcard.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_version.h"
#include "max32666_loader_int.h"
#include "max32666_bl.h"
#include "max32666_loader.h"
#include "mx25.h"
#include "mscmem.h"
#include "diskio.h"
#include "massStorage.h"
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
extern void *_app_isr[];
extern int _app_start;
extern int _app_end;
extern int _boot_mem_start;
extern int _boot_mode;
extern int _boot_mem_end;
extern int _boot_mem_len;
char dir_list[MAX32666_BL_MAX_DIR_NUMBER][MAX32666_BL_MAX_DIR_LEN] = {0};
char max32666_msbl_path[MAX32666_BL_MAX_FW_PATH_LEN] = {0};
char max78000_video_msbl_path[MAX32666_BL_MAX_FW_PATH_LEN] = {0};
char max78000_audio_msbl_path[MAX32666_BL_MAX_FW_PATH_LEN] = {0};
char bootloader_string[40] = {0};
static const mxc_gpio_cfg_t button_x_int_pin = MAX32666_BUTTON_X_INT_PIN;
volatile uint8_t button_x_pressed = 0;
volatile uint8_t button_y_pressed = 0;
static uint8_t flash = 0;
//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void run_application(void);
static int check_if_app_is_valid(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void button_x_int_handler(void *cbdata)
{
    button_x_pressed = 1;
}

void button_y_int_handler(int state)
{
    if (state) {
        PR_DEBUG("button Y released");
    } else {
        PR_DEBUG("button Y pressed");
        button_y_pressed = 1;
    }
}

static int check_if_app_is_valid(void)
{
    //Does app have a valid isr vector table?
    if ((uint32_t)_app_isr[1] == UNINITIALIZED_MEM) {
        return FALSE;
    }
    return TRUE;
}

static void flash_setup(){
    MXC_SPIXF_Disable();
    MXC_SPIXF_SetSPIFrequency(EXT_FLASH_BAUD);
    MXC_SPIXF_SetMode(MXC_SPIXF_MODE_0);
    MXC_SPIXF_SetSSPolActiveLow();
    MXC_SPIXF_SetSSActiveTime(MXC_SPIXF_SYS_CLOCKS_0);
    MXC_SPIXF_SetSSInactiveTime(MXC_SPIXF_SYS_CLOCKS_1);
    MXC_SPIXF_SetCmdValue(EXT_FLASH_CMD_QREAD);
    MXC_SPIXF_SetCmdWidth(MXC_SPIXF_QUAD_SDIO);
    MXC_SPIXF_SetAddrWidth(MXC_SPIXF_QUAD_SDIO);
    MXC_SPIXF_SetDataWidth(MXC_SPIXF_WIDTH_4);
    MXC_SPIXF_SetModeClk(EXT_FLASH_QREAD_DUMMY);
}

int main(void)
{
    int ret = 0;
    int dir_count = 0;
    int selected = 0;
    unsigned int x_pressed;

    // Set PORT1 and PORT2 rail to VDDIO
    MXC_GPIO0->vssel =  0x00;
    MXC_GPIO1->vssel =  0x00;


    MXC_GPIO_Config(&button_x_int_pin);
    x_pressed = (MXC_GPIO_InGet (button_x_int_pin.port, button_x_int_pin.mask)& button_x_int_pin.mask) ? FALSE : TRUE;

    loader_int_enable_bootloader_mode();

    // Init button X interrupt
    MXC_GPIO_RegisterCallback(&button_x_int_pin, button_x_int_handler, NULL);
    MXC_GPIO_IntConfig(&button_x_int_pin, MAX32666_BUTTON_X_INT_MODE);
    MXC_GPIO_EnableInt(button_x_int_pin.port, button_x_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(button_x_int_pin.port)));

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
    uint8_t buff[2];

    i2c_master_byte_read_buf(I2C_ADDR_MAX7325_PORTS, buff, 2);
    if(!((check_if_app_is_valid() == FALSE) || (x_pressed == TRUE) || (((buff[1] & EXPANDER_INPUT_BUTTON_Y)==0) && ((buff[0] & EXPANDER_INPUT_BUTTON_Y) == 0)))) {
        loader_int_disable_bootloader_mode();
        Console_Shutdown();
        run_application();
        return E_NO_ERROR;
    }

    ret = pmic_init();
    if (ret != E_NO_ERROR) {
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    ret = lcd_init();
    if (ret != E_NO_ERROR) {
        pmic_led_blue(0);
        pmic_led_red(1);
    }

    if(((buff[1] & EXPANDER_INPUT_BUTTON_Y)==0) && ((buff[0] & EXPANDER_INPUT_BUTTON_Y) == 0)){
    	loader_int_disable_bootloader_mode();
        memset(lcd_buff, 0x00, sizeof(lcd_buff));
        fonts_putString(6, 33, "USBMassStorage", &Font_16x26, WHITE, 1,BLACK, lcd_buff);
        fonts_putString(6, 123, "Restart the camera to start applications", &Font_11x18, BLUE, 1,BLACK, lcd_buff);
        pmic_led_green(1);
        pmic_led_blue(1);
    	lcd_drawImage(lcd_buff);
    	massstorage();
    	return 0;
    }
    // Draw bootloader title
    snprintf(bootloader_string, sizeof(bootloader_string) - 1, "App Switcher v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    memset(lcd_buff, 0x00, sizeof(lcd_buff));
    fonts_putString(6, 3, bootloader_string, &Font_11x18, BLUE, 1,BLACK, lcd_buff);

    ret = sdcard_init();
    if (ret != E_NO_ERROR) {
        if(ret == 3){
        	flash = 1;
            disk_flash(flash);
            flash_setup();
            int err;
            if((err = sdcard_mount()) != E_NO_ERROR) {
            	pmic_led_blue(0);
            	pmic_led_red(1);
            	fonts_putString(1, 34, "External flash not   initialized", &Font_11x18, RED, 1, BLACK, lcd_buff);
            	fonts_putString(1, 84, "Start the camera in  mass storage mode and format the external  flash with the FAT32 file system.", &Font_11x18, RED, 1, BLACK, lcd_buff);
            	lcd_drawImage(lcd_buff);
            	MXC_Delay(MXC_DELAY_MSEC(1000));
            	while(1);
            }
        }
        else{
        	pmic_led_blue(0);
        	pmic_led_red(1);
        	fonts_putString(1, 14, "SD card not found!", &Font_11x18, RED, 1, BLACK, lcd_buff);
        	lcd_drawImage(lcd_buff);
        	MXC_Delay(MXC_DELAY_MSEC(1000));
        	while(1);
        }
    }


    ret = sdcard_get_dirs(dir_list, &dir_count);
    if (ret != E_NO_ERROR) {
        pmic_led_blue(0);
        pmic_led_red(1);
    }

    if (dir_count == 0) {
    	fonts_putString(1, 40, "No folder found!", &Font_11x18, RED, 1, BLACK, lcd_buff);
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putString(1,100, "Please restart", &Font_11x18, WHITE, 1, BLACK, lcd_buff);
        lcd_drawImage(lcd_buff);
        MXC_Delay(MXC_DELAY_MSEC(1000));
        while(1);
    }

    pmic_led_blue(1);

    while (1) {
        memset(lcd_buff, 0x00, sizeof(lcd_buff));

        fonts_putString(6, 3, bootloader_string, &Font_11x18, BLUE, 1, BLACK, lcd_buff);
        fonts_putString(4, 40, "Select the demo:", &Font_11x18, WHITE, 1, BLACK, lcd_buff);
        fonts_putString(4, LCD_HEIGHT - 18, "[X]->TOGGLE [Y]->LOAD" , &Font_11x18, LIGHTBLUE, 1, BLACK, lcd_buff);

        expander_worker();

        if (button_x_pressed) {
            button_x_pressed = 0;
            selected = (selected + 1) % dir_count;
        }

        if(selected<5){
        	for (int i = 0; i < 5; i++) {
        		if (i == selected) {
        			fonts_putString(5, 3*25 + (i * 26), dir_list[i], &Font_16x26, GREEN, 1, BLACK, lcd_buff);
        		} else {
        			fonts_putString(5, 3*25 + (i * 26), dir_list[i], &Font_16x26, WHITE, 1, BLACK, lcd_buff);
        		}
        	}
        }
        else{
        	for (int i = 0; i < 4; i++) {
        			fonts_putStringOver(5, 3*25 + (4 * 26), dir_list[selected], &Font_16x26, GREEN, 1, BLACK, lcd_buff);
        			fonts_putStringOver(5, 3*25 + (i * 26), dir_list[selected-(4-i)], &Font_16x26, WHITE, 1, BLACK, lcd_buff);
        	}
        }

        if (button_y_pressed) {
            button_y_pressed = 0;

            ret = sdcard_get_fw_paths(dir_list[selected], max32666_msbl_path, max78000_video_msbl_path, max78000_audio_msbl_path);
            if (ret != E_NO_ERROR) {
                memset(lcd_buff, 0x00, sizeof(lcd_buff));
                fonts_putString(1, 30, "Folder content is not valid!", &Font_11x18, RED, 1, BLACK, lcd_buff);
                fonts_putString(1, 80, dir_list[selected], &Font_11x18, RED, 1, BLACK, lcd_buff);
                lcd_drawImage(lcd_buff);
                MXC_Delay(MXC_DELAY_SEC(3));
            } else {
                memset(lcd_buff, 0x00, sizeof(lcd_buff));
                fonts_putStringCentered(30, dir_list[selected], &Font_16x26, BROWN,lcd_buff);
                fonts_putString(1, 80, "Updating Firmware", &Font_11x18, WHITE, 1, BLACK, lcd_buff);
                lcd_drawImage(lcd_buff);
                break;
            }
        }

        lcd_drawImage(lcd_buff);
        MXC_Delay(MXC_DELAY_MSEC(100));
    }

    // Erase MAX32666 FW
    bl_master_erase();

    loader_int_spi_init();
    loader_int_gpio_init();
    bl_conf_struct_t plt;

    plt.read     = loader_int_spi_read;
    plt.write    = loader_int_spi_write;
    plt.gpio_set = loader_int_gpio_set;
    plt.delay_ms = loader_int_delay_ms;

    loader_init(&plt);

    // MAX78000 Audio
    loader_int_set_active_ss(S_SS_AUDIO);
    ret = loader_flash_image(max78000_audio_msbl_path, 0);
    if (ret != E_NO_ERROR) {
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putStringOver(1, 120, "Audio FW Update Failed", &Font_11x18, RED, 1,BLACK, lcd_buff);
        lcd_drawImage(lcd_buff);
        while(1);
    } else {

        fonts_putStringOver(1, 120, "Audio FW Update OK", &Font_11x18, WHITE, 1,BLACK, lcd_buff);
        lcd_drawImage(lcd_buff);
    }

    // MAX78000 Video
    loader_int_set_active_ss(S_SS_VIDEO);
    ret = loader_flash_image(max78000_video_msbl_path, 1);
    if (ret != E_NO_ERROR) {
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putStringOver(1, 160, "Video FW Update Failed", &Font_11x18, RED, 1, BLACK, lcd_buff);
        lcd_drawImage(lcd_buff);
        while(1);
    } else {
        fonts_putStringOver(1, 160, "Video FW Update OK", &Font_11x18, WHITE, 1,BLACK, lcd_buff);
        lcd_drawImage(lcd_buff);
    }

    // MAX32666 Self programmer
    ret = bl_load_from_sdcard(max32666_msbl_path);
    if (ret != E_NO_ERROR) {
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putStringOver(1, 200, "MAX32666 FW Update Failed", &Font_11x18, RED, 1,BLACK, lcd_buff);
        lcd_drawImage(lcd_buff);
        while(1);
    } else {
        fonts_putStringOver(1, 200, "MAX32666 FW Update OK", &Font_11x18, WHITE, 1, BLACK, lcd_buff);
        lcd_drawImage(lcd_buff);
    }

    ret = sdcard_uninit();
    if (ret != E_NO_ERROR) {
        pmic_led_blue(0);
        pmic_led_red(1);
    }

    pmic_led_red(0);
    pmic_led_blue(0);
    pmic_led_green(1);

    MXC_Delay(MXC_DELAY_MSEC(500));
    pmic_hard_reset();

    MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);

    return E_NO_ERROR;
}

void hal_sys_jump_fw(void *fw_entry);
static void run_application(void)
{
    hal_sys_jump_fw((void(*)())_app_isr[1]);
}
