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
#include <camera.h>
#include <gpio.h>
#include <icc.h>
#include <mxc.h>
#include <mxc_delay.h>
#include <rtc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "max78000_debug.h"
#include "max78000_qspi_slave.h"
#include "max78000_video_cnn.h"
#include "max78000_video_embedding_process.h"
#include "max78000_video_weights.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_utility.h"
#include "maxrefdes178_version.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME          "main"

//#define PRINT_TIME_CNN
#define CAMERA_FREQ     (20 * 1000 * 1000)


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
mxc_gpio_cfg_t gpio_flash  = MAX78000_VIDEO_FLASH_PIN;
mxc_gpio_cfg_t gpio_camera = MAX78000_VIDEO_CAMERA_PIN;
mxc_gpio_cfg_t gpio_red    = MAX78000_VIDEO_LED_RED_PIN;
mxc_gpio_cfg_t gpio_green  = MAX78000_VIDEO_LED_GREEN_PIN;
mxc_gpio_cfg_t gpio_blue   = MAX78000_VIDEO_LED_BLUE_PIN;


static const uint8_t camera_settings[][2] = {
    {0x0e, 0x08}, // Sleep mode
    {0x69, 0x52}, // BLC window selection, BLC enable (default is 0x12)
    {0x1e, 0xb3}, // AddLT1F (default 0xb1)
    {0x48, 0x42},
    {0xff, 0x01}, // Select MIPI register bank
    {0xb5, 0x30},
    {0xff, 0x00}, // Select system control register bank
    {0x16, 0x03}, // (default)
    {0x62, 0x10}, // (default)
    {0x12, 0x01}, // Select Bayer RAW
    {0x17, 0x65}, // Horizontal Window Start Point Control (LSBs), default is 0x69
    {0x18, 0xa4}, // Horizontal sensor size (default)
    {0x19, 0x0c}, // Vertical Window Start Line Control (default)
    {0x1a, 0xf6}, // Vertical sensor size (default)
    {0x37, 0x04}, // PCLK is double system clock (default is 0x0c)
    {0x3e, 0x20}, // (default)
    {0x81, 0x3f}, // sde_en, uv_adj_en, scale_v_en, scale_h_en, uv_avg_en, cmx_en
    {0xcc, 0x02}, // High 2 bits of horizontal output size (default)
    {0xcd, 0x80}, // Low 8 bits of horizontal output size (default)
    {0xce, 0x01}, // Ninth bit of vertical output size (default)
    {0xcf, 0xe0}, // Low 8 bits of vertical output size (default)
    {0x82, 0x01}, // 01: Raw from CIP (default is 0x00)
    {0xc8, 0x02},
    {0xc9, 0x80},
    {0xca, 0x01},
    {0xcb, 0xe0},
    {0xd0, 0x28},
    {0x0e, 0x00}, // Normal mode (not sleep mode)
    {0x70, 0x00},
    {0x71, 0x34},
    {0x74, 0x28},
    {0x75, 0x98},
    {0x76, 0x00},
    {0x77, 0x64},
    {0x78, 0x01},
    {0x79, 0xc2},
    {0x7a, 0x4e},
    {0x7b, 0x1f},
    {0x7c, 0x00},
    {0x11, 0x01}, // CLKRC, Internal clock pre-scalar divide by 2 (default divide by 1)
    {0x20, 0x00}, // Banding filter (default)
    {0x21, 0x57}, // Banding filter (default is 0x44)
    {0x50, 0x4d},
    {0x51, 0x40}, // 60Hz Banding AEC 8 bits (default 0x80)
    {0x4c, 0x7d},
    {0x0e, 0x00},
    {0x80, 0x7f},
    {0x85, 0x00},
    {0x86, 0x00},
    {0x87, 0x00},
    {0x88, 0x00},
    {0x89, 0x2a},
    {0x8a, 0x22},
    {0x8b, 0x20},
    {0xbb, 0xab},
    {0xbc, 0x84},
    {0xbd, 0x27},
    {0xbe, 0x0e},
    {0xbf, 0xb8},
    {0xc0, 0xc5},
    {0xc1, 0x1e},
    {0xb7, 0x05},
    {0xb8, 0x09},
    {0xb9, 0x00},
    {0xba, 0x18},
    {0x5a, 0x1f},
    {0x5b, 0x9f},
    {0x5c, 0x69},
    {0x5d, 0x42},
    {0x24, 0x78}, // AGC/AEC
    {0x25, 0x68}, // AGC/AEC
    {0x26, 0xb3}, // AGC/AEC
    {0xa3, 0x0b},
    {0xa4, 0x15},
    {0xa5, 0x29},
    {0xa6, 0x4a},
    {0xa7, 0x58},
    {0xa8, 0x65},
    {0xa9, 0x70},
    {0xaa, 0x7b},
    {0xab, 0x85},
    {0xac, 0x8e},
    {0xad, 0xa0},
    {0xae, 0xb0},
    {0xaf, 0xcb},
    {0xb0, 0xe1},
    {0xb1, 0xf1},
    {0xb2, 0x14},
    {0x8e, 0x92},
    {0x96, 0xff},
    {0x97, 0x00},
    {0x14, 0x3b},   // AGC value, manual, set banding (default is 0x30)
    {0x0e, 0x00},
    {0x0c, 0xd6},
    {0x82, 0x3},
    {0x11, 0x00},   // Set clock prescaler
    {0x12, 0x6},
    {0x61, 0x0},
    {0x64, 0x11},
    {0xc3, 0x80},
    {0x81, 0x3f},
    {0x16, 0x3},
    {0x37, 0xc},
    {0x3e, 0x20},
    {0x5e, 0x0},
    {0xc4, 0x1},
    {0xc5, 0x80},
    {0xc6, 0x1},
    {0xc7, 0x80},
    {0xc8, 0x2},
    {0xc9, 0x80},
    {0xca, 0x1},
    {0xcb, 0xe0},
    {0xcc, 0x0},
    {0xcd, 0x40},   // Default to 64 line width
    {0xce, 0x0},
    {0xcf, 0x40},   // Default to 64 lines high
    {0x1c, 0x7f},
    {0x1d, 0xa2},
    {0xee, 0xee}  // End of register list marker 0xee
};

// *****************************************************************************
static int8_t prev_decision = -2;
static int8_t decision = -2;
static uint32_t time_counter = 0;
static int8_t enable_cnn = 1;
static int8_t enable_video = 1;
static uint8_t *qspi_payload_buffer = NULL;

version_t version = {S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD};

#ifdef PRINT_TIME_CNN
#define PR_TIMER(fmt, args...) if((time_counter % 10) == 0) printf("T[%-5s:%4d] " fmt "\r\n", S_MODULE_NAME, __LINE__, ##args )
#endif


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void fail(void);
static void send_img(void);
static void run_cnn(int x_offset, int y_offset);
static void run_demo(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int main(void)
{
    int ret = 0;
    int slaveAddress;
    int id;

    // Enable camera
    GPIO_CLR(gpio_camera);
    MXC_GPIO_Config(&gpio_camera);

    // Configure flash pin
    GPIO_CLR(gpio_flash);
    MXC_GPIO_Config(&gpio_flash);

    // Configure LEDs
    GPIO_CLR(gpio_red);
    MXC_GPIO_Config(&gpio_red);

    GPIO_CLR(gpio_green);
    MXC_GPIO_Config(&gpio_green);

    GPIO_CLR(gpio_blue);
    MXC_GPIO_Config(&gpio_blue);

    /* Enable cache */
    MXC_ICC_Enable(MXC_ICC0);

    /* Set system clock to 100 MHz */
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    PR_INFO("maxrefdes178_max78000_video v%d.%d.%d [%s]",
            version.major, version.minor, version.build, S_BUILD_TIMESTAMP);

    // Enable peripheral, enable CNN interrupt, turn on CNN clock
    // CNN clock: 50 MHz div 1
    ret  = cnn_enable(MXC_S_GCR_PCLKDIV_CNNCLKSEL_PCLK, MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1);
    ret &= cnn_init(); // Bring CNN state machine into consistent state
    ret &= cnn_load_weights(); // Load CNN kernels
    ret &= cnn_configure(); // Configure CNN state machine
    if (ret != CNN_OK) {
        PR_ERROR("Could not initialize the CNN accelerator %d", ret);
        fail();
    }

    ret = init_database();
    if (ret != E_NO_ERROR) {
        PR_ERROR("Could not initialize the database, %d", ret);
        fail();
    }

    /* Initialize RTC */
    ret = MXC_RTC_Init(0, 0);
    if (ret != E_NO_ERROR) {
        PR_ERROR("Could not initialize rtc, %d", ret);
        fail();
    }

    ret = MXC_RTC_Start();
    if (ret != E_NO_ERROR) {
        PR_ERROR("Could not start rtc, %d", ret);
        fail();
    }

    ret = MXC_DMA_Init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("DMA INIT ERROR %d", ret);
        fail();
    }

    ret = qspi_slave_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("qspi_dma_slave_init fail %d", ret);
        fail();
    }

    // Initialize the camera driver.
    ret = camera_init(CAMERA_FREQ);
    if (ret != E_NO_ERROR) {
        PR_ERROR("Camera init failed! %d", ret);
        fail();
    }

    // Obtain the I2C slave address of the camera.
    slaveAddress = camera_get_slave_address();
    PR_INFO("Camera I2C slave address is 0x%02hhX", slaveAddress);

    // Obtain the product ID of the camera.
    ret = camera_get_product_id(&id);
    if (ret != STATUS_OK) {
        PR_ERROR("Error returned from reading camera product id. Error : %d", ret);
        fail();
    }
    PR_INFO("Camera Product ID is 0x%04hhX", id);

    // Obtain the manufacture ID of the camera.
    ret = camera_get_manufacture_id(&id);
    if (ret != STATUS_OK) {
        PR_ERROR("Error returned from reading camera manufacture id. Error : %d", ret);
        return -1;
    }
    PR_INFO("Camera Manufacture ID is 0x%04hhX", id);

    // set camera registers with default values
    for (int i = 0; (camera_settings[i][0] != 0xee); i++) {
        camera_write_reg(camera_settings[i][0], camera_settings[i][1]);
    }

    // Setup the camera image dimensions, pixel format and data acquiring details.
    ret = camera_setup(CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_FORMAT, FIFO_FOUR_BYTE, USE_DMA, MAX78000_VIDEO_CAMERA_DMA_CHANNEL);
    if (ret != STATUS_OK) {
        PR_ERROR("Error returned from setting up camera. Error : %d", ret);
        fail();
    }

    // Use camera interface buffer for QSPI payload buffer
    {
        uint32_t  imgLen;
        uint32_t  w, h;
        camera_get_image(&qspi_payload_buffer, &imgLen, &w, &h);
    }

    // Successfully initialize the program
    PR_INFO("Program initialized successfully");

    run_demo();

    return 0;
}

static void fail(void)
{
    PR_ERROR("fail");

    GPIO_SET(gpio_red);
    GPIO_CLR(gpio_green);
    GPIO_CLR(gpio_blue);

    while(1);
}

static void run_demo(void)
{
//    uint8_t run_count = 0;
    uint32_t capture_started_time = GET_RTC_MS();
    uint32_t cnn_completed_time;
    uint32_t qspi_completed_time;
    uint32_t capture_completed_time;
    max78000_statistics_t max78000_statistics;
    qspi_packet_header_t qspi_rx_header;
    qspi_state_e qspi_rx_state;

    PR_INFO("Embeddings subject names:");
    for (int i = 0; i < get_subject_count(); i++) {
          PR_INFO("  %s", get_subject_name(i));
    }

    camera_start_capture_image();

    while (1) { //Capture image and run CNN

        /* Check if QSPI RX has data */
        qspi_rx_state = qspi_slave_get_rx_state();
        if (qspi_rx_state == QSPI_STATE_CS_DEASSERTED_HEADER) {
            qspi_rx_header = qspi_slave_get_rx_header();

            // Use camera interface buffer for QSPI payload
            MXC_PCIF_Stop();

            qspi_slave_set_rx_data(qspi_payload_buffer, qspi_rx_header.info.packet_size);
            qspi_slave_trigger();
            qspi_slave_wait_rx();

            // Check payload crc again
            if (qspi_rx_header.payload_crc16 != crc16_sw(qspi_payload_buffer, qspi_rx_header.info.packet_size)) {
                PR_ERROR("Invalid payload crc %x", qspi_rx_header.payload_crc16);
                qspi_slave_set_rx_state(QSPI_STATE_IDLE);
                camera_start_capture_image();
                continue;
            }

            // QSPI commands with payload
            switch(qspi_rx_header.info.packet_type) {
                case QSPI_PACKET_TYPE_VIDEO_FACEID_EMBED_UPDATE_CMD:
                    PR_INFO("facied embeddings received %d", qspi_rx_header.info.packet_size);

                    uninit_database();

                    uint8_t faceid_embed_update_status;
                    if (update_database(qspi_payload_buffer, qspi_rx_header.info.packet_size) != E_NO_ERROR) {
                        PR_ERROR("Could not update the database");
                        faceid_embed_update_status = FACEID_EMBED_UPDATE_STATUS_ERROR_UNKNOWN;
                    } else if (init_database() != E_NO_ERROR) {
                        PR_ERROR("Could not initialize the database");
                        faceid_embed_update_status = FACEID_EMBED_UPDATE_STATUS_ERROR_UNKNOWN;
                    } else {
                        PR_INFO("Embeddings update completed");
                        for (int i = 0; i < get_subject_count(); i++) {
                            PR_INFO("  %s", get_subject_name(i));
                        }
                        faceid_embed_update_status = FACEID_EMBED_UPDATE_STATUS_SUCCESS;
                    }
                    qspi_slave_set_rx_state(QSPI_STATE_IDLE);
                    qspi_slave_send_packet(&faceid_embed_update_status, 1, QSPI_PACKET_TYPE_VIDEO_FACEID_EMBED_UPDATE_RES);

                    break;
                case QSPI_PACKET_TYPE_TEST:
                    PR_INFO("test data received %d", qspi_rx_header.info.packet_size);
                    for (int i = 0; i < qspi_rx_header.info.packet_size; i++) {
                        printf("%02hhX ", qspi_payload_buffer[i]);
                    }
                    printf("\n");
                    break;
                default:
                    PR_ERROR("Invalid packet %d", qspi_rx_header.info.packet_type);
                    break;
            }

            qspi_slave_set_rx_state(QSPI_STATE_IDLE);
            camera_start_capture_image();

        } else if (qspi_rx_state == QSPI_STATE_COMPLETED) {
            qspi_rx_header = qspi_slave_get_rx_header();

            // QSPI commands without payload
            switch(qspi_rx_header.info.packet_type) {
            case QSPI_PACKET_TYPE_VIDEO_VERSION_CMD:
                qspi_slave_set_rx_state(QSPI_STATE_IDLE);
                qspi_slave_send_packet((uint8_t *) &version, sizeof(version),
                        QSPI_PACKET_TYPE_VIDEO_VERSION_RES);
                break;
            case QSPI_PACKET_TYPE_VIDEO_SERIAL_CMD:
                // TODO
                break;
            case QSPI_PACKET_TYPE_VIDEO_ENABLE_CMD:
                PR_INFO("enable video");
                enable_video = 1;
                camera_start_capture_image();
                break;
            case QSPI_PACKET_TYPE_VIDEO_DISABLE_CMD:
                PR_INFO("disable video");
                enable_video = 0;
                MXC_PCIF_Stop();
                GPIO_CLR(gpio_red);
                GPIO_CLR(gpio_green);
                break;
            case QSPI_PACKET_TYPE_VIDEO_ENABLE_CNN_CMD:
                PR_INFO("enable cnn");
                enable_cnn = 1;
                break;
            case QSPI_PACKET_TYPE_VIDEO_DISABLE_CNN_CMD:
                PR_INFO("disable cnn");
                enable_cnn = 0;
                GPIO_CLR(gpio_red);
                GPIO_CLR(gpio_green);
                break;
            case QSPI_PACKET_TYPE_VIDEO_FACEID_SUBJECTS_CMD:
                // Use camera interface buffer for FaceID embeddings subject names
                MXC_PCIF_Stop();

                memcpy(qspi_payload_buffer, get_subject_name(0), get_subject_names_len());
                qspi_slave_set_rx_state(QSPI_STATE_IDLE);
                qspi_slave_send_packet(qspi_payload_buffer, get_subject_names_len(), QSPI_PACKET_TYPE_VIDEO_FACEID_SUBJECTS_RES);

                camera_start_capture_image();
                break;
            case QSPI_PACKET_TYPE_VIDEO_ENABLE_FLASH_LED_CMD:
                GPIO_SET(gpio_flash);
                PR_INFO("enable flash");
                break;
            case QSPI_PACKET_TYPE_VIDEO_DISABLE_FLASH_LED_CMD:
                GPIO_CLR(gpio_flash);
                PR_INFO("disable flash");
                break;
            }

            qspi_slave_set_rx_state(QSPI_STATE_IDLE);
        }

        if (!enable_video) {
            __WFI();
            continue;
        }

        if (camera_is_image_rcv()) { // Check whether image is ready
            capture_completed_time = GET_RTC_MS();

            send_img();

            qspi_completed_time = GET_RTC_MS();

            if (enable_cnn) {
                run_cnn(0, 0);

//                switch (run_count) {
//                case 0:
//                    run_cnn(0, 0);
//                    break;
//                case 1:
//                    run_cnn(-10, -10);
//                    break;
//                case 2:
//                    run_cnn(10, 10);
//                    break;
//                case 3:
//                    run_cnn(0, 0);
//                    break;
//                case 4:
//                    run_cnn(-10, 10);
//                    break;
//                case 5:
//                    run_cnn(10, -10);
//                    break;
//                }
//               run_count++;
//               run_count = run_count % 6;

//                run_cnn(0, 0);
//                if ((run_count % 2) == 0){
//                    run_count = 0;
//                    run_cnn(-10, -10);
//                    run_cnn(10, 10);
//                } else {
//                    run_cnn(-10, 10);
//                    run_cnn(10, -10);
//                }
//                run_count++;
            }

            cnn_completed_time = GET_RTC_MS();

            if (time_counter % 10 == 0) {
                max78000_statistics.capture_duration_us = (capture_completed_time - capture_started_time) * 1000;
                max78000_statistics.communication_duration_us = (qspi_completed_time - capture_completed_time) * 1000;
                max78000_statistics.cnn_duration_us = (cnn_completed_time - qspi_completed_time) * 1000;
                max78000_statistics.total_duration_us = (cnn_completed_time - capture_started_time) * 1000;

                PR_DEBUG("Capture : %lu", max78000_statistics.capture_duration_us);
                PR_DEBUG("CNN     : %lu", max78000_statistics.cnn_duration_us);
                PR_DEBUG("QSPI    : %lu", max78000_statistics.communication_duration_us);
                PR_DEBUG("Total   : %lu\n\n", max78000_statistics.total_duration_us);

                qspi_slave_send_packet((uint8_t *) &max78000_statistics, sizeof(max78000_statistics),
                        QSPI_PACKET_TYPE_VIDEO_STATISTICS_RES);
            }

            time_counter++;

            camera_start_capture_image();
            capture_started_time = GET_RTC_MS();

        }
    }
}

static void send_img(void)
{
    uint8_t   *raw;
    uint32_t  imgLen;
    uint32_t  w, h;

    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &imgLen, &w, &h);

    qspi_slave_send_packet(raw, imgLen, QSPI_PACKET_TYPE_VIDEO_DATA_RES);
//    MXC_Delay(MXC_DELAY_MSEC(3)); // Yield SPI DMA RAM read
}

static void run_cnn(int x_offset, int y_offset)
{
    uint8_t *data;
    uint8_t *raw;
    uint8_t ur, ug, ub;
    int8_t r, g, b;
    uint32_t number;
    uint32_t w, h;
    static uint32_t noface_count = 0;

    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &number, &w, &h);

#ifdef PRINT_TIME_CNN
    uint32_t pass_time = GET_RTC_MS();
#endif

    // Enable CNN clock
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_CNN);

    cnn_init(); // Bring state machine into consistent state
    //cnn_load_weights(); // No need to reload kernels
    cnn_configure(); // Configure state machine

    cnn_start();

#ifdef PRINT_TIME_CNN
    PR_TIMER("CNN init : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif

    for (int i = y_offset; i < FACEID_HEIGHT + y_offset; i++) {
        data = raw + (((LCD_HEIGHT - FACEID_HEIGHT) / 2) + i) * LCD_WIDTH * LCD_BYTE_PER_PIXEL;  // down
        data += ((LCD_WIDTH - FACEID_WIDTH) / 2) * LCD_BYTE_PER_PIXEL;  // right

        for(int j = x_offset; j < FACEID_WIDTH + x_offset; j++) {
            // RGB565, |RRRRRGGG|GGGBBBBB|
            ub = (data[j * LCD_BYTE_PER_PIXEL + 1] << 3);
            ug = ((data[j * LCD_BYTE_PER_PIXEL] << 5) | ((data[j * LCD_BYTE_PER_PIXEL + 1] & 0xE0) >> 3));
            ur = (data[j * LCD_BYTE_PER_PIXEL] & 0xF8);

            b = ub - 128;
            g = ug - 128;
            r = ur - 128;

            // Loading data into the CNN fifo
            while (((*((volatile uint32_t *) 0x50000004) & 1)) != 0); // Wait for FIFO 0

            number = 0x00FFFFFF & ((((uint8_t)b) << 16) | (((uint8_t)g) << 8) | ((uint8_t) r));

            *((volatile uint32_t *) 0x50000008) = number; // Write FIFO 0
        }
    }

#ifdef PRINT_TIME_CNN
    PR_TIMER("CNN load : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif

    while (cnn_time == 0)
        __WFI(); // Wait for CNN done

#ifdef PRINT_TIME_CNN
    PR_TIMER("CNN wait : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif

    cnn_unload((uint32_t*)(raw));

    cnn_stop();
    // Disable CNN clock to save power
    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_CNN);

#ifdef PRINT_TIME_CNN
    PR_TIMER("CNN unload : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif

    int pResult = calculate_minDistance((uint8_t*)(raw));

#ifdef PRINT_TIME_CNN
    PR_TIMER("Embedding calc : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif

    if (pResult == 0) {
        classification_result_t classification_result = {0};
        uint8_t *counter;
        uint8_t counter_len;
        get_min_dist_counter(&counter, &counter_len);
        classification_result.classification = CLASSIFICATION_NOTHING;
        GPIO_CLR(gpio_red);
        GPIO_CLR(gpio_green);

        prev_decision = decision;
        decision = -5;

        for(uint8_t id = 0; id < counter_len; ++id){
            if (counter[id] >= (uint8_t)(closest_sub_buffer_size * 0.8)){   // >80% detection
                strncpy(classification_result.result, get_subject_name(id), sizeof(classification_result.result) - 1);
                decision = id;
                classification_result.classification = CLASSIFICATION_DETECTED;
                noface_count = 0;
                GPIO_CLR(gpio_red);
                GPIO_SET(gpio_green);
                break;
            } else if (counter[id] >= (uint8_t)(closest_sub_buffer_size * 0.4)){  // >%40 adjust
                strncpy(classification_result.result, "Adjust Face", sizeof(classification_result.result) - 1);
                decision = -2;
                classification_result.classification = CLASSIFICATION_LOW_CONFIDENCE;
                noface_count = 0;
                GPIO_SET(gpio_red);
                GPIO_SET(gpio_green);
                break;
            } else if (counter[id] > closest_sub_buffer_size * 0.2){   // >20% unknown
                strncpy(classification_result.result, "Unknown", sizeof(classification_result.result) - 1);
                decision = -1;
                classification_result.classification = CLASSIFICATION_UNKNOWN;
                noface_count = 0;
                GPIO_SET(gpio_red);
                GPIO_CLR(gpio_green);
                break;
            }
            else if (counter[id] > closest_sub_buffer_size * 0.1){   // >10% transition
                strncpy(classification_result.result, "Transition", sizeof(classification_result.result) - 1);
                decision = -3;
                classification_result.classification = CLASSIFICATION_NOTHING;
                noface_count = 0;
            }
            else
            {
                noface_count ++;
                if (noface_count > 10)
                {
                    strncpy(classification_result.result, "No Face", sizeof(classification_result.result) - 1);
                    decision = -4;
                    classification_result.classification = CLASSIFICATION_NOTHING;
                    noface_count --;
                }
            }
        }

        if(decision != prev_decision){
            qspi_slave_send_packet((uint8_t *) &classification_result, sizeof(classification_result),
                    QSPI_PACKET_TYPE_VIDEO_CLASSIFICATION_RES);
            PR_DEBUG("Result : %s\n", classification_result.result);
        }
    }

#ifdef PRINT_TIME_CNN
    PR_TIMER("Embedding result : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif
}
