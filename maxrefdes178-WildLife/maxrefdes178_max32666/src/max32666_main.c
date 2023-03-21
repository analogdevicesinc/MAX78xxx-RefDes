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

#include "max32666_accel.h"
#include "max32666_audio_codec.h"
//#include "max32666_ble.h"
//#include "max32666_ble_command.h"
//#include "max32666_ble_queue.h"
#include "max32666_data.h"
#include "max32666_debug.h"
#include "max32666_expander.h"
#include "max32666_ext_flash.h"
#include "max32666_ext_sram.h"
#include "max32666_fonts.h"
#include "max32666_fuel_gauge.h"
#include "max32666_i2c.h"
#include "max32666_lcd.h"
#include "max32666_lcd_images.h"
#include "max32666_pmic.h"
#include "max32666_powmon.h"
#include "max32666_qspi_master.h"
#include "max32666_sdcard.h"
#include "max32666_spi_dma.h"
#include "max32666_timer_led_button.h"
#include "max32666_touch.h"
#include "max32666_usb.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_version.h"


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
static volatile int core1_init_done = 0;
static char lcd_string_buff[LCD_NOTIFICATION_MAX_SIZE] = {0};
static char version_string[14] = {0};
static char usn_string[(sizeof(serial_num_t) + 1) * 3] = {0};
static char mac_string[(sizeof(device_info.ble_mac) + 1) * 3] = {0};
static uint16_t video_string_color;
static uint16_t video_frame_color;
static uint16_t audio_string_color;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void core0_irq_init(void);
static void core1_irq_init(void);
static void core0_icc(int enable);
static void core1_icc(int enable);
static void run_application(void);
static int refresh_screen(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int main(void)
{
    int ret = 0;

    // Set PORT1 and PORT2 rail to VDDIO
    MXC_GPIO0->vssel = 0x00;
    MXC_GPIO1->vssel = 0x00;

    // Disable Core0 ICC0 Instruction cache
    core0_icc(0);

    ret = MXC_SEMA_Init();
    if (ret != E_NO_ERROR) {
        printf("MXC_SEMA_Init failed %d\n", ret);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    device_info.device_version.max32666.major = S_VERSION_MAJOR;
    device_info.device_version.max32666.minor = S_VERSION_MINOR;
    device_info.device_version.max32666.build = S_VERSION_BUILD;
    snprintf(version_string, sizeof(version_string) - 1, "v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    snprintf(device_info.max32666_demo_name, sizeof(device_info.max32666_demo_name) - 1, "%s", WILDLIFE_DEMO_NAME);
    PR("\n\n\n");
    PR_INFO("maxrefdes178_max32666 %s core0 %s [%s]", device_info.max32666_demo_name, version_string, S_BUILD_TIMESTAMP);

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
    pmic_led_green(1);

   // BLE should init first since it is mischievous
   // BLE init somehow damages GPIO settings for P0.0, P0.23
    core0_irq_init();
    Core1_Start();

    for(uint32_t cnt = 10000000; !core1_init_done && cnt; cnt--) {
        if (cnt == 1) {
            PR_ERROR("timeout, reset");
            MXC_Delay(MXC_DELAY_MSEC(100));
            pmic_hard_reset();
            MXC_Delay(MXC_DELAY_MSEC(100));
            MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
        }
    }

    ret = fuel_gauge_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("fuel_gauge_init failed %d", ret);
        pmic_led_red(1);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    // Initialize DMA peripheral
    ret = MXC_DMA_Init(MXC_DMA0);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_DMA_Init failed %d", ret);
        pmic_led_red(1);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

//    ret = ext_sram_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("ext_sram_init failed %d", ret);
//        pmic_led_red(1);
//    }

    ret = lcd_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("lcd_init failed %d", ret);
        pmic_led_red(1);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    ret = qspi_master_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("qspi_master_init failed %d", ret);
        pmic_led_red(1);
        MXC_Delay(MXC_DELAY_MSEC(100));
        MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
    }

    ret = timer_led_button_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("timer_led_button_init failed %d", ret);
        pmic_led_red(1);
    }

    ret = accel_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("accel_init failed %d", ret);
        pmic_led_red(1);
    }

    ret = touch_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("touch_init failed %d", ret);
        pmic_led_red(1);
    }

//    ret = powmon_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("powmon_init failed %d", ret);
//        pmic_led_red(1);
//    }
//
//    ret = usb_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("usb_init failed %d", ret);
//        pmic_led_red(1);
//    }
//
//    ret = ext_flash_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("ext_flash_init failed %d", ret);
//        pmic_led_red(1);
//    }
//
//    ret = audio_codec_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("audio_codec_init failed %d", ret);
//        pmic_led_red(1);
//    }
//
//    ret = sdcard_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("sdcard_init failed %d", ret);
//        pmic_led_red(1);
//    }
//
//    ret = ble_queue_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("ble_queue_init failed %d", ret);
//        pmic_led_red(1);
//    }
//
//    ret = ble_command_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("ble_command_init failed %d", ret);
//        pmic_led_red(1);
//    }

	//ret = MXC_SYS_GetUSN(device_info.device_serial_num.max32666, sizeof(device_info.device_serial_num.max32666));
	uint8_t checksum[MXC_SYS_USN_CHECKSUM_LEN];
    ret = MXC_SYS_GetUSN(device_info.device_serial_num.max32666, checksum);

    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_SYS_GetUSN failed %d", ret);
        pmic_led_red(1);
    }
    snprintf(usn_string, sizeof(usn_string) - 1, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            device_info.device_serial_num.max32666[0],
            device_info.device_serial_num.max32666[1],
            device_info.device_serial_num.max32666[2],
            device_info.device_serial_num.max32666[3],
            device_info.device_serial_num.max32666[4],
            device_info.device_serial_num.max32666[5],
            device_info.device_serial_num.max32666[6],
            device_info.device_serial_num.max32666[7],
            device_info.device_serial_num.max32666[8],
            device_info.device_serial_num.max32666[9],
            device_info.device_serial_num.max32666[10],
            device_info.device_serial_num.max32666[11],
            device_info.device_serial_num.max32666[12]);
    PR_INFO("MAX32666 Serial number: %s", usn_string);

//    snprintf(mac_string, sizeof(mac_string) - 1, "%02X:%02X:%02X:%02X:%02X:%02X",
//            device_info.ble_mac[5], device_info.ble_mac[4], device_info.ble_mac[3],
//            device_info.ble_mac[2], device_info.ble_mac[1], device_info.ble_mac[0]);
//    PR_INFO("MAX32666 BLE MAC: %s", mac_string);

    /* Select USB-Type-C Debug Connection to MAX78000-Video on IO expander */
    if ((ret = expander_select_debugger(DEBUGGER_SELECT_MAX78000_VIDEO)) != E_NO_ERROR) {
       PR_ERROR("expander_debug_select failed %d", ret);
       pmic_led_red(1);
    }

    // Print logo and version
    fonts_putStringCentered(LCD_HEIGHT - 66, version_string, &Font_16x26, GRED, adi_logo);
    fonts_putStringCentered(LCD_HEIGHT - 38, mac_string, &Font_11x18, BLUE, adi_logo);
    fonts_putStringCentered(3, usn_string, &Font_7x10, LGRAY, adi_logo); //change to light grey to match the new background
    fonts_putStringCentered(55, device_info.max32666_demo_name, &Font_16x26, MAGENTA, adi_logo);
    lcd_drawImage(adi_logo);

    // Wait MAX78000s
    MXC_Delay(MXC_DELAY_MSEC(3000));

    // Get information from MAX78000
    {
        qspi_packet_type_e qspi_packet_type_rx = 0;
        for (int try = 0; try < 3; try++) {
            qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_FACEID_SUBJECTS_CMD);
            qspi_master_wait_video_int();
            qspi_master_video_rx_worker(&qspi_packet_type_rx);
            if (device_status.faceid_embed_subject_names_size) {
                break;
            }
        }

        for (int try = 0; try < 3; try++) {
            qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_VERSION_CMD);
            qspi_master_wait_video_int();
            qspi_master_video_rx_worker(&qspi_packet_type_rx);
            if (device_info.device_version.max78000_video.major || device_info.device_version.max78000_video.minor) {
                break;
            }
        }
        for (int try = 0; try < 3; try++) {
            qspi_master_send_audio(NULL, 0, QSPI_PACKET_TYPE_AUDIO_VERSION_CMD);
            qspi_master_wait_audio_int();
            qspi_master_audio_rx_worker(&qspi_packet_type_rx);
            if (device_info.device_version.max78000_audio.major || device_info.device_version.max78000_audio.minor) {
                break;
            }
        }

        for (int try = 0; try < 3; try++) {
            qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_DEMO_NAME_CMD);
            qspi_master_wait_video_int();
            qspi_master_video_rx_worker(&qspi_packet_type_rx);
            if (device_info.max78000_video_demo_name[0]) {
                break;
            }
        }
        for (int try = 0; try < 3; try++) {
            qspi_master_send_audio(NULL, 0, QSPI_PACKET_TYPE_AUDIO_DEMO_NAME_CMD);
            qspi_master_wait_audio_int();
            qspi_master_audio_rx_worker(&qspi_packet_type_rx);
            if (device_info.max78000_audio_demo_name[0]) {
                break;
            }
        }

        ret = E_NO_ERROR;
        // Check video and audio fw version
        if (!(device_info.device_version.max78000_video.major || device_info.device_version.max78000_video.minor)) {
            PR_ERROR("max78000_video communication error");
            ret = E_COMM_ERR;
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "No video comm");
            fonts_putStringCentered(100, lcd_string_buff, &Font_11x18, RED, adi_logo);
        } else if (memcmp(&device_info.device_version.max32666, &device_info.device_version.max78000_video, sizeof(version_t))) {
            PR_ERROR("max32666 and max78000_video versions are different");
            ret = E_INVALID;
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "video fw err %d.%d.%d",
                    device_info.device_version.max78000_video.major,
                    device_info.device_version.max78000_video.minor,
                    device_info.device_version.max78000_video.build);
            fonts_putStringCentered(100, lcd_string_buff, &Font_11x18, RED, adi_logo);
        } else if (strncmp(device_info.max32666_demo_name, device_info.max78000_video_demo_name, DEMO_STRING_SIZE)) {
            PR_ERROR("max32666 and max78000_video demos are different");
            ret = E_INVALID;
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "video fw demo err %s", device_info.max78000_video_demo_name);
            fonts_putStringCentered(100, lcd_string_buff, &Font_11x18, RED, adi_logo);
        }

        if (!(device_info.device_version.max78000_audio.major || device_info.device_version.max78000_audio.minor)) {
            PR_ERROR("max78000_audio communication error");
            ret = E_COMM_ERR;
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "No audio comm");
            fonts_putStringCentered(130, lcd_string_buff, &Font_11x18, RED, adi_logo);
        } else if (memcmp(&device_info.device_version.max32666, &device_info.device_version.max78000_audio, sizeof(version_t))) {
            PR_ERROR("max32666 and max78000_audio versions are different");
            ret = E_INVALID;
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "audio fw err %d.%d.%d",
                    device_info.device_version.max78000_audio.major,
                    device_info.device_version.max78000_audio.minor,
                    device_info.device_version.max78000_audio.build);
            fonts_putStringCentered(130, lcd_string_buff, &Font_11x18, RED, adi_logo);
        } else if (strncmp(device_info.max32666_demo_name, device_info.max78000_audio_demo_name, DEMO_STRING_SIZE)) {
            PR_ERROR("max32666 and max78000_audio demos are different");
            ret = E_INVALID;
            snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "audio fw demo err %s", device_info.max78000_audio_demo_name);
            fonts_putStringCentered(130, lcd_string_buff, &Font_11x18, RED, adi_logo);
        }

        if (ret != E_NO_ERROR) {
            lcd_drawImage(adi_logo);
            pmic_led_red(1);
            while(1) {
//                if (device_settings.enable_ble && device_status.ble_connected) {
//                    ble_command_worker();
//                }
                expander_worker();

                if (lcd_data.refresh_screen && !spi_dma_busy_flag(MAX32666_LCD_DMA_CHANNEL)) {
                    memcpy(lcd_data.buffer, adi_logo, sizeof(lcd_data.buffer));
                    if (strlen(lcd_data.notification) < (LCD_WIDTH / Font_11x18.width)) {
                        fonts_putStringCentered(LCD_HEIGHT - Font_11x18.height - 3, lcd_data.notification, &Font_11x18, lcd_data.notification_color, lcd_data.buffer);
                    } else {
                        fonts_putStringCentered(LCD_HEIGHT - Font_7x10.height - 3, lcd_data.notification, &Font_7x10, lcd_data.notification_color, lcd_data.buffer);
                    }
                    lcd_drawImage(lcd_data.buffer);
                }
            }
        }
    }

    PR_INFO("core 0 init completed");

    // print application name
    snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "Audio enabled");
    fonts_putStringCentered(37, lcd_string_buff, &Font_11x18, RED, adi_logo);

    run_application();

    return E_NO_ERROR;
}

static void run_application(void)
{
    qspi_packet_type_e qspi_packet_type_rx = 0;
    video_frame_color = WHITE;
    uint16_t touch_x1, touch_y1;

    core0_icc(1);

    // Main application loop
    while (1) {

        // Handle Video QSPI RX
        if (qspi_master_video_rx_worker(&qspi_packet_type_rx) == E_NO_ERROR) {
            switch(qspi_packet_type_rx) {
            case QSPI_PACKET_TYPE_VIDEO_DATA_RES:
                timestamps.video_data_received = timer_ms_tick;
                lcd_data.refresh_screen = 1;
                break;
            case QSPI_PACKET_TYPE_VIDEO_CLASSIFICATION_RES:
                timestamps.activity_detected = timer_ms_tick;
                if (device_status.classification_video.classification == CLASSIFICATION_UNKNOWN) {
                    video_string_color = RED;
                    video_frame_color = RED;
                } else if (device_status.classification_video.classification == CLASSIFICATION_LOW_CONFIDENCE) {
                    video_string_color = YELLOW;
                    video_frame_color = YELLOW;
                } else if (device_status.classification_video.classification == CLASSIFICATION_DETECTED) {
                    video_string_color = GREEN;
                    video_frame_color = GREEN;
                } else if (device_status.classification_video.classification == CLASSIFICATION_NOTHING) {
                    video_frame_color = WHITE;
                }

//                if (device_settings.enable_ble_send_classification && device_status.ble_connected) {
//                    ble_command_send_single_packet(BLE_COMMAND_GET_MAX78000_VIDEO_CLASSIFICATION_RES,
//                        sizeof(device_status.classification_video), (uint8_t *) &device_status.classification_video);
//                }
                break;
            case QSPI_PACKET_TYPE_VIDEO_FACEID_EMBED_UPDATE_RES:
//                if (device_status.ble_connected) {
//                    ble_command_send_single_packet(BLE_COMMAND_FACEID_EMBED_UPDATE_RES,
//                        sizeof(device_status.faceid_embed_update_status), (uint8_t *) &device_status.faceid_embed_update_status);
//                }
                qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_FACEID_SUBJECTS_CMD);
                lcd_notification(GREEN, "FaceID signature updated");
                break;
            case QSPI_PACKET_TYPE_VIDEO_FACEID_SUBJECTS_RES:
                timestamps.faceid_subject_names_received = timer_ms_tick;
                break;
            default:
                break;
            }
        }


       // PR_INFO("LCD:%d VIDEO:%d CNN:%d AUDIO:%d",device_settings.enable_lcd, device_settings.enable_max78000_video
       // 		,device_settings.enable_max78000_video_cnn, device_settings.enable_max78000_audio);
        // Handle Audio QSPI RX
        if (qspi_master_audio_rx_worker(&qspi_packet_type_rx) == E_NO_ERROR) {
            switch(qspi_packet_type_rx) {
            case QSPI_PACKET_TYPE_AUDIO_CLASSIFICATION_RES:
                timestamps.audio_result_received = timer_ms_tick;
                timestamps.activity_detected = timer_ms_tick;

                if (device_status.classification_audio.classification == CLASSIFICATION_UNKNOWN) {
                    audio_string_color = RED;
                } else if (device_status.classification_audio.classification == CLASSIFICATION_LOW_CONFIDENCE) {
                    audio_string_color = YELLOW;
                } else if (device_status.classification_audio.classification == CLASSIFICATION_DETECTED) {
                    audio_string_color = YELLOW;//GREEN;

                    if (strncmp(device_status.classification_audio.result, "OFF", 3) == 0) {
                    	PR_INFO("OFF");
                        //  device_settings.enable_lcd = 0;
                        //  lcd_backlight(0, 0);
                        //  qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_DISABLE_CMD);
                    } else if(strncmp(device_status.classification_audio.result, "ON", 2) == 0) {
                    	PR_INFO("ON");
                        device_settings.enable_lcd = 1;
                        if (device_settings.enable_max78000_video) {
                            qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_ENABLE_CMD);
                        }
                        lcd_backlight(1, MAX32666_LCD_BACKLIGHT_HIGH);

                        // also enable cnn
                        device_settings.enable_max78000_video_cnn = 1;
                        //  qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_ENABLE_CNN_CMD);

                    } else if (strncmp(device_status.classification_audio.result, "GO", 2) == 0) {
                    	PR_INFO("GO");
                    	device_settings.enable_max78000_video_cnn = 1;
                        //  qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_ENABLE_CNN_CMD);
                    } else if(strncmp(device_status.classification_audio.result, "STOP", 4) == 0) {
                    	PR_INFO("STOP");
                        //  device_settings.enable_max78000_video_cnn = 0;
                        //  qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_DISABLE_CNN_CMD);

                    }
                }

//                if (device_settings.enable_ble_send_classification && device_status.ble_connected) {
//                    ble_command_send_single_packet(BLE_COMMAND_GET_MAX78000_AUDIO_CLASSIFICATION_RES,
//                        sizeof(device_status.classification_audio), (uint8_t *) &device_status.classification_audio);
//                }
                if (!device_settings.enable_max78000_video) {
                    lcd_data.refresh_screen = 1;
                }
                break;
            default:
                break;
            }
        }

        // Handle QSPI TX
        qspi_master_video_tx_worker();
        qspi_master_audio_tx_worker();

//        // Send BLE periodic statistics
//        if (device_settings.enable_ble_send_statistics && device_status.ble_connected) {
//            if ((timer_ms_tick - timestamps.statistics_sent) > BLE_STATISTICS_INTERVAL) {
//                timestamps.statistics_sent = timer_ms_tick;
//                ble_command_send_single_packet(BLE_COMMAND_GET_STATISTICS_RES,
//                    sizeof(device_status.statistics), (uint8_t *) &device_status.statistics);
//            }
//        }

        if (device_settings.enable_max78000_video) {
            // If video is not available for a long time, draw logo and refresh periodically
            if ((timer_ms_tick - timestamps.video_data_received) > LCD_NO_VIDEO_REFRESH_DURATION) {
                timestamps.video_data_received = timer_ms_tick;
                memcpy(lcd_data.buffer, adi_logo, sizeof(lcd_data.buffer));
                snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "No video!");
                fonts_putStringCentered(16, lcd_string_buff, &Font_11x18, RED, lcd_data.buffer);
                lcd_data.refresh_screen = 1;
            }
        } else {
            // If video is disabled, draw logo and refresh periodically
            if ((timer_ms_tick - timestamps.screen_drew) > LCD_VIDEO_DISABLE_REFRESH_DURATION) {
                memcpy(lcd_data.buffer, adi_logo, sizeof(lcd_data.buffer));
                snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "Video disabled");
                fonts_putStringCentered(15, lcd_string_buff, &Font_11x18, RED, lcd_data.buffer);
                lcd_data.refresh_screen = 1;
            }
        }

//        // Handle BLE communication
//        if (device_settings.enable_ble && device_status.ble_connected) {
//            ble_command_worker();
//        }
//
//        // Check if BLE status changed
//        if (device_status.ble_connected_status_changed) {
//            device_status.ble_connected_status_changed = 0;
//
//            ble_queue_flush();
//            ble_command_reset();
//            device_settings.enable_ble_send_classification = 0;
//            device_settings.enable_ble_send_statistics = 0;
//
//            if (device_status.ble_connected) {
//                snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1,
//                        "BLE %02X:%02X:%02X:%02X:%02X:%02X connected!",
//                        device_status.ble_connected_peer_mac[5], device_status.ble_connected_peer_mac[4],
//                        device_status.ble_connected_peer_mac[3], device_status.ble_connected_peer_mac[2],
//                        device_status.ble_connected_peer_mac[1], device_status.ble_connected_peer_mac[0]);
//                lcd_notification(BLUE, lcd_string_buff);
//            } else {
//                lcd_notification(BLUE, "BLE disconnected!");
//            }
//        }
//
//        if (device_status.ble_running_status_changed) {
//            if (device_settings.enable_ble) {
//                PR_INFO("Enable Core1");
//                Core1_Start();
//            } else {
//                PR_INFO("Disable Core1");
//                Core1_Stop();
//            }
//            device_status.ble_running_status_changed = 0;
//        }

        // Check inactivity
        if (device_settings.enable_inactivity) {
            if ((timer_ms_tick - timestamps.activity_detected) > INACTIVITY_LONG_DURATION) {
                if (device_status.inactivity_state != INACTIVITY_STATE_INACTIVE_LONG) {
                    device_status.inactivity_state = INACTIVITY_STATE_INACTIVE_LONG;
                    // Switch to inactive long state
                    lcd_backlight(0, 0);
                    qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_DISABLE_CMD);
                    PR_INFO("Inactive long");
                }
            } else if ((timer_ms_tick - timestamps.activity_detected) > INACTIVITY_SHORT_DURATION) {
                if (device_status.inactivity_state != INACTIVITY_STATE_INACTIVE_SHORT) {
                    device_status.inactivity_state = INACTIVITY_STATE_INACTIVE_SHORT;
                    // Switch to inactive short state
                    if (device_settings.enable_lcd) {
                        lcd_backlight(1, MAX32666_LCD_BACKLIGHT_LOW);
                    }
                    PR_INFO("Inactive short");
                }
            } else {
                if (device_status.inactivity_state != INACTIVITY_STATE_ACTIVE) {
                    device_status.inactivity_state = INACTIVITY_STATE_ACTIVE;
                    // Switch to active state
                    if (device_settings.enable_lcd) {
                        lcd_backlight(1, MAX32666_LCD_BACKLIGHT_HIGH);
                    }
                    if (device_settings.enable_max78000_video && device_settings.enable_lcd) {
                        MXC_Delay(MXC_DELAY_MSEC(600));
                        qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_ENABLE_CMD);
                    }
                    PR_INFO("Active");
                }
            }
        }

        // Check PMIC and Fuel Gauge
        if ((timer_ms_tick - timestamps.pmic_check) > MAX32666_PMIC_INTERVAL) {
            timestamps.pmic_check = timer_ms_tick;
            pmic_worker();
            if (device_status.fuel_gauge_working) {
                fuel_gauge_worker();
            }
        }

        // Power accumulator worker
//        if ((timer_ms_tick - timestamps.powmon) > MAX32666_POWMON_INTERVAL) {
//            timestamps.powmon = timer_ms_tick;
//            powmon_worker();
//        }

        // LED worker
        if ((timer_ms_tick - timestamps.led) > MAX32666_LED_INTERVAL) {
            timestamps.led = timer_ms_tick;
            led_worker();
        }

        // IO expander worker
        expander_worker();

        // Touch screen worker
        if (touch_worker(&touch_x1, &touch_y1) == E_NO_ERROR) {

            // Check if init page start button is clicked
            if (device_settings.enable_max78000_video == 0) {
                if ((LCD_START_BUTTON_X1 <= touch_x1) && (touch_x1 <= LCD_START_BUTTON_X2) &&
                    (LCD_START_BUTTON_Y1 <= touch_y1) && (touch_y1 <= LCD_START_BUTTON_Y2)) {
                    device_settings.enable_max78000_video = 1;
                    qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_ENABLE_CMD);
                    PR_INFO("start button clicked");

                    // Start video
                    MXC_Delay(MXC_DELAY_MSEC(1000));
                    device_settings.enable_max78000_video_cnn = 1;
                   // qspi_master_send_video(NULL, 0, QSPI_PACKET_TYPE_VIDEO_ENABLE_CNN_CMD);
                }
            }

            PR_INFO("touch %d %d", touch_x1, touch_y1);
            timestamps.activity_detected = timer_ms_tick;
        }

        // Button worker
        button_worker();

        // USB worker
//        usb_worker();

        // Refresh LCD
        if (lcd_data.refresh_screen && device_settings.enable_lcd && !spi_dma_busy_flag(MAX32666_LCD_DMA_CHANNEL)) {
            refresh_screen();
        }

        // Sleep until an interrupt
        __WFI();
    }
}

static int refresh_screen(void)
{
    if (device_status.fuel_gauge_working) {
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%3d%%", device_status.statistics.battery_soc);
        if (device_status.usb_chgin) {
            fonts_putString(LCD_WIDTH - 31, 3, lcd_string_buff, &Font_7x10, ORANGE, 0, 0, lcd_data.buffer);
        } else if (device_status.statistics.battery_soc <= MAX32666_SOC_WARNING_LEVEL) {
            fonts_putString(LCD_WIDTH - 31, 3, lcd_string_buff, &Font_7x10, RED, 0, 0, lcd_data.buffer);
        } else {
            fonts_putString(LCD_WIDTH - 31, 3, lcd_string_buff, &Font_7x10, GREEN, 0, 0, lcd_data.buffer);
        }
    }

    // Draw FaceID frame and result
    if (device_settings.enable_max78000_video && device_settings.enable_max78000_video_cnn) {
        if (device_status.classification_video.classification != CLASSIFICATION_NOTHING) {
            strncpy(lcd_string_buff, device_status.classification_video.result, sizeof(lcd_string_buff) - 1);
            fonts_putStringCentered(LCD_HEIGHT - 29, lcd_string_buff, &Font_16x26, video_string_color, lcd_data.buffer);
        }
        fonts_drawRectangle(WILDLIFE_RECTANGLE_X1 - 0, WILDLIFE_RECTANGLE_Y1 - 0, WILDLIFE_RECTANGLE_X2 + 0, WILDLIFE_RECTANGLE_Y2 + 0, video_frame_color, lcd_data.buffer);
        fonts_drawRectangle(WILDLIFE_RECTANGLE_X1 - 1, WILDLIFE_RECTANGLE_Y1 - 1, WILDLIFE_RECTANGLE_X2 + 1, WILDLIFE_RECTANGLE_Y2 + 1, video_frame_color, lcd_data.buffer);
        fonts_drawRectangle(WILDLIFE_RECTANGLE_X1 - 2, WILDLIFE_RECTANGLE_Y1 - 2, WILDLIFE_RECTANGLE_X2 + 2, WILDLIFE_RECTANGLE_Y2 + 2, BLACK, lcd_data.buffer);
        fonts_drawRectangle(WILDLIFE_RECTANGLE_X1 - 3, WILDLIFE_RECTANGLE_Y1 - 3, WILDLIFE_RECTANGLE_X2 + 3, WILDLIFE_RECTANGLE_Y2 + 3, BLACK, lcd_data.buffer);
    }

    if (device_settings.enable_lcd_statistics) {
        int line_pos = 3;

        // LCD frame per seconds
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "FPS:%.2f", (double)device_status.statistics.lcd_fps);
        fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
        line_pos += 12;

        // FaceID duration (MAX78000 Video CNN + embeddings calculation)
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "Forest:%d us", device_status.statistics.max78000_video.cnn_duration_us);
        fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
        line_pos += 12;

        // KWS duration (MAX78000 Audio CNN)
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "KWS:%d us", device_status.statistics.max78000_audio.cnn_duration_us);
        fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
        line_pos += 12;

        // Video camera capture duration (frame capture)
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "VidCap:%d ms", device_status.statistics.max78000_video.capture_duration_us / 1000);
        fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
        line_pos += 12;

        // Video communication duration (frame transfer from MAX78000 to MAX32666 over QSPI)
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "VidComm:%d ms", device_status.statistics.max78000_video.communication_duration_us / 1000);
        fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
        line_pos += 12;

//        // MAX78000 Video power
//        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "Vid:%d mW", device_status.statistics.max78000_video_cnn_power_mw);
//        fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
//        line_pos += 12;
//
//        // MAX78000 Audio power
//        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "Aud:%d mW", device_status.statistics.max78000_audio_cnn_power_mw);
//        fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
//        line_pos += 12;

        if ((timestamps.screen_drew - timestamps.faceid_subject_names_received) < LCD_NOTIFICATION_DURATION) {
            line_pos += 5;
            for (int i = 0; i < device_status.faceid_embed_subject_names_size; i += strlen(&device_status.faceid_embed_subject_names[i]) + 1) {
                snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%s", &device_status.faceid_embed_subject_names[i]);
                fonts_putString(3, line_pos, lcd_string_buff, &Font_7x10, CYAN, 0, 0, lcd_data.buffer);
                line_pos += 12;
            }
        }
    }

    // Draw button in init screen
    if (device_settings.enable_max78000_video == 0) {
        // Start button
        fonts_drawFilledRectangle(LCD_START_BUTTON_X1, LCD_START_BUTTON_Y1, LCD_START_BUTTON_X2 - LCD_START_BUTTON_X1,
                                  LCD_START_BUTTON_Y2 - LCD_START_BUTTON_Y1, LGRAY, lcd_data.buffer);
        fonts_drawThickRectangle(LCD_START_BUTTON_X1, LCD_START_BUTTON_Y1, LCD_START_BUTTON_X2, LCD_START_BUTTON_Y2, LIGHTBLUE, 4, lcd_data.buffer);
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "Start Video");
        fonts_putStringCentered(LCD_START_BUTTON_Y1 + 10, lcd_string_buff, &Font_16x26, ADIBLUE, lcd_data.buffer);
    }

    if (device_settings.enable_max78000_audio) {
        if ((timestamps.screen_drew - timestamps.audio_result_received) < LCD_CLASSIFICATION_DURATION) {
            if (device_settings.enable_lcd_probabilty) {
                snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "%s (%d%%)",
                        device_status.classification_audio.result, (uint8_t) device_status.classification_audio.probability);
            } else {
                strncpy(lcd_string_buff, device_status.classification_audio.result, sizeof(lcd_string_buff) - 1);
            }

            fonts_putStringCentered(3, lcd_string_buff, &Font_16x26, audio_string_color, lcd_data.buffer);
        }
    } else {
        snprintf(lcd_string_buff, sizeof(lcd_string_buff) - 1, "Audio disabled");
        fonts_putStringCentered(3, lcd_string_buff, &Font_11x18, RED, lcd_data.buffer);
    }

    if ((timestamps.screen_drew - timestamps.notification_received) < LCD_NOTIFICATION_DURATION) {
        if (strlen(lcd_data.notification) < (LCD_WIDTH / Font_11x18.width)) {
            fonts_putStringCentered(LCD_HEIGHT - Font_11x18.height - 3, lcd_data.notification, &Font_11x18, lcd_data.notification_color, lcd_data.buffer);
        } else {
            fonts_putStringCentered(LCD_HEIGHT - Font_7x10.height - 3, lcd_data.notification, &Font_7x10, lcd_data.notification_color, lcd_data.buffer);
        }
    }

    if (lcd_drawImage(lcd_data.buffer) == E_NO_ERROR) {
        device_status.statistics.lcd_fps = (float) 1000.0 / (float)(timer_ms_tick - timestamps.screen_drew);
        timestamps.screen_drew = timer_ms_tick;
    }

    return E_NO_ERROR;
}

// Similar to Core 0, the entry point for Core 1
// is Core1Main()
// Execution begins when the CPU1 Clock is enabled
int Core1_Main(void)
{
    //  __asm__("BKPT");

    // Disable Core1 ICC1 Instruction cache
    core1_icc(0);

//    int ret = 0;

    PR_INFO("maxrefdes178_max32666 core1");

    core1_irq_init();

//    ret = ble_init();
//    if (ret != E_NO_ERROR) {
//        PR_ERROR("ble_init %d", ret);
//    }

    core1_init_done = 1;

    PR_INFO("core 1 init completed");

//    core1_icc(1);

    while (1) {
//        ble_worker();
    }

    return E_NO_ERROR;
}

static void core0_irq_init(void)
{
    // Disable all interrupts used by core1
//    NVIC_DisableIRQ(SysTick_IRQn);

    NVIC_DisableIRQ(BTLE_TX_DONE_IRQn);
    NVIC_DisableIRQ(BTLE_RX_RCVD_IRQn);
    NVIC_DisableIRQ(BTLE_RX_ENG_DET_IRQn);
    NVIC_DisableIRQ(BTLE_SFD_DET_IRQn);
    NVIC_DisableIRQ(BTLE_SFD_TO_IRQn);
    NVIC_DisableIRQ(BTLE_GP_EVENT_IRQn);
    NVIC_DisableIRQ(BTLE_CFO_IRQn);
    NVIC_DisableIRQ(BTLE_SIG_DET_IRQn);
    NVIC_DisableIRQ(BTLE_AGC_EVENT_IRQn); // Disabled
    NVIC_DisableIRQ(BTLE_RFFE_SPIM_IRQn);
    NVIC_DisableIRQ(BTLE_TX_AES_IRQn); // Disabled
    NVIC_DisableIRQ(BTLE_RX_AES_IRQn); // Disabled
    NVIC_DisableIRQ(BTLE_INV_APB_ADDR_IRQn); // Disabled
    NVIC_DisableIRQ(BTLE_IQ_DATA_VALID_IRQn); // Disabled

    NVIC_DisableIRQ(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_BLE)));
    NVIC_DisableIRQ(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_BLE_SLEEP)));

    NVIC_DisableIRQ(WUT_IRQn);
}

static void core1_irq_init(void)
{
//    NVIC_DisableIRQ(SysTick_IRQn);

    NVIC_DisableIRQ(GPIO0_IRQn);
    NVIC_DisableIRQ(GPIO1_IRQn);

    NVIC_DisableIRQ(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX32666_TIMER_MS)));
}

static void core0_icc(int enable)
{
    MXC_ICC0->invalidate = 1;
    while (!(MXC_ICC0->cache_ctrl & MXC_F_ICC_CACHE_CTRL_RDY));
    if (enable) {
        MXC_ICC0->cache_ctrl |= MXC_F_ICC_CACHE_CTRL_EN;
    } else {
        MXC_ICC0->cache_ctrl &= ~MXC_F_ICC_CACHE_CTRL_EN;
    }
    while (!(MXC_ICC0->cache_ctrl & MXC_F_ICC_CACHE_CTRL_RDY));
}

static void core1_icc(int enable)
{
    MXC_ICC1->invalidate = 1;
    while (!(MXC_ICC1->cache_ctrl & MXC_F_ICC_CACHE_CTRL_RDY));
    if (enable) {
        MXC_ICC1->cache_ctrl |= MXC_F_ICC_CACHE_CTRL_EN;
    } else {
        MXC_ICC1->cache_ctrl &= ~MXC_F_ICC_CACHE_CTRL_EN;
    }
    while (!(MXC_ICC1->cache_ctrl & MXC_F_ICC_CACHE_CTRL_RDY));
}
