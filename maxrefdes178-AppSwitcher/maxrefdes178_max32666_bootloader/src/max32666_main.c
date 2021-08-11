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
char bootlaoder_string[40] = {0};
static const mxc_gpio_cfg_t button_x_int_pin = MAX32666_BUTTON_X_INT_PIN;
volatile int button_x_pressed = 0;
volatile int button_y_pressed = 0;


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
        PR_INFO("\r\n#App ISR table is not valid.\n");
        return FALSE;
    }
    return TRUE;
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

    if(!((check_if_app_is_valid() == FALSE) || (x_pressed == TRUE))) {
        loader_int_disable_bootloader_mode();
        Console_Shutdown();
        run_application();
        return E_NO_ERROR;
    }

    loader_int_enable_bootloader_mode();
    PR_INFO("maxrefdes178_max32666 bootloader v%d.%d.%d [%s]", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD, S_BUILD_TIMESTAMP);

    // Init button X interrupt
    MXC_GPIO_RegisterCallback(&button_x_int_pin, button_x_int_handler, NULL);
    MXC_GPIO_IntConfig(&button_x_int_pin, MAX32666_BUTTON_X_INT_MODE);
    MXC_GPIO_EnableInt(button_x_int_pin.port, button_x_int_pin.mask);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(button_x_int_pin.port)));

    ret = i2c_master_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("i2c_init failed %d", ret);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    ret = expander_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("expander_init failed %d", ret);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    ret = pmic_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("pmic_init failed %d", ret);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    ret = lcd_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("lcd_init failed %d", ret);
        pmic_led_blue(0);
        pmic_led_red(1);
    }

    // Draw bootlaoder title
    snprintf(bootlaoder_string, sizeof(bootlaoder_string) - 1, "MRD178 App Switcher v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    memset(lcd_buff, 0xff, sizeof(lcd_buff));
    fonts_putString(23, 3, bootlaoder_string, &Font_7x10, BLUE, 0, 0, lcd_buff);

    ret = sdcard_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("sdcard_init failed %d", ret);
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putString(1, 14, "SD card not found!", &Font_7x10, RED, 0, 0, lcd_buff);
        fonts_putString(1, 30, "Insert SD card and restart", &Font_7x10, BLACK, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
        MXC_Delay(MXC_DELAY_MSEC(1000));
        while(1);
    }

    ret = sdcard_get_dirs(dir_list, &dir_count);
    if (ret != E_NO_ERROR) {
        pmic_led_blue(0);
        pmic_led_red(1);
        PR_ERROR("sdcard_get_dirs failed %d", ret);
    }

    if (dir_count == 0) {
        PR_ERROR("No folder found in SD card!");
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putString(1, 14, "No folder found in SD card!", &Font_7x10, RED, 0, 0, lcd_buff);
        fonts_putString(1, 30, "Please restart", &Font_7x10, BLACK, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
        MXC_Delay(MXC_DELAY_MSEC(1000));
        while(1);
    }

    pmic_led_blue(1);

    while (1) {
        memset(lcd_buff, 0xff, sizeof(lcd_buff));
        fonts_putString(23, 3, bootlaoder_string, &Font_7x10, BLUE, 0, 0, lcd_buff);

        expander_worker();

        if (button_x_pressed) {
            button_x_pressed = 0;
            selected = (selected + 1) % dir_count;
        }

        for (int i = 0; i < dir_count; i++) {
            if (i == selected) {
                fonts_putString(1, 14 + (i * 10), dir_list[i], &Font_7x10, GREEN, 0, 0, lcd_buff);
            } else {
                fonts_putString(1, 14 + (i * 10), dir_list[i], &Font_7x10, BLACK, 0, 0, lcd_buff);
            }
        }

        if (button_y_pressed) {
            button_y_pressed = 0;

            ret = sdcard_get_fw_paths(dir_list[selected], max32666_msbl_path, max78000_video_msbl_path, max78000_audio_msbl_path);
            if (ret != E_NO_ERROR) {
                PR_ERROR("Folder content is invalid! %s", dir_list[selected]);
                memset(lcd_buff, 0xff, sizeof(lcd_buff));
                fonts_putString(23, 3, bootlaoder_string, &Font_7x10, BLUE, 0, 0, lcd_buff);
                fonts_putString(1, 20, "Folder content is invalid!", &Font_7x10, RED, 0, 0, lcd_buff);
                fonts_putString(1, 40, dir_list[selected], &Font_7x10, RED, 0, 0, lcd_buff);
                lcd_drawImage(lcd_buff);
                MXC_Delay(MXC_DELAY_SEC(3));
            } else {
                PR_INFO("Firmware update started for: %s", dir_list[selected]);
                memset(lcd_buff, 0xff, sizeof(lcd_buff));
                fonts_putString(23, 3, bootlaoder_string, &Font_7x10, BLUE, 0, 0, lcd_buff);
                fonts_putString(1, 20, "Firmware update started for:", &Font_7x10, BLACK, 0, 0, lcd_buff);
                fonts_putString(1, 40, dir_list[selected], &Font_7x10, BROWN, 0, 0, lcd_buff);
                lcd_drawImage(lcd_buff);
                break;
            }
        }

        lcd_drawImage(lcd_buff);
        MXC_Delay(MXC_DELAY_MSEC(100));
    }

    PR_INFO("%s", max32666_msbl_path);
    PR_INFO("%s", max78000_video_msbl_path);
    PR_INFO("%s", max78000_audio_msbl_path);

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
        PR_ERROR("MAX78000 Audio FW Update Failed");
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putStringOver(1, 60, "MAX78000 Audio FW Update Failed", &Font_7x10, RED, 0, 0, lcd_buff);
        fonts_putString(1, 150, "Please restart", &Font_7x10, BLACK, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
        while(1);
    } else {
        PR_INFO("MAX78000 Audio FW Update OK");
        fonts_putStringOver(1, 60, "MAX78000 Audio FW Update OK", &Font_7x10, GREEN, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
    }

    // MAX78000 Video
    loader_int_set_active_ss(S_SS_VIDEO);
    ret = loader_flash_image(max78000_video_msbl_path, 1);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MAX78000 Video FW Update Failed");
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putStringOver(1, 80, "MAX78000 Video FW Update Failed", &Font_7x10, RED, 0, 0, lcd_buff);
        fonts_putString(1, 150, "Please restart", &Font_7x10, BLACK, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
        while(1);
    } else {
        PR_INFO("MAX78000 Video FW Update OK");
        fonts_putStringOver(1, 80, "MAX78000 Video FW Update OK", &Font_7x10, GREEN, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
    }

    // MAX32666 Self programmer
    ret = bl_load_from_sdcard(max32666_msbl_path);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MAX32666 FW Update Failed");
        pmic_led_blue(0);
        pmic_led_red(1);
        fonts_putStringOver(1, 100, "MAX32666 FW Update Failed", &Font_7x10, RED, 0, 0, lcd_buff);
        fonts_putString(1, 150, "Please restart", &Font_7x10, BLACK, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
        while(1);
    } else {
        PR_INFO("MAX32666 FW Update OK");
        fonts_putStringOver(1, 100, "MAX32666 FW Update OK", &Font_7x10, GREEN, 0, 0, lcd_buff);
        lcd_drawImage(lcd_buff);
    }

    ret = sdcard_uninit();
    if (ret != E_NO_ERROR) {
        PR_ERROR("sdcard_uninit failed %d", ret);
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
