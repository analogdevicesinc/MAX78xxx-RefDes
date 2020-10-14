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
#include <stdio.h>
#include <stdint.h>
#include "board.h"
#include "mxc_delay.h"
#include "camera.h"
#include <mxc.h>
#include "icc.h"
#include "rtc.h"
#include "gpio.h"
#include "spi.h"

#include "spi_dma.h"
#include "faceid_definitions.h"
#include "version.h"
#include "embedding_process.h"
#include "maxcam_debug.h"
#include "faceID.h"
#include "weights.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "main"

#define IMAGE_XRES  240
#define IMAGE_YRES  240

#define HEIGHT          160
#define WIDTH           120
#define THICKNESS       4
#define IMAGE_H         240
#define IMAGE_W         240
#define BYTE_PER_PIXEL  2
#define FRAME_COLOR_DARK    0x535A
#define FRAME_COLOR_LIGHT   0xFFFF

//#define PRINT_TIME

/* Peripherals */
#define GPIO_SET(x)         MXC_GPIO_OutSet(x.port, x.mask)
#define GPIO_CLR(x)         MXC_GPIO_OutClr(x.port, x.mask)

#define QSPI_ID             MXC_SPI0

#define DMA_CHANNEL_CAMERA          0
#define DMA_CHANNEL_CAMERA_IRQ      DMA0_IRQn
#define DMA_CHANNEL_CAMERA_IRQ_HAND DMA0_IRQHandler

#define DMA_CHANNEL_QSPI            1
#define DMA_CHANNEL_QSPI_IRQ        DMA1_IRQn
#define DMA_CHANNEL_QSPI_IRQ_HAND   DMA1_IRQHandler


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
mxc_gpio_cfg_t gpio_flash  = {MXC_GPIO0, MXC_GPIO_PIN_19, MXC_GPIO_FUNC_OUT,
                              MXC_GPIO_PAD_PULL_DOWN, MXC_GPIO_VSSEL_VDDIO};

mxc_gpio_cfg_t gpio_camera = {MXC_GPIO0, MXC_GPIO_PIN_3, MXC_GPIO_FUNC_OUT,
                              MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};

mxc_gpio_cfg_t qspi_int    = {MXC_GPIO0, MXC_GPIO_PIN_12, MXC_GPIO_FUNC_OUT,
                              MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};

mxc_gpio_cfg_t gpio_red    = {MXC_GPIO2, MXC_GPIO_PIN_0, MXC_GPIO_FUNC_OUT,
                              MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};

mxc_gpio_cfg_t gpio_green  = {MXC_GPIO2, MXC_GPIO_PIN_1, MXC_GPIO_FUNC_OUT,
                              MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};

mxc_gpio_cfg_t gpio_blue   = {MXC_GPIO2, MXC_GPIO_PIN_2, MXC_GPIO_FUNC_OUT,
                              MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};

uint8_t embedding_result[512];

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
    {0x30, 0x05},
    {0x31, 0x03},
    {0x37, 0x0C}, // PCLK is double system clock (default is 0x0c)
    {0x38, 0x10},
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
    {0x11, 0xC0}, // CLKRC, Internal clock pre-scalar divide by 2 (default divide by 1)
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
    {0x11, 0xc0},   // Set clock prescaler
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


#ifdef PRINT_TIME
    uint32_t timer_counter = 0;
#define PR_TIMER(fmt, args...)    if(timer_counter%10==0) printf("T[%-10s:%4d] " fmt "\r\n", S_MODULE_NAME, __LINE__, ##args )
#endif


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static void fail(void);
static void process_img(void);
static void run_cnn(int x_offset, int y_offset);
static void run_demo(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void DMA_CHANNEL_CAMERA_IRQ_HAND(void)
{
}

void DMA_CHANNEL_QSPI_IRQ_HAND(void)
{
    spi_dma_int_handler(DMA_CHANNEL_QSPI, QSPI_ID);
}

uint32_t utils_get_time_ms(void)
{
    int sec;
    double subsec;
    uint32_t ms;

    subsec = MXC_RTC_GetSubSecond() / 4096.0;
    sec = MXC_RTC_GetSecond();

    ms = (sec*1000) +  (int)(subsec*1000);

    return ms;
}

int main(void)
{
    int ret = 0;
    int slaveAddress;
    int id;

    // Enable camera
    GPIO_CLR(gpio_camera);
    MXC_GPIO_Config(&gpio_camera);

    // Configure flash pin
    MXC_GPIO_Config(&gpio_flash);

    // Configure SPI int pin
    GPIO_SET(qspi_int);
    MXC_GPIO_Config(&qspi_int);

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

    PR_INFO("\nmaxcam_electronica_max78000_video v%d.%d.%d\n", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);

    if (initCNN() < 0 ) {
        PR_ERROR("Could not initialize the CNN accelerator");
        fail();
    }

    if (init_database() < 0 ) {
        PR_ERROR("Could not initialize the database");
        fail();
    }

    /* Initialize RTC */
    MXC_RTC_Init(0, 0);
    MXC_RTC_Start();

    mxc_spi_pins_t qspi_pins;
    qspi_pins.clock = TRUE;
    qspi_pins.miso = TRUE;
    qspi_pins.mosi = TRUE;
    qspi_pins.sdio2 = TRUE;
    qspi_pins.sdio3 = TRUE;
    qspi_pins.ss0 = TRUE;
    qspi_pins.ss1 = FALSE;
    qspi_pins.ss2 = FALSE;

    spi_dma_slave_init(QSPI_ID, qspi_pins);

    if (MXC_DMA_Init() != E_NO_ERROR) {
        PR_ERROR("DMA INIT ERROR");
        fail();
    }

//    NVIC_EnableIRQ(DMA_CHANNEL_CAMERA_IRQ);
    NVIC_EnableIRQ(DMA_CHANNEL_QSPI_IRQ);

    // Initialize the camera driver.
    if (camera_init() != E_NO_ERROR) {
        PR_ERROR("Camera init failed!");
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
    ret = camera_setup(IMAGE_XRES, IMAGE_YRES, PIXFORMAT_RGB565, FIFO_FOUR_BYTE, USE_DMA, DMA_CHANNEL_CAMERA);
    if (ret != STATUS_OK) {
        PR_ERROR("Error returned from setting up camera. Error : %d", ret);
        fail();
    }

    // Successfully initialize the program
    PR_INFO("Program initialized successfully");

    GPIO_SET(gpio_green);

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
    camera_start_capture_image();
    uint32_t run_count = 0;
#ifdef PRINT_TIME
    /* Get current time */
    uint32_t process_time = utils_get_time_ms();
    uint32_t total_time = utils_get_time_ms();
#endif

    while (1) { //Capture image and run CNN

        if (camera_is_image_rcv()) { // Check whether image is ready

#ifdef PRINT_TIME
            process_time = utils_get_time_ms();
#endif
            run_cnn(0, 0);
            if ((run_count % 2) == 0){
                run_cnn(-10, -10);
                run_cnn(10, 10);
            } else {
                run_cnn(-10, 10);
                run_cnn(10, -10);
            }
            run_count++;

            process_img();


#ifdef PRINT_TIME
            PR_TIMER("Process Time Total : %dms", utils_get_time_ms()-process_time);
#endif

            camera_start_capture_image();

#ifdef PRINT_TIME
            PR_TIMER("Capture Time : %dms", process_time - total_time);
            PR_TIMER("Total Time : %dms\n\n\n", utils_get_time_ms()-total_time);
            total_time = utils_get_time_ms();
            timer_counter++;
#endif

        }
    }
}

static void process_img(void)
{
    uint8_t   *raw;
    uint32_t  imgLen;
    uint32_t  w, h;

    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &imgLen, &w, &h);

#ifdef PRINT_TIME
    uint32_t pass_time = 0;
    pass_time = utils_get_time_ms();
#endif

    spi_dma_send_packet(DMA_CHANNEL_QSPI, QSPI_ID, raw, imgLen,
            QSPI_TYPE_RESPONSE_VIDEO_DATA, &qspi_int);

#ifdef PRINT_TIME
    PR_TIMER("QSPI transfer duration : %d", utils_get_time_ms() - pass_time);
#endif
}

static void run_cnn(int x_offset, int y_offset)
{
    uint8_t   *raw;
    uint32_t  imgLen;
    uint32_t  w, h;

    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &imgLen, &w, &h);

#ifdef PRINT_TIME
    uint32_t pass_time = utils_get_time_ms();
#endif

    cnn_load();

    cnn_start();

    uint8_t * data = raw;

#ifdef PRINT_TIME
    PR_TIMER("CNN initialization duration : %d", utils_get_time_ms() - pass_time);
    pass_time = utils_get_time_ms();
#endif

//    int counter = 0;

    data =  raw + ((IMAGE_H - (HEIGHT))/2)*IMAGE_W*BYTE_PER_PIXEL;
    for (int i = y_offset; i<HEIGHT+y_offset; i++) {
        data =  raw + (((IMAGE_H - HEIGHT)/2)+i)*IMAGE_W*BYTE_PER_PIXEL;
        data += ((IMAGE_W - WIDTH)/2)*BYTE_PER_PIXEL;
        for(int j =x_offset; j< WIDTH+x_offset; j++) {
            uint8_t ur,ug,ub;
            int8_t r,g,b;
            uint32_t number;

            ub = (uint8_t)(data[j*BYTE_PER_PIXEL+1]<<3);
            ug = (uint8_t)((data[j*BYTE_PER_PIXEL]<<5) | ((data[j*BYTE_PER_PIXEL+1]&0xE0)>>3));
            ur = (uint8_t)(data[j*BYTE_PER_PIXEL]&0xF8);

//            raw[counter++] = ur;
//            raw[counter++] = ug;
//            raw[counter++] = ub;

            b = ub - 128;
            g = ug - 128;
            r = ur - 128;

            // Loading data into the CNN fifo
            while (((*((volatile uint32_t *) 0x50000004) & 1)) != 0); // Wait for FIFO 0

            number = 0x00FFFFFF & ((((uint8_t)b)<<16) | (((uint8_t)g)<<8) | ((uint8_t)r));

            *((volatile uint32_t *) 0x50000008) = number; // Write FIFO 0
        }
    }

#ifdef PRINT_TIME
    PR_TIMER("CNN load data duration : %d", utils_get_time_ms() - pass_time);
    pass_time = utils_get_time_ms();
#endif
    cnn_wait();

#ifdef PRINT_TIME
    PR_TIMER("CNN wait duration : %d", utils_get_time_ms() - pass_time);
    pass_time = utils_get_time_ms();
#endif

    cnn_unload(embedding_result);

#ifdef PRINT_TIME
    PR_TIMER("CNN unload duration : %d", utils_get_time_ms() - pass_time);
    pass_time = utils_get_time_ms();
#endif

    int pResult = calculate_minDistance(embedding_result);

#ifdef PRINT_TIME
    PR_TIMER("Embedding calculation duration : %d", utils_get_time_ms() - pass_time);
#endif

    if ( pResult == 0 ) {
        char name[RESULT_MAX_SIZE] = "nothing";
        uint8_t *counter;
        uint8_t counter_len;
        get_min_dist_counter(&counter, &counter_len);

        prev_decision = decision;
        decision = -3;
        for(uint8_t id=0; id<counter_len; ++id){
            if (counter[id] >= (closest_sub_buffer_size-4)){
                strncpy(name, get_subject(id), sizeof(name) - 1);
                decision = id;
                break;
            } else if (counter[id] >= (closest_sub_buffer_size/2+1)){
                strncpy(name, "Adjust Face", sizeof(name) - 1);
                decision = -2;
                break;
            } else if (counter[id] > 4){
                strncpy(name, "Unknown", sizeof(name) - 1);
                decision = -1;
                break;
            }
        }

        if(decision != prev_decision){
            spi_dma_send_packet(DMA_CHANNEL_QSPI, QSPI_ID, (uint8_t *)name,
                    strlen(name), QSPI_TYPE_RESPONSE_VIDEO_RESULT, &qspi_int);
            PR_DEBUG("Result : %s\n", name);
        }
    }
}
