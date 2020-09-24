/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
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
******************************************************************************/

/**
 * @file    main.c
 * @brief   FaceID Electronica 2020 Demo
 *
 * @details
 *
 */

#define S_MODULE_NAME   "Main"

/***** Includes *****/
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

#include "faceid_definitions.h"
#include "version.h"
#include "embedding_process.h"
#include "utils.h"
#include "AI85_Debug.h"
#include "faceID.h"
#include "weights.h"

#define IMAGE_XRES  240
#define IMAGE_YRES  240

#define HEIGHT          160
#define WIDTH           120
#define THICKNESS       4
#define IMAGE_H         240
#define IMAGE_W         240
#define BYTE_PER_PIXEL  2
#define FRAME_COLOR     0x535A

#define PRINT_TIME 0

#define GPIO_SET(x)         MXC_GPIO_OutSet(x.port, x.mask)
#define GPIO_CLR(x)         MXC_GPIO_OutClr(x.port, x.mask)

static void fail(void);
static void process_img(void);
static void run_cnn(int x_offset, int y_offset);
static void run_demo(void);
static void send_image_to_me14(uint8_t *image, uint32_t len);
static void send_result_to_me14(char *result, uint32_t len);

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

volatile uint8_t DMA_FLAG = 0;

static int8_t prev_decision = -2;
static int8_t decision = -2;

mxc_gpio_cfg_t gpio_flash;

void SPI0_IRQHandler(void)
{
    MXC_SPI_AsyncHandler(MXC_SPI0);
}

void DMA0_IRQHandler(void)
{
    MXC_DMA_Handler();
}

void DMA1_IRQHandler(void)
{
    MXC_DMA_Handler();
    DMA_FLAG = 1;
}

mxc_gpio_cfg_t gpio_cs;

mxc_gpio_cfg_t gpio_red;
mxc_gpio_cfg_t gpio_green;
mxc_gpio_cfg_t gpio_blue;

int main(void)
{
    int ret = 0;
    int slaveAddress;
    int id;

    PR_INFO("FaceID Electronica Demo v%d.%d.%d\n", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);
    mxc_gpio_cfg_t gpio_camera;



    // Enable camera
    gpio_camera.port = MXC_GPIO0;
    gpio_camera.mask = MXC_GPIO_PIN_3;
    gpio_camera.pad = MXC_GPIO_PAD_NONE;
    gpio_camera.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_camera);
    GPIO_CLR(gpio_camera);


    // Configure LEDs
    gpio_red.port = MXC_GPIO2;
    gpio_red.mask = MXC_GPIO_PIN_0;
    gpio_red.pad = MXC_GPIO_PAD_NONE;
    gpio_red.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_red);
    GPIO_SET(gpio_red);

    gpio_green.port = MXC_GPIO2;
    gpio_green.mask = MXC_GPIO_PIN_1;
    gpio_green.pad = MXC_GPIO_PAD_NONE;
    gpio_green.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_green);
    GPIO_SET(gpio_green);

    gpio_blue.port = MXC_GPIO2;
    gpio_blue.mask = MXC_GPIO_PIN_2;
    gpio_blue.pad = MXC_GPIO_PAD_NONE;
    gpio_blue.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_blue);
    GPIO_SET(gpio_blue);

    MXC_Delay(500*1000);


    gpio_flash.port = MXC_GPIO0;
    gpio_flash.mask = MXC_GPIO_PIN_19;
    gpio_flash.pad = MXC_GPIO_PAD_PULL_DOWN;
    gpio_flash.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_flash);


    GPIO_SET(gpio_red);
    GPIO_SET(gpio_green);
    GPIO_SET(gpio_blue);
    while(1) {

    	GPIO_CLR(gpio_flash);
        GPIO_CLR(gpio_green);
        GPIO_CLR(gpio_blue);
    	MXC_Delay(5000);
    	GPIO_SET(gpio_flash);
        GPIO_SET(gpio_green);
        GPIO_SET(gpio_blue);
        MXC_Delay(5000);

        PR_INFO("*");
    }




    gpio_cs.port = MXC_GPIO0;
    gpio_cs.mask = MXC_GPIO_PIN_4;
    gpio_cs.pad = MXC_GPIO_PAD_NONE;
    gpio_cs.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_cs);
    GPIO_SET(gpio_cs);

    gpio_flash.port = MXC_GPIO0;
    gpio_flash.mask = MXC_GPIO_PIN_19;
    gpio_flash.pad = MXC_GPIO_PAD_NONE;
    gpio_flash.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_flash);

    /* Enable cache */
    MXC_ICC_Enable(MXC_ICC0);

    /* Set system clock to 100 MHz */
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

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

    mxc_spi_pins_t spi_pins;

    spi_pins.clock = TRUE;
    spi_pins.miso = TRUE;
    spi_pins.mosi = TRUE;
    spi_pins.sdio2 = TRUE;
    spi_pins.sdio3 = TRUE;
    spi_pins.ss0 = FALSE;
    spi_pins.ss1 = FALSE;
    spi_pins.ss2 = FALSE;

    // Configure the peripheral
    if (MXC_SPI_Init(MXC_SPI0, 1, 0, 0, 0, QSPI_SPEED, spi_pins) != E_NO_ERROR) {
        PR_ERROR("SPI Initialization failed!");
        fail();
    }

    if (MXC_SPI_SetDataSize(MXC_SPI0, 8) != E_NO_ERROR) {
        PR_ERROR("SPI Set data size failed!");
        fail();
    }

    if (MXC_SPI_SetWidth(MXC_SPI0, SPI_WIDTH_QUAD) != E_NO_ERROR) {
        PR_ERROR("SPI Set width failed!");
        fail();
    }

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
    ret = camera_setup(IMAGE_XRES, IMAGE_YRES, PIXFORMAT_RGB565, FIFO_FOUR_BYTE, USE_DMA);
    if (ret != STATUS_OK) {
        PR_ERROR("Error returned from setting up camera. Error : %d", ret);
        fail();
    }

    // Successfully initialize the program
    PR_INFO("Program initialized successfully");
    GPIO_CLR(gpio_red);
    GPIO_CLR(gpio_blue);
    GPIO_SET(gpio_green);

    MXC_Delay(500*1000);

    GPIO_CLR(gpio_green);
    GPIO_SET(gpio_blue);

    run_demo();

    return 0;
}

static void fail(void)
{
    GPIO_SET(gpio_red);
    GPIO_CLR(gpio_green);
    GPIO_CLR(gpio_blue);
    while(1);
}

static void run_demo(void)
{
    camera_start_capture_image();
    uint32_t run_count = 0;
#if (PRINT_TIME==1)
    /* Get current time */
    uint32_t process_time = utils_get_time_ms();
    uint32_t total_time = utils_get_time_ms();
#endif

    while (1) { //Capture image and run CNN

        if (camera_is_image_rcv()) { // Check whether image is ready

#if (PRINT_TIME==1)
            process_time = utils_get_time_ms();
#endif
            process_img();

            run_cnn(0, 0);
            if ((run_count % 2) == 0){
                run_cnn(-10, -10);
                run_cnn(10, 10);
            } else {
                run_cnn(-10, 10);
                run_cnn(10, -10);
            }
            run_count++;

#if (PRINT_TIME==1)
            PR_INFO("Process Time Total : %dms", utils_get_time_ms()-process_time);
            printf("\n\n\n");
#endif

            camera_start_capture_image();

#if (PRINT_TIME==1)
            PR_INFO("Capture Time : %dms", process_time - total_time);
            PR_INFO("Total Time : %dms", utils_get_time_ms()-total_time);
            total_time = utils_get_time_ms();
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

    // Send the image through the UART to the console.
    // A python program will read from the console and write to an image file.
//  utils_send_img_to_pc(raw, imgLen, w, h, camera_get_pixel_format());

    uint16_t *image = (uint16_t*)raw;   // 2bytes per pixel RGB565

#if (PRINT_TIME==1)
    uint32_t pass_time = 0;
    pass_time = utils_get_time_ms();
#endif

    // left line
    image+=((IMAGE_H - (HEIGHT+2*THICKNESS))/2)*IMAGE_W;
    for (int i = 0; i<THICKNESS; i++) {
        image+=((IMAGE_W - (WIDTH+2*THICKNESS))/2);
        for(int j=0; j< WIDTH+2*THICKNESS; j++) {
            *(image++) = FRAME_COLOR; //color
        }
        image+=((IMAGE_W - (WIDTH+2*THICKNESS))/2);
    }

    //right line
    image = ((uint16_t*)raw) + (((IMAGE_H - (HEIGHT+2*THICKNESS))/2) + HEIGHT + THICKNESS )*IMAGE_W;
    for (int i = 0; i<THICKNESS; i++) {
        image+=((IMAGE_W - (WIDTH+2*THICKNESS))/2);
        for(int j =0; j< WIDTH+2*THICKNESS; j++) {
            *(image++) = FRAME_COLOR; //color
        }
        image+=((IMAGE_W - (WIDTH+2*THICKNESS))/2);
    }

    //top + bottom lines
    image = ((uint16_t*)raw) + ((IMAGE_H - (HEIGHT+2*THICKNESS))/2)*IMAGE_W;
    for (int i = 0; i<HEIGHT+2*THICKNESS; i++) {
        image+=((IMAGE_W - (WIDTH+2*THICKNESS))/2);
        for(int j =0; j< THICKNESS; j++) {
            *(image++) = FRAME_COLOR; //color
        }
        image+=WIDTH;
        for(int j =0; j< THICKNESS; j++) {
            *(image++) = FRAME_COLOR; //color
        }
        image+=((IMAGE_W - (WIDTH+2*THICKNESS))/2);
    }

#if (PRINT_TIME==1)
    PR_INFO("Frame drawing time : %d", utils_get_time_ms() - pass_time);

    pass_time = utils_get_time_ms();

    PR_INFO("Screen print time : %d", utils_get_time_ms() - pass_time);
#endif

//    utils_send_img_to_pc(raw, imgLen, w, h, (uint8_t*)"RGB565");

    GPIO_SET(gpio_blue);
    send_image_to_me14(raw, imgLen);
    GPIO_CLR(gpio_blue);

}



static void run_cnn(int x_offset, int y_offset)
{
    uint8_t   *raw;
    uint32_t  imgLen;
    uint32_t  w, h;

    // Get the details of the image from the camera driver.
    camera_get_image(&raw, &imgLen, &w, &h);

#if (PRINT_TIME==1)
    /* Get current time */
    uint32_t pass_time = utils_get_time_ms();
#endif

    cnn_load();

    cnn_start();

    uint8_t * data = raw;

#if (PRINT_TIME==1)
    PR_INFO("CNN initialization time : %d", utils_get_time_ms() - pass_time);
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

//    utils_send_img_to_pc(raw, 120*160*3, 120, 160, (uint8_t*)"RGB888");

#if (PRINT_TIME==1)
    PR_INFO("CNN load data time : %d", utils_get_time_ms() - pass_time);
    pass_time = utils_get_time_ms();
#endif
    cnn_wait();

#if (PRINT_TIME==1)
    PR_INFO("CNN wait time : %d", utils_get_time_ms() - pass_time);
    pass_time = utils_get_time_ms();
#endif

    cnn_unload((uint8_t*)(raw));

#if (PRINT_TIME==1)
    PR_INFO("CNN unload time : %d", utils_get_time_ms() - pass_time);
    pass_time = utils_get_time_ms();
#endif

    int pResult = calculate_minDistance((uint8_t*)(raw));

#if (PRINT_TIME==1)
    PR_INFO("Embedding time : %d", utils_get_time_ms() - pass_time);
#endif

    if ( pResult == 0 ) {
            char *name;

            uint8_t *counter;
            uint8_t counter_len;
            get_min_dist_counter(&counter, &counter_len);

            name = "";
            prev_decision = decision;
            decision = -3;
            for(uint8_t id=0; id<counter_len; ++id){
                if (counter[id] >= (closest_sub_buffer_size-4)){
                    name = get_subject(id);
                    decision = id;
                    break;
                } else if (counter[id] >= (closest_sub_buffer_size/2+1)){
                    name = "Adjust Face";
                    decision = -2;
                    break;
                } else if (counter[id] > 4){
                    name = "Unknown";
                    decision = -1;
                    break;
                }
            }

            if(decision != prev_decision){
                if (strlen(name) == 0) {
                    name = " ";
                }
                send_result_to_me14(name, strlen(name));
                PR_DEBUG("Result : %s\n", name);
            }
        }

}

static void send_image_to_me14(uint8_t *image, uint32_t len)
{
    mxc_spi_req_t req;

    qspi_header_t header;

    header.start_symbol = QSPI_START_SYMBOL;
    header.data_len = len;
    header.command = QSPI_COMMAND_IMAGE;
    //SPI Request
    req.spi = MXC_SPI0;
    req.txData = (uint8_t *)&header;
    req.rxData = NULL;
    req.txLen = sizeof(qspi_header_t);
    req.rxLen = 0;
    req.ssIdx = 0;
    req.ssDeassert = 0;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.completeCB = NULL;

    GPIO_CLR(gpio_cs);

    MXC_SPI_MasterTransaction(&req);

    GPIO_SET(gpio_cs);

    MXC_Delay(100); //100usec delay

    //SPI Request
    req.txData = image;
    req.txLen = len/2;
    req.txCnt = 0;

    GPIO_CLR(gpio_cs);

    MXC_SPI_MasterTransaction(&req);

    GPIO_SET(gpio_cs);

    MXC_Delay(100); //100usec delay

    GPIO_CLR(gpio_cs);

    //SPI Request
    req.txData = (image+(len/2));
    req.txLen = len - len/2;
    req.txCnt = 0;

    MXC_SPI_MasterTransaction(&req);

    GPIO_SET(gpio_cs);
}

static void send_result_to_me14(char *result, uint32_t len)
{
    mxc_spi_req_t req;

    qspi_header_t header;

    header.start_symbol = QSPI_START_SYMBOL;
    header.data_len = len;
    header.command = QSPI_COMMAND_RESULT;

    //SPI Request
    req.spi = MXC_SPI0;
    req.txData = (uint8_t *)&header;
    req.rxData = NULL;
    req.txLen = sizeof(qspi_header_t);
    req.rxLen = 0;
    req.ssIdx = 1;
    req.ssDeassert = 1;
    req.txCnt = 0;
    req.rxCnt = 0;
    req.completeCB = NULL;

    GPIO_CLR(gpio_cs);

    MXC_SPI_MasterTransaction(&req);

    GPIO_SET(gpio_cs);

    MXC_Delay(100); //100usec delay

    //SPI Request
    req.txData = (uint8_t *)result;
    req.txLen = len;
    req.txCnt = 0;

    GPIO_CLR(gpio_cs);

    MXC_SPI_MasterTransaction(&req);

    GPIO_SET(gpio_cs);
}
