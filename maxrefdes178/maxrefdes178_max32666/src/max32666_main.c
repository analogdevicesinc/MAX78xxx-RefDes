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
#include <mxc_delay.h>
#include <mxc_sys.h>
#include <rtc.h>
#include <stdint.h>
#include <string.h>

#include "max32666_ble.h"
#include "max32666_commbuf.h"
#include "max32666_commhandler.h"
#include "max32666_data.h"
#include "max32666_debug.h"
#include "max32666_expander.h"
#include "max32666_fonts.h"
#include "max32666_i2c.h"
#include "max32666_lcd.h"
#include "max32666_lcd_images.h"
#include "max32666_led_button.h"
#include "max32666_max20303.h"
#include "max32666_qspi.h"
#include "max32666_sdcard.h"
#include "max32666_utils.h"
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


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void core0_irq_init(void);
static void core1_irq_init(void);
static void run_application(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int main(void)
{
    int ret = 0;
    char version_string[10] = {0};

    // Set PORT1 and PORT2 rail to VDDIO
    MXC_GPIO0->vssel =  0x00;
    MXC_GPIO1->vssel =  0x00;

    ret = MXC_SEMA_Init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_SEMA_Init failed %d", ret);
        while(1);
    }

    device_info.device_version.max32666.major = S_VERSION_MAJOR;
    device_info.device_version.max32666.minor = S_VERSION_MINOR;
    device_info.device_version.max32666.build = S_VERSION_BUILD;
    snprintf(version_string, sizeof(version_string) - 1, "v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    PR_INFO("maxrefdes178_max32666 core0 %s [%s]", version_string, S_BUILD_TIMESTAMP);

    ret = i2c_master_init(MAX32666_I2C, I2C_SPEED);
    if (ret != E_NO_ERROR) {
        PR_ERROR("i2c_init failed %d", ret);
        while(1);
    }

    ret = max20303_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("max20303_init failed %d", ret);
        while(1);
    }
    max20303_led_green(1);

   // BLE should init first since it is mischievous
   // BLE init somehow damages GPIO settings for P0.0, P0.23
    core0_irq_init();
    Core1_Start();

    for(uint32_t cnt = 20000000; !core1_init_done && cnt; cnt--) {
        if (cnt == 1) {
            PR_ERROR("timeout, reset");
            MXC_Delay(MXC_DELAY_MSEC(100));
            MXC_SYS_Reset_Periph(MXC_SYS_RESET_SYSTEM);
//            MXC_GCR->rstr0 = 0xffffffff;
        }
    }

    ret = MXC_SEMA_Init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_SEMA_Init failed %d", ret);
        while(1);
    }

    ret = expander_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("expander_init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    /* Select USB-TYpe-C Debug Connection to MAX78000-Video on IO expander */
    ret = expander_debug_select(eExpanderDebugVideo);
    if (ret != E_NO_ERROR) {
        PR_ERROR("expander_debug_select failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    // Initialize DMA peripheral
    ret = MXC_DMA_Init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_DMA_Init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    ret = lcd_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("lcd_init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    ret = qspi_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("qspi_init failed %d", ret);
        max20303_led_red(1);
        while(1);
    }

    ret = sdcard_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("sdcard_init failed %d", ret);
//        max20303_led_red(1);
    }

    ret = led_button_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("led_button_init failed %d", ret);
        max20303_led_red(1);
    }

    ret = commbuf_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("commbuf_init failed %d", ret);
        max20303_led_red(1);
    }

    ret = commhandler_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("commhandler_init failed %d", ret);
        max20303_led_red(1);
    }

    ret = MXC_RTC_Init(0, 0);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_RTC_Init failed %d", ret);
        max20303_led_red(1);
    }

    ret = MXC_RTC_Start();
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_RTC_Start failed %d", ret);
        max20303_led_red(1);
    }

    ret = MXC_SYS_GetUSN(device_info.device_serial_num.max32666, sizeof(device_info.device_serial_num.max32666));
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_SYS_GetUSN failed %d", ret);
        max20303_led_red(1);
    }
    PR_INFO("MAX32666 Serial number: ");
    for (int i = 0; i < sizeof(device_info.device_serial_num.max32666); i++) {
        PR("%02X", device_info.device_serial_num.max32666[i]);
    }
    PR("\n");

    // TODO: get max78000_video and max78000_audio version and serial num

    // Print logo and version
    fonts_putSubtitle(LCD_WIDTH, LCD_HEIGHT, version_string, Font_16x26, RED, maxim_logo);
    lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, maxim_logo);

    PR_INFO("core 0 init completed");

    run_application();

    return -1;
}

static void run_application(void)
{
    uint32_t audio_result_time = 0;
    uint32_t lcd_draw_time = 0;
    qspi_packet_type_e qspi_packet_type = 0;
    char fps_string[10] = {0};
    packet_container_t ble_packet_container = {0};
    lcd_data.toptitle_color = YELLOW;

    while (1) {
        // Handle QSPI RX
        if (qspi_worker(&qspi_packet_type) == QSPI_STATUS_NEW_DATA) {
            switch(qspi_packet_type) {
            case QSPI_PACKET_TYPE_VIDEO_DATA_RES:
                if (device_settings.enable_max78000_video_cnn) {
                    if (device_status.classification_video.classification != CLASSIFICATION_NOTHING) {
                        // if (device_settings.enable_show_probabilty_lcd)
                        strncpy(lcd_data.subtitle, device_status.classification_video.result, sizeof(lcd_data.subtitle) - 1);
                        fonts_putSubtitle(LCD_WIDTH, LCD_HEIGHT, lcd_data.subtitle, Font_16x26, lcd_data.subtitle_color, lcd_data.buffer);
                    }

                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 0, FACEID_RECTANGLE_Y1 - 0,
                            FACEID_RECTANGLE_X2 + 0, FACEID_RECTANGLE_Y2 + 0, lcd_data.frame_color, lcd_data.buffer);
                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 1, FACEID_RECTANGLE_Y1 - 1,
                            FACEID_RECTANGLE_X2 + 1, FACEID_RECTANGLE_Y2 + 1, lcd_data.frame_color, lcd_data.buffer);
                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 2, FACEID_RECTANGLE_Y1 - 2,
                            FACEID_RECTANGLE_X2 + 2, FACEID_RECTANGLE_Y2 + 2, BLACK, lcd_data.buffer);
                    fonts_drawRectangle(LCD_WIDTH, LCD_HEIGHT, FACEID_RECTANGLE_X1 - 3, FACEID_RECTANGLE_Y1 - 3,
                            FACEID_RECTANGLE_X2 + 3, FACEID_RECTANGLE_Y2 + 3, BLACK, lcd_data.buffer);
                }

                if (device_settings.enable_lcd) {
                    device_status.statistics.lcd_fps = (float) 1000.0 / (float)(utils_get_time_ms() - lcd_draw_time);
                    lcd_draw_time = utils_get_time_ms();
                    if (device_settings.enable_show_statistics_lcd) {
                        snprintf(fps_string, sizeof(fps_string) - 1, "%5.2f", (double)device_status.statistics.lcd_fps);
                        fonts_putString(LCD_WIDTH, LCD_HEIGHT, LCD_WIDTH - 37, 3, fps_string, Font_7x10, MAGENTA, 0, 0, lcd_data.buffer);
                    }

                    if (lcd_draw_time - audio_result_time < KWS_PRINT_DURATION) {
                        if (device_settings.enable_show_probabilty_lcd) {
                            snprintf(lcd_data.toptitle, sizeof(lcd_data.toptitle) - 1, "%s %0.1f",
                                    device_status.classification_audio.result, (double) device_status.classification_audio.probabily);
                        } else {
                            strncpy(lcd_data.toptitle, device_status.classification_audio.result, sizeof(lcd_data.toptitle) - 1);
                        }

                        fonts_putToptitle(LCD_WIDTH, LCD_HEIGHT, lcd_data.toptitle, Font_16x26, lcd_data.toptitle_color, lcd_data.buffer);
                    }

                    lcd_drawImage(0, 0, LCD_WIDTH, LCD_HEIGHT, lcd_data.buffer);
                }
                break;
            case QSPI_PACKET_TYPE_VIDEO_CLASSIFICATION_RES:
                if (device_status.classification_video.classification == CLASSIFICATION_UNKNOWN) {
                    lcd_data.subtitle_color = RED;
                    lcd_data.frame_color = RED;
                } else if (device_status.classification_video.classification == CLASSIFICATION_LOW_CONFIDENCE) {
                    lcd_data.subtitle_color = YELLOW;
                    lcd_data.frame_color = YELLOW;
                } else if (device_status.classification_video.classification == CLASSIFICATION_DETECTED) {
                    lcd_data.subtitle_color = GREEN;
                    lcd_data.frame_color = GREEN;
                } else if (device_status.classification_video.classification == CLASSIFICATION_NOTHING) {
                    lcd_data.frame_color = WHITE;
                }
                break;
            case QSPI_PACKET_TYPE_AUDIO_CLASSIFICATION_RES:
                audio_result_time = utils_get_time_ms();

                if (strcmp(device_status.classification_audio.result, "OFF") == 0) {
                    device_settings.enable_lcd = 0;
                    lcd_backlight(0);
                } else if(strcmp(device_status.classification_audio.result, "ON") == 0) {
                    device_settings.enable_lcd = 1;
                    lcd_backlight(1);
                } else if (strcmp(device_status.classification_audio.result, "GO") == 0) {
                    device_settings.enable_max78000_video_cnn = 1;
                } else if(strcmp(device_status.classification_audio.result, "STOP") == 0) {
                    device_settings.enable_max78000_video_cnn = 0;
                }
                break;
            default:
                break;
            }
        }
    }

    // Handle SPI RX
    if (commbuf_pop_rx_ble(&ble_packet_container) == COMMBUF_STATUS_OK) {
        PR_INFO("BLE RX packet size %d", ble_packet_container.size);
        if (ble_packet_container.packet.packet_info == PACKET_TYPE_COMMAND) {
            PR_INFO("Command %02X", ble_packet_container.packet.command_packet.header.command);
            PR_INFO("Command size %d", ble_packet_container.packet.command_packet.header.command_size);
            PR_INFO("Payload: ");
            for (int i = 0; i < ble_packet_container.size - sizeof(payload_packet_header_t); i++) {
                PR("0x%02hhX ", ble_packet_container.packet.command_packet.payload[i]);
            }
            PR("\n");
        } else if (ble_packet_container.packet.packet_info == PACKET_TYPE_PAYLOAD) {
            PR_INFO("Payload: ");
            for (int i = 0; i < ble_packet_container.size - sizeof(command_packet_header_t); i++) {
                PR("0x%02hhX ", ble_packet_container.packet.payload_packet.payload[i]);
            }
            PR("\n");
        }
    }
}

// Similar to Core 0, the entry point for Core 1
// is Core1Main()
// Execution begins when the CPU1 Clock is enabled
int Core1_Main(void)
{
    //  __asm__("BKPT");

    int ret = 0;

    PR_INFO("maxrefdes178_max32666 core1");

    core1_irq_init();

    ret = ble_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("ble_init %d", ret);
    }

    core1_init_done = 1;

    PR_INFO("core 1 init completed");

    while (1) {
        ble_worker();
    }

    return -1;
}

static void core0_irq_init(void)
{
    // Disable all interrupts used by core1
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

    NVIC_DisableIRQ(TMR0_IRQn);
    NVIC_DisableIRQ(TMR1_IRQn);

    NVIC_DisableIRQ(WUT_IRQn);

    NVIC_DisableIRQ(SDMA_IRQn); // ?
}

static void core1_irq_init(void)
{
//    // Disable all interrupts
//    for (IRQn_Type irq = PF_IRQn; irq < MXC_IRQ_EXT_COUNT; irq++) {
//        NVIC_DisableIRQ(irq);
//    }
}

void HardFault_Handler(void)
{
    unsigned int cnt = 0;
    while(1) {
        if (cnt % 100000000 == 0) {
            PR("\n\n\n\n!!!!!\n Core %d FaultISR: CFSR %p, BFAR %p, MMFAR %p, HFSR %p\n!!!!!\n\n\n",
                    (SCB->VTOR == (unsigned long)&__isr_vector_core1),
                    SCB->CFSR, SCB->BFAR, SCB->MMFAR, SCB->HFSR);
            cnt = 1;
//            __asm__("BKPT");
        }
        cnt++;
    }
}
