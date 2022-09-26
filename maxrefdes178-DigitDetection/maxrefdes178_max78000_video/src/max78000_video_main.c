/*******************************************************************************
 * Copyright (C) 2020-2022 Maxim Integrated Products, Inc., All rights Reserved.
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
#include <math.h>
#include <stdlib.h>

#include "max78000_debug.h"
#include "max78000_qspi_slave.h"
#include "max78000_video_cnn.h"
#include "max78000_video_weights.h"
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_utility.h"
#include "maxrefdes178_version.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME          "main"
//#define PRINT_TIME_CNN
//#define USE_SAMPLEDATA        // shows the sample data

#ifdef USE_SAMPLEDATA // Include sample data
#include "sampledata.h"
#endif

#define IMAGE_HEIGHT 240
#define IMAGE_WIDTH  240
#define X_SIZE       74
#define Y_SIZE       74
#define X_OFFSET     9
#define Y_OFFSET     9
#define SQUARE(x)   ((x) * (x))
#define MIN(x, y)   (((x) < (y)) ? (x) : (y))
#define MAX(x, y)   (((x) > (y)) ? (x) : (y))
#define IMG_SCALE   3
#define NUM_ARS     4
#define NUM_SCALES  4
#define NUM_CLASSES 12
#define LOC_DIM     4 //(x, y, w, h) or (x1, y1, x2, y2)
#define NUM_PRIORS_PER_AR   425
#define NUM_PRIORS          NUM_PRIORS_PER_AR*NUM_ARS
#define MAX_PRIORS          100
#define MIN_CLASS_SCORE     16384 // ~0.25*65536
#define MAX_ALLOWED_OVERLAP 0.3   //170
#define ML_DATA_SIZE 5
//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

/* **************************************************************************** */
static void get_priors(void);
static void nms(void);
static void get_cxcy(float* cxcy, int prior_idx);
static void gcxgcy_to_cxcy(float* cxcy, int prior_idx, float* priors_cxcy);
static void cxcy_to_xy(float* xy, float* cxcy);
static void localize_objects(void);
static uint32_t  camera_image[LCD_DATA_SIZE/4];

#ifdef USE_SAMPLEDATA
const uint32_t input_buffer[] = SAMPLE_INPUT_0;
#endif

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
mxc_tmr_unit_t units;

static const int dims[NUM_SCALES] = {18, 9, 4, 2}; // NUM_PRIORS_PER_AR = SQUARE(dims[0]) + SQUARE(dims[1]) + SQUARE(dims[2]) + SQUARE(dims[3])
static const float scales[NUM_SCALES] = {0.15f, 0.35f, 0.55f, 0.725f};
static const float ars[NUM_ARS]       = {0.85f, 0.60f, 0.40f, 0.25f};
//Arrays pointers to store model outputs
static int8_t *prior_cls;  // int8_t prior_cls[NUM_CLASSES * NUM_PRIORS]; // 20400 bytes
static uint16_t *prior_cls_softmax; // uint16_t prior_cls_softmax[NUM_CLASSES * NUM_PRIORS] // 40800 bytes
static int8_t *prior_locs; // int8_t prior_locs[LOC_DIM * NUM_PRIORS]; (x, y, w, h) // 6800 bytes
static uint8_t *objects; // store objects result
//NMS related array pointer
static uint8_t *nms_removed; // uint8_t nms_removed[NUM_CLASSES - 2][MAX_PRIORS]; // 1000 bytes
//NMS related arrays
static int num_nms_priors[NUM_CLASSES - 2];    // 40 bytes
static uint16_t nms_scores[NUM_CLASSES - 2][MAX_PRIORS]; // 2000 bytes
static uint16_t nms_indices[NUM_CLASSES - 2][MAX_PRIORS]; // 2000 bytes

static const mxc_gpio_cfg_t gpio_flash     = MAX78000_VIDEO_FLASH_LED_PIN;
static const mxc_gpio_cfg_t gpio_camera    = MAX78000_VIDEO_CAMERA_PIN;
static const mxc_gpio_cfg_t gpio_sram_cs   = MAX78000_VIDEO_SRAM_CS_PIN;
static const mxc_gpio_cfg_t gpio_i2c       = MAX78000_VIDEO_I2C_PINS;
static const mxc_gpio_cfg_t gpio_debug_sel = MAX78000_VIDEO_DEBUG_SEL_PIN;
static const mxc_gpio_cfg_t gpio_exp_io    = MAX78000_VIDEO_EXPANDER_IO_PIN;
static const mxc_gpio_cfg_t gpio_exp_out   = MAX78000_VIDEO_EXPANDER_OUT_PIN;
static const mxc_gpio_cfg_t gpio_audio_int = MAX78000_VIDEO_AUDIO_INT_PIN;
static const mxc_gpio_cfg_t gpio_cnn_boost = MAX78000_VIDEO_CNN_BOOST_PIN;
static const mxc_gpio_cfg_t gpio_red       = MAX78000_VIDEO_LED_RED_PIN;
static const mxc_gpio_cfg_t gpio_green     = MAX78000_VIDEO_LED_GREEN_PIN;
static const mxc_gpio_cfg_t gpio_blue      = MAX78000_VIDEO_LED_BLUE_PIN;

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
    {0x14, 0x2b},   // AGC value, manual, set banding (default is 0x30)
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
static volatile int8_t button_pressed = 0;
static uint32_t time_counter = 0;
static int8_t enable_cnn = 0;
static int8_t flash_led = 0;
static int8_t camera_vflip = 1;
static int8_t enable_video = 0;
static int8_t enable_sleep = 0;
static uint8_t *qspi_payload_buffer = NULL;
static version_t version = {S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD};
static char demo_name[] = DIGIT_DET_DEMO_NAME;
static uint32_t camera_clock = 10 * 1000 * 1000;

#ifdef PRINT_TIME_CNN
#define PR_TIMER(fmt, args...) if((time_counter % 10) == 0) printf("T[%-5s:%4d] " fmt "\r\n", S_MODULE_NAME, __LINE__, ##args )
#endif

//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------

static void fail(void);
static void send_img(void);
static void run_cnn(void);
static void run_demo(void);
static void get_priors(void);
static void nms(void);
static void nms_memory_init(void);
static void get_cxcy(float* cxcy, int prior_idx);
static void gcxgcy_to_cxcy(float* cxcy, int prior_idx, float* priors_cxcy);
static void cxcy_to_xy(float* xy, float* cxcy);
static void localize_objects(void);

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void button_int(void *cbdata)
{
    button_pressed = 1;
}

void sleep_defer_int(void)
{
    // Clear interrupt
    MXC_TMR_ClearFlags(MAX78000_VIDEO_SLEEP_DEFER_TMR);

    enable_sleep = 1;
}

int main(void)
{
int dma_channel;

    MXC_Delay(MXC_DELAY_MSEC(500)); // Wait supply to be ready

    int ret = 0;
    int slaveAddress;
    int id;

    // Enable camera
    GPIO_CLR(gpio_camera);
    MXC_GPIO_Config(&gpio_camera);

    // Configure flash pin
    GPIO_SET(gpio_flash);
    MXC_GPIO_Config(&gpio_flash);
    MXC_GPIO0->ds0 |= gpio_flash.mask;
    MXC_GPIO0->ds1 |= gpio_flash.mask;
    GPIO_SET(gpio_flash);

    // Configure CNN boost
    //GPIO_SET(gpio_cnn_boost);
    GPIO_CLR(gpio_cnn_boost);
    MXC_GPIO_Config(&gpio_cnn_boost);

    // Configure LEDs
    GPIO_CLR(gpio_red);
    MXC_GPIO_Config(&gpio_red);

    GPIO_CLR(gpio_green);
    MXC_GPIO_Config(&gpio_green);

    GPIO_CLR(gpio_blue);
    MXC_GPIO_Config(&gpio_blue);

    // Configure unused pins as high-z
    MXC_GPIO_Config(&gpio_sram_cs);
    MXC_GPIO_Config(&gpio_i2c);
    MXC_GPIO_Config(&gpio_debug_sel);
    MXC_GPIO_Config(&gpio_exp_io);
    MXC_GPIO_Config(&gpio_exp_out);
    MXC_GPIO_Config(&gpio_audio_int);

    /* Enable cache */
    MXC_ICC_Enable(MXC_ICC0);

    /* Set system clock to 100 MHz */
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    PR_INFO("maxrefdes178_max78000_video %s v%d.%d.%d [%s]",
            demo_name, version.major, version.minor, version.build, S_BUILD_TIMESTAMP);

    // Enable peripheral, enable CNN interrupt, turn on CNN clock
    // CNN clock: 50 MHz div 1
    ret  = cnn_enable(MXC_S_GCR_PCLKDIV_CNNCLKSEL_PCLK, MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1);
    ret &= cnn_init(); // Bring CNN state machine into consistent state
    ret &= cnn_load_weights(); // Load CNN kernels
    ret &= cnn_load_bias(); // Load bias
    ret &= cnn_configure(); // Configure CNN state machine
    if (ret != CNN_OK) {
        PR_ERROR("Could not initialize the CNN accelerator %d", ret);
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

    // Initialize DMA for camera interface
    dma_channel = MXC_DMA_AcquireChannel();
    PR_INFO("Camera DMA channel = %d", dma_channel);

    ret = qspi_slave_init();
    if (ret != E_NO_ERROR) {
        PR_ERROR("qspi_dma_slave_init fail %d", ret);
        fail();
    }
    
    // Initialize the camera driver.
    ret = camera_init(camera_clock);
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
    ret = camera_setup(CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_FORMAT, FIFO_FOUR_BYTE, STREAMING_DMA, dma_channel);

    if (ret != STATUS_OK) {
        PR_ERROR("Error returned from setting up camera. Error : %d", ret);
        fail();
    }

    // set camera clock prescaller to prevent streaming overflow due to QSPI communication and CNN load latency
    //camera_write_reg(0x11, 0x2);

    // make the scale ratio of camera input size the same as output size, make is faster and regular
    camera_write_reg(0xc8, 0x1);
    camera_write_reg(0xc9, 0x60);
    camera_write_reg(0xca, 0x1);
    camera_write_reg(0xcb, 0x60);

    // Init button interrupt
    PB_RegisterCallback(0, (pb_callback) button_int);

    // Init sleep defer timer interrupt
    mxc_tmr_cfg_t tmr;
    MXC_TMR_Shutdown(MAX78000_VIDEO_SLEEP_DEFER_TMR);
    tmr.pres = TMR_PRES_128;
    tmr.mode = TMR_MODE_ONESHOT;
    tmr.bitMode = TMR_BIT_MODE_32;
    tmr.clock = MXC_TMR_APB_CLK;
    tmr.cmp_cnt = MAX78000_SLEEP_DEFER_DURATION * PeripheralClock / 128;
    tmr.pol = 0;
    MXC_NVIC_SetVector(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX78000_VIDEO_SLEEP_DEFER_TMR)), sleep_defer_int);
    NVIC_EnableIRQ(MXC_TMR_GET_IRQ(MXC_TMR_GET_IDX(MAX78000_VIDEO_SLEEP_DEFER_TMR)));
    MXC_TMR_Init(MAX78000_VIDEO_SLEEP_DEFER_TMR, &tmr, false);
    MXC_TMR_EnableInt(MAX78000_VIDEO_SLEEP_DEFER_TMR);
    MXC_TMR_Start(MAX78000_VIDEO_SLEEP_DEFER_TMR);

    // Use end of camera interface buffer for QSPI payload buffer
    qspi_payload_buffer = (uint8_t*)&camera_image[LCD_DATA_SIZE/4 - 1000];

    // Setup NMS algorithm memory
    nms_memory_init();

    // Successfully initialize the program
    PR_INFO("Initialization complete");

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
    uint32_t capture_started_time = GET_RTC_MS();
    uint32_t cnn_completed_time = 0;
    uint32_t qspi_completed_time = 0;
    uint32_t capture_completed_time = 0;
    max78000_statistics_t max78000_statistics = {0};
    qspi_packet_header_t qspi_rx_header;
    qspi_state_e qspi_rx_state;

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
                continue;
            }

            // QSPI commands with payload
            switch(qspi_rx_header.info.packet_type) {
                case QSPI_PACKET_TYPE_VIDEO_FACEID_EMBED_UPDATE_CMD:
                    PR_INFO("Command not supported");
                    break;
                case QSPI_PACKET_TYPE_TEST:
                    PR_INFO("test data received %d", qspi_rx_header.info.packet_size);
                    for (int i = 0; i < qspi_rx_header.info.packet_size; i++) {
                        PR_INFO("%02hhX ", qspi_payload_buffer[i]);
                    }
                    PR_INFO("\n");
                    break;
                default:
                    PR_ERROR("Invalid packet %d", qspi_rx_header.info.packet_type);
                    break;
            }

            qspi_slave_set_rx_state(QSPI_STATE_IDLE);

        } else if (qspi_rx_state == QSPI_STATE_COMPLETED) {
            qspi_rx_header = qspi_slave_get_rx_header();

            // QSPI commands without payload
            switch(qspi_rx_header.info.packet_type) {
            case QSPI_PACKET_TYPE_VIDEO_VERSION_CMD:
                qspi_slave_set_rx_state(QSPI_STATE_IDLE);
                qspi_slave_send_packet((uint8_t *) &version, sizeof(version),
                        QSPI_PACKET_TYPE_VIDEO_VERSION_RES);
                break;
            case QSPI_PACKET_TYPE_VIDEO_DEMO_NAME_CMD:
                qspi_slave_set_rx_state(QSPI_STATE_IDLE);
                qspi_slave_send_packet((uint8_t *) demo_name, strlen(demo_name),
                        QSPI_PACKET_TYPE_VIDEO_DEMO_NAME_RES);
                break;
            case QSPI_PACKET_TYPE_VIDEO_SERIAL_CMD:
                // TODO
                break;
            case QSPI_PACKET_TYPE_VIDEO_ENABLE_CMD:
                PR_INFO("enable video");
                enable_video = 1;
                // Also CNN
                PR_INFO("enable cnn");
                enable_cnn = 1;
                // Enable camera
                GPIO_CLR(gpio_camera);
                break;
            case QSPI_PACKET_TYPE_VIDEO_DISABLE_CMD:
                PR_INFO("disable video");
                enable_video = 0;
                MXC_PCIF_Stop();
                // Disable camera
                GPIO_SET(gpio_camera);
                GPIO_CLR(gpio_red);
                GPIO_CLR(gpio_green);
                // Also CNN
                PR_INFO("disable cnn");
                enable_cnn = 0;
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
            	PR_INFO("command not supported!");
                //MXC_PCIF_Stop();
                break;
            case QSPI_PACKET_TYPE_VIDEO_ENABLE_FLASH_LED_CMD:
                PR_INFO("enable flash");
                GPIO_CLR(gpio_flash);
                flash_led = 1;
                break;
            case QSPI_PACKET_TYPE_VIDEO_DISABLE_FLASH_LED_CMD:
                PR_INFO("disable flash");
                GPIO_SET(gpio_flash);
                flash_led = 0;
                break;
            case QSPI_PACKET_TYPE_VIDEO_CAMERA_CLOCK_5_MHZ_CMD:
                PR_INFO("camera clock 5mhz");
                camera_clock = 5 * 1000 * 1000;
                MXC_PT_Stop(MXC_F_PTG_ENABLE_PT0);
                MXC_PT_SqrWaveConfig(0, camera_clock);
                MXC_PT_Start(MXC_F_PTG_ENABLE_PT0);
                break;
            case QSPI_PACKET_TYPE_VIDEO_CAMERA_CLOCK_10_MHZ_CMD:
                PR_INFO("camera clock 10mhz");
                camera_clock = 10 * 1000 * 1000;
                MXC_PT_Stop(MXC_F_PTG_ENABLE_PT0);
                MXC_PT_SqrWaveConfig(0, camera_clock);
                MXC_PT_Start(MXC_F_PTG_ENABLE_PT0);
                break;
            case QSPI_PACKET_TYPE_VIDEO_CAMERA_CLOCK_15_MHZ_CMD:
                PR_INFO("camera clock 15mhz");
                camera_clock = 15 * 1000 * 1000;
                MXC_PT_Stop(MXC_F_PTG_ENABLE_PT0);
                MXC_PT_SqrWaveConfig(0, camera_clock);
                MXC_PT_Start(MXC_F_PTG_ENABLE_PT0);
                break;
            case QSPI_PACKET_TYPE_VIDEO_ENABLE_VFLIP_CMD:
                PR_INFO("flip enable");
                camera_vflip = 1;
                camera_set_vflip(camera_vflip);
                break;
            case QSPI_PACKET_TYPE_VIDEO_DISABLE_VFLIP_CMD:
                PR_INFO("flip disable");
                camera_vflip = 0;
                camera_set_vflip(camera_vflip);
                break;
            case QSPI_PACKET_TYPE_VIDEO_DEBUG_CMD:
                PR_INFO("dont sleep for %ds to let debugger connection", MAX78000_SLEEP_DEFER_DURATION);
                enable_sleep = 0;
                MXC_TMR_Start(MAX78000_VIDEO_SLEEP_DEFER_TMR);
                break;
            default:
                PR_ERROR("Invalid packet %d", qspi_rx_header.info.packet_type);
                break;
            }

            qspi_slave_set_rx_state(QSPI_STATE_IDLE);
        }

        if (button_pressed) {
            button_pressed = 0;
            PR_INFO("button A pressed");

            flash_led ^= 1;
            if (flash_led) {
                GPIO_CLR(gpio_flash);
            } else {
                GPIO_SET(gpio_flash);
            }

            qspi_slave_send_packet(NULL, 0, QSPI_PACKET_TYPE_VIDEO_BUTTON_PRESS_RES);
        }

        if (!enable_video) {
            if (enable_sleep) {
                //MXC_LP_EnterSleepMode();
                __WFI();
            }
            continue;
        }

        capture_started_time = GET_RTC_MS();
        // capture image frame for display
        camera_start_capture_image();
        send_img();
        capture_completed_time = GET_RTC_MS();
        qspi_completed_time = GET_RTC_MS();

        if (enable_cnn) {
           	PR_INFO("CNN_EN");
            run_cnn();
        }
        else
          	PR_INFO("CNN_DIS");
            
        cnn_completed_time = GET_RTC_MS();

        if (time_counter % 10 == 0) {
            max78000_statistics.capture_duration_us = (capture_completed_time - capture_started_time) * 1000;
            max78000_statistics.communication_duration_us = (qspi_completed_time - capture_completed_time) * 1000;
            max78000_statistics.cnn_duration_us = cnn_time; //(cnn_completed_time - qspi_completed_time) * 1000;
            max78000_statistics.total_duration_us = (cnn_completed_time - capture_started_time) * 1000;

            PR_DEBUG("Capture : %lu", max78000_statistics.capture_duration_us);
            PR_DEBUG("CNN     : %lu", max78000_statistics.cnn_duration_us);
            PR_DEBUG("QSPI    : %lu", max78000_statistics.communication_duration_us);
            PR_DEBUG("Total   : %lu\n\n", max78000_statistics.total_duration_us);

            PR_INFO("Send Stat");
            qspi_slave_send_packet((uint8_t *) &max78000_statistics, sizeof(max78000_statistics),
                                    QSPI_PACKET_TYPE_VIDEO_STATISTICS_RES);
        }

        time_counter++;
    }
}

static void load_input_camera(int x_offset, int y_offset)
{
#ifdef USE_SAMPLEDATA
    // This function loads the sample data input
    memcpy32((uint32_t*)0x50402000, input_buffer, X_SIZE * Y_SIZE);
#else // Camera
    int cnt = 0;
    uint32_t sample;
    uint32_t *dst;
    uint8_t *data;
    uint8_t *buffer;
    uint8_t r, g, b;

	buffer = (uint8_t*)&camera_image[0];
    dst = (uint32_t *) 0x50402000; 

	// Read 240x240 pixels, pick one out of 3 pixels with offset (9,9) to make 74x74 CNN input
	// CNN needs RGB888, pack into 32bit word
    for (int i = y_offset; i < IMAGE_HEIGHT - y_offset; i += 3) {
        data = buffer + i * CAMERA_WIDTH * LCD_BYTE_PER_PIXEL;  // down

        for(int j = x_offset; j < IMAGE_WIDTH - x_offset; j += 3) {

        	// RGB565, |RRRRRGGG|GGGBBBBB|
            b = (data[j * LCD_BYTE_PER_PIXEL + 1] << 3);
            g = ((data[j * LCD_BYTE_PER_PIXEL] << 5) | ((data[j * LCD_BYTE_PER_PIXEL + 1] & 0xE0) >> 3));
            r = (data[j * LCD_BYTE_PER_PIXEL] & 0xF8);

            // Pack colors into 32bit word and convert to sign value
            sample = ((b << 16) | (g << 8) | r) ^ 0x00808080;
			// Load sample to CNN
			*dst++ = sample;
			cnt++;
        }
    }
	//PR_INFO("Total samples: %d", cnt);
#endif
}


static void send_img(void)
{
int row;
uint8_t* data = NULL;
stream_stat_t* stat;

    // Get image line by line
    for (row = 0; row < CAMERA_HEIGHT; row++) {    
        // Wait until camera streaming buffer is full
        while ((data = get_camera_stream_buffer()) == NULL) {
            if (camera_is_image_rcv()) {
                break;
            }
        };

        // Copy image row to the buffer
        memcpy((uint8_t*)&camera_image[row*CAMERA_WIDTH*LCD_BYTE_PER_PIXEL/4], data, CAMERA_WIDTH*LCD_BYTE_PER_PIXEL);

        // Release stream buffer
        release_camera_stream_buffer();
    }

    // Send captured image to MAX32666
    qspi_slave_send_packet((uint8_t*)camera_image, LCD_DATA_SIZE, QSPI_PACKET_TYPE_VIDEO_DATA_RES);
    stat = get_camera_stream_statistic();

    if (stat->overflow_count > 0) {
        PR_ERROR("OVERFLOW DISP = %d\n", stat->overflow_count);
            //while (1);
    }
    PR_INFO("DMA transfer = %d\n", stat->dma_transfer_count);
}

static void run_cnn(void)
{

#ifdef PRINT_TIME_CNN
    uint32_t pass_time = GET_RTC_MS();
#endif

    // Enable CNN clock
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_CNN);

    cnn_init(); // Bring state machine into consistent state
    //cnn_load_weights(); // No need to reload kernels
    cnn_load_bias(); //load bias
    cnn_configure(); // Configure state machine

#ifdef PRINT_TIME_CNN
    PR_TIMER("CNN init : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif

    load_input_camera(X_OFFSET, Y_OFFSET);
	cnn_start();

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

    MXC_TMR_GetTime(CNN_INFERENCE_TIMER, cnn_time, (uint32_t*) &cnn_time, &units);
	switch (units) {
		case TMR_UNIT_NANOSEC:
			cnn_time /= 1000;
			break;
		case TMR_UNIT_MILLISEC:
			cnn_time *= 1000;
			break;
		case TMR_UNIT_SEC:
			cnn_time *= 1000000;
			break;
		default:
			break;
		}
	
	PR_INFO("load_inference_time: %d us", cnn_time);
	
    // Process CNN output
    get_priors();
    localize_objects();

    cnn_stop();
    // Disable CNN clock to save power
    MXC_SYS_ClockDisable(MXC_SYS_PERIPH_CLOCK_CNN);

#ifdef PRINT_TIME_CNN
    PR_TIMER("CNN unload : %d", GET_RTC_MS() - pass_time);
    pass_time = GET_RTC_MS();
#endif

    //MXC_Delay(MXC_DELAY_MSEC(500));
}

static void nms_memory_init(void)
{
    // Use "camera_image" buffer as a scratch memory to allocate NMS related arrays
    prior_cls_softmax = (uint16_t*)&camera_image[0];
    nms_removed = (uint8_t*)&prior_cls_softmax[NUM_CLASSES * NUM_PRIORS];
    prior_cls = (int8_t*)&nms_removed[(NUM_CLASSES-2) * MAX_PRIORS];
    prior_locs = (int8_t*)&prior_cls[NUM_CLASSES * NUM_PRIORS];
    objects = (uint8_t*)&prior_locs[LOC_DIM * NUM_PRIORS];
}

static int get_prior_idx(int ar_idx, int scale_idx, int rel_idx)
{
    int prior_idx = 0;

    for (int s = 0; s < scale_idx; ++s) {
        prior_idx += NUM_ARS * SQUARE(dims[s]);
    }

    prior_idx += NUM_ARS * rel_idx + ar_idx;
    return prior_idx;
}

static void get_indices(int* ar_idx, int* scale_idx, int* rel_idx, int prior_idx)
{
    int s;

    int prior_count = 0;

    for (s = 0; s < NUM_SCALES; ++s) {
        prior_count += (NUM_ARS * SQUARE(dims[s]));

        if (prior_idx < prior_count) {
            *scale_idx = s;
            break;
        }
    }

    int in_scale_idx = prior_idx;

    for (s = 0; s < *scale_idx; ++s) {
        in_scale_idx -= (NUM_ARS * SQUARE(dims[s]));
    }

    *ar_idx  = in_scale_idx % NUM_ARS;
    *rel_idx = in_scale_idx / NUM_ARS; //SQUARE(dims[*scale_idx]);
}

static void softmax(void)
{
    int i, ch, calc_softmax;
    float sum;

    memset(prior_cls_softmax, 0, 2 * NUM_CLASSES * NUM_PRIORS);

    for (i = 0; i < NUM_PRIORS; ++i) {
        sum          = 0.;
        calc_softmax = 0;

        for (ch = 1; ch < (NUM_CLASSES - 1); ++ch) {
            if (prior_cls[i * NUM_CLASSES + ch] >= prior_cls[i * NUM_CLASSES]) {
                calc_softmax = 1;
                break;
            }
        }

        if (calc_softmax == 0) {
            continue;
        }

        for (ch = 0; ch < (NUM_CLASSES); ++ch) {
            sum += exp(prior_cls[i * NUM_CLASSES + ch] / 128.);
        }

        for (ch = 0; ch < (NUM_CLASSES); ++ch) {
            prior_cls_softmax[i * NUM_CLASSES + ch] =
                (uint16_t)(65536. * exp(prior_cls[i * NUM_CLASSES + ch] / 128.) / sum);
        }
    }
}

static void get_prior_locs(void)
{
    int8_t* loc_addr = (int8_t*)0x50403000;

    int ar_idx, scale_idx, rel_idx, prior_idx, prior_count;

    for (ar_idx = 0; ar_idx < NUM_ARS; ++ar_idx) {
        int8_t* loc_addr_temp = loc_addr;

        for (scale_idx = 0; scale_idx < NUM_SCALES; ++scale_idx) {
            prior_count = SQUARE(dims[scale_idx]);

            for (rel_idx = 0; rel_idx < prior_count; ++rel_idx) {
                prior_idx = get_prior_idx(ar_idx, scale_idx, rel_idx);
                memcpy(&prior_locs[LOC_DIM * prior_idx], loc_addr_temp, LOC_DIM);
                loc_addr_temp += LOC_DIM;
            }
        }

        loc_addr += 0x8000;
    }
}

static void get_prior_cls(void)
{
    int8_t* cl_addr = (int8_t*)0x50803000;

    int ar_idx, cl_idx, scale_idx, rel_idx, prior_idx, prior_count;

    for (ar_idx = 0; ar_idx < NUM_ARS; ++ar_idx) {
        for (cl_idx = 0; cl_idx < NUM_CLASSES; cl_idx += 4) {
            int8_t* cl_addr_temp = cl_addr;

            for (scale_idx = 0; scale_idx < NUM_SCALES; ++scale_idx) {
                prior_count = SQUARE(dims[scale_idx]);

                for (rel_idx = 0; rel_idx < prior_count; ++rel_idx) {
                    prior_idx = get_prior_idx(ar_idx, scale_idx, rel_idx);
                    memcpy(&prior_cls[NUM_CLASSES * prior_idx + cl_idx], cl_addr_temp, 4);
                    cl_addr_temp += 4;
                }
            }

            cl_addr += 0x8000;

            if ((cl_addr == (int8_t*)0x50823000) || (cl_addr == (int8_t*)0x50c23000)) {
                cl_addr += 0x003e0000;
            }
        }
    }

    softmax();
}

static void get_priors(void)
{
    get_prior_locs();
    get_prior_cls();
}

static float calculate_IOU(float* box1, float* box2)
{
    float x_left   = MAX(box1[0], box2[0]);
    float y_top    = MAX(box1[1], box2[1]);
    float x_right  = MIN(box1[2], box2[2]);
    float y_bottom = MIN(box1[3], box2[3]);
    float intersection_area;

    if (x_right < x_left || y_bottom < y_top) {
        return 0.0;
    }

    intersection_area = (x_right - x_left) * (y_bottom - y_top);

    float box1_area = (box1[2] - box1[0]) * (box1[3] - box1[1]);
    float box2_area = (box2[2] - box2[0]) * (box2[3] - box2[1]);

    float iou = (float)(intersection_area) / (float)(box1_area + box2_area - intersection_area);

    return iou;
}

static void get_cxcy(float* cxcy, int prior_idx)
{
    int i, scale_idx, ar_idx, rel_idx, cx, cy;

    get_indices(&ar_idx, &scale_idx, &rel_idx, prior_idx);

    cy      = rel_idx / dims[scale_idx];
    cx      = rel_idx % dims[scale_idx];
    cxcy[0] = (float)((float)(cx + 0.5) / dims[scale_idx]);
    cxcy[1] = (float)((float)(cy + 0.5) / dims[scale_idx]);
    cxcy[2] = scales[scale_idx] * sqrt(ars[ar_idx]);
    cxcy[3] = scales[scale_idx] / sqrt(ars[ar_idx]);

    for (i = 0; i < 4; ++i) {
        cxcy[i] = MAX(0.0, cxcy[i]);
        cxcy[i] = MIN(cxcy[i], 1.0);
    }
}

static void gcxgcy_to_cxcy(float* cxcy, int prior_idx, float* priors_cxcy)
{
    float gcxgcy[4];

    for (int i = 0; i < 4; i++) {
        gcxgcy[i] = (float)prior_locs[4 * prior_idx + i] / 128.0;
    }

    cxcy[0] = priors_cxcy[0] + gcxgcy[0] * priors_cxcy[2] / 10;
    cxcy[1] = priors_cxcy[1] + gcxgcy[1] * priors_cxcy[3] / 10;
    cxcy[2] = exp(gcxgcy[2] / 5) * priors_cxcy[2];
    cxcy[3] = exp(gcxgcy[3] / 5) * priors_cxcy[3];
}

static void cxcy_to_xy(float* xy, float* cxcy)
{
    xy[0] = cxcy[0] - cxcy[2] / 2;
    xy[1] = cxcy[1] - cxcy[3] / 2;
    xy[2] = cxcy[0] + cxcy[2] / 2;
    xy[3] = cxcy[1] + cxcy[3] / 2;
}

static void insert_val(uint16_t val, uint16_t* arr, int arr_len, int idx)
{
    if (arr_len < MAX_PRIORS) {
        arr[arr_len] = arr[arr_len - 1];
    }

    for (int j = (arr_len - 1); j > idx; --j) {
        arr[j] = arr[j - 1];
    }

    arr[idx] = val;
}

static void insert_idx(uint16_t val, uint16_t* arr, int arr_len, int idx)
{
    if (arr_len < MAX_PRIORS) {
        arr[arr_len] = arr[arr_len - 1];
    }

    for (int j = (arr_len - 1); j > idx; --j) {
        arr[j] = arr[j - 1];
    }

    arr[idx] = val;
}

static void insert_nms_prior(uint16_t val, int idx, uint16_t* val_arr, uint16_t* idx_arr, int* arr_len)
{
    if ((*arr_len == 0) || ((val <= val_arr[*arr_len - 1]) && (*arr_len != MAX_PRIORS))) {
        val_arr[*arr_len] = val;
        idx_arr[*arr_len] = idx;
    } else {
        for (int i = 0; i < *arr_len; ++i) {
            if (val > val_arr[i]) {
                insert_val(val, val_arr, *arr_len, i);
                insert_idx(idx, idx_arr, *arr_len, i);
                break;
            }
        }
    }

    *arr_len = MIN((*arr_len + 1), MAX_PRIORS);
}

static void reset_nms(void)
{
    for (int cl = 0; cl < NUM_CLASSES - 2; ++cl) {
        num_nms_priors[cl] = 0;

        for (int p_idx = 0; p_idx < MAX_PRIORS; ++p_idx) {
            nms_scores[cl][p_idx]  = 0;
            nms_indices[cl][p_idx] = 0;
            //nms_removed[cl][p_idx] = 0;
            nms_removed[cl*MAX_PRIORS + p_idx] = 0;
        }
    }
}

static void nms(void)
{
    int prior_idx, class_idx, nms_idx1, nms_idx2, prior1_idx, prior2_idx;
    uint16_t cls_prob;
    float prior_cxcy1[4];
    float prior_cxcy2[4];
    float cxcy1[4];
    float cxcy2[4];
    float xy1[4];
    float xy2[4];

    reset_nms();

    for (prior_idx = 0; prior_idx < NUM_PRIORS; ++prior_idx) {
        for (class_idx = 0; class_idx < (NUM_CLASSES - 2); ++class_idx) {
            cls_prob = prior_cls_softmax[prior_idx * NUM_CLASSES + class_idx + 1];

            if (cls_prob < MIN_CLASS_SCORE) {
                continue;
            }

            insert_nms_prior(cls_prob, prior_idx, nms_scores[class_idx], nms_indices[class_idx], &num_nms_priors[class_idx]);
        }
    }

    for (class_idx = 0; class_idx < (NUM_CLASSES - 2); ++class_idx) {
        for (nms_idx1 = 0; nms_idx1 < num_nms_priors[class_idx]; ++nms_idx1) {
            //if (nms_removed[class_idx][nms_idx1] != 1 &&
            if (nms_removed[class_idx * num_nms_priors[class_idx] + nms_idx1] != 1 &&
                nms_idx1 != num_nms_priors[class_idx] - 1) {
                for (nms_idx2 = nms_idx1 + 1; nms_idx2 < num_nms_priors[class_idx]; ++nms_idx2) {
                    prior1_idx = nms_indices[class_idx][nms_idx1];
                    prior2_idx = nms_indices[class_idx][nms_idx2];

                    // TODO: Let's implement the box loc finding before this nested loop for 100 priors
                    get_cxcy(prior_cxcy1, prior1_idx);
                    get_cxcy(prior_cxcy2, prior2_idx);

                    gcxgcy_to_cxcy(cxcy1, prior1_idx, prior_cxcy1);
                    gcxgcy_to_cxcy(cxcy2, prior2_idx, prior_cxcy2);

                    cxcy_to_xy(xy1, cxcy1);
                    cxcy_to_xy(xy2, cxcy2);

                    float iou = calculate_IOU(xy1, xy2);

                    if (iou > MAX_ALLOWED_OVERLAP) {
                        //nms_removed[class_idx][nms_idx2] = 1;
                        nms_removed[class_idx * num_nms_priors[class_idx] + nms_idx2] = 1;
                    }
                }
            }
        }
    }
}

static void localize_objects(void)
{
    float prior_cxcy[4];
    float cxcy[4];
    float xy[4];
    int class_idx, prior_idx, global_prior_idx;
    uint8_t obj_number = 0; 

    nms();

    for (class_idx = 0; class_idx < (NUM_CLASSES - 2); ++class_idx) {
        for (prior_idx = 0; prior_idx < num_nms_priors[class_idx]; ++prior_idx) {
            //if (nms_removed[class_idx][prior_idx] != 1) {
            if (nms_removed[class_idx * num_nms_priors[class_idx] + prior_idx] != 1) {
                global_prior_idx = nms_indices[class_idx][prior_idx];
                get_cxcy(prior_cxcy, global_prior_idx);
                gcxgcy_to_cxcy(cxcy, global_prior_idx, prior_cxcy);
                cxcy_to_xy(xy, cxcy);

                PR_INFO("class: %d, prior_idx: %d, prior: %d, x1: %.2f, y1: %.2f, x2: %.2f, y2: "
                       "%.2f \n",
                       class_idx + 1, prior_idx, global_prior_idx, xy[0], xy[1], xy[2], xy[3]);
                
                // Save objects box coordinates
                objects[obj_number*ML_DATA_SIZE + 1] = class_idx;
                objects[obj_number*ML_DATA_SIZE + 2] = (uint8_t)IMG_SCALE*X_SIZE*xy[0]; // x1
                objects[obj_number*ML_DATA_SIZE + 3] = (uint8_t)IMG_SCALE*Y_SIZE*xy[1]; // y1
                objects[obj_number*ML_DATA_SIZE + 4] = (uint8_t)IMG_SCALE*X_SIZE*xy[2]; // x2
                objects[obj_number*ML_DATA_SIZE + 5] = (uint8_t)IMG_SCALE*Y_SIZE*xy[3]; // y2
                obj_number++;
            }
        }
    }

    if(obj_number) {    
        // First element is number of detected objects
        objects[0] = obj_number;
        // Send result to MAX32666
        qspi_slave_send_packet(&objects[0], ML_DATA_SIZE*obj_number + 1, QSPI_PACKET_TYPE_VIDEO_ML_RES);
    }
}
