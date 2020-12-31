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

#ifndef _MAXREFDES178_DEFINTIIONS_H_
#define _MAXREFDES178_DEFINTIIONS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdint.h>


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
/*** Common ***/
// Common SPI
#define QSPI_SPEED                         10000000UL  // hz
#define QSPI_START_SYMBOL                  0xAABBCCDD

#define SPI_TIMEOUT_CNT                    10000000  // loop counter

// Common I2C
#define I2C_SPEED                          100000  // hz

#define I2C_ADDR_MAX20303_PMIC             0x50
#define I2C_ADDR_MAX20303_FUEL_GAUGE       0x6C
#define I2C_ADDR_MAX7325_PORTS             0xD2
#define I2C_ADDR_MAX7325_OUTPUTS           0xB2
#define I2C_ADDR_BMI160                    0xD0
#define I2C_ADDR_MAX34417                  0x10

// Common LCD
#define LCD_WIDTH                          240
#define LCD_HEIGHT                         240
#define LCD_BYTE_PER_PIXEL                 2  // RGB565
#define LCD_DATA_SIZE                      (LCD_WIDTH * LCD_HEIGHT * LCD_BYTE_PER_PIXEL)
#define LCD_SUBTITLE_SIZE                  32
#define LCD_TOPTITLE_SIZE                  32
#define LCD_NOTIFICATION_SIZE              40
#define LCD_ROTATION                       2  // 0-3

#define LCD_CLASSIFICATION_DURATION        1000  // ms
#define LCD_NO_VIDEO_DURATION              300   // ms
#define LCD_NOTIFICATION_DURATION          7000  // ms

// Common Camera
#define CAMERA_WIDTH                       LCD_WIDTH
#define CAMERA_HEIGHT                      LCD_HEIGHT
#define CAMERA_FORMAT                      PIXFORMAT_RGB565

// Common FaceID
#define FACEID_WIDTH                       120
#define FACEID_HEIGHT                      160

#define FACEID_RECTANGLE_X1                ((LCD_WIDTH - FACEID_WIDTH) / 2)
#define FACEID_RECTANGLE_Y1                ((LCD_HEIGHT - FACEID_HEIGHT) / 2)
#define FACEID_RECTANGLE_X2                FACEID_RECTANGLE_X1 + FACEID_WIDTH
#define FACEID_RECTANGLE_Y2                FACEID_RECTANGLE_Y1 + FACEID_HEIGHT

// Common BLE
#define BLE_MAX_MTU_SIZE                   256
#define BLE_MAX_MTU_REQUEST_SIZE           (BLE_MAX_MTU_SIZE - 4)
#define BLE_MAX_PACKET_SIZE                (BLE_MAX_MTU_REQUEST_SIZE - 3)

// Common communication
#define COMMUNICATION_MAX_PACKET_SIZE      BLE_MAX_PACKET_SIZE
#define COMMUNICATION_STATISTICS_INTERVAL  1000  // ms

/*** MAX32666 ***/
// MAX32666 PINS
#define MAX32666_VIDEO_INT_PIN             {MXC_GPIO0, MXC_GPIO_PIN_30, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO}
#define MAX32666_VIDEO_INT_MODE            MXC_GPIO_INT_FALLING
#define MAX32666_VIDEO_CS_PIN              {MXC_GPIO0, MXC_GPIO_PIN_8, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX32666_AUDIO_INT_PIN             {MXC_GPIO1, MXC_GPIO_PIN_13, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO}
#define MAX32666_AUDIO_INT_MODE            MXC_GPIO_INT_FALLING
#define MAX32666_AUDIO_CS_PIN              {MXC_GPIO0, MXC_GPIO_PIN_14, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX32666_LCD_DC_PIN                {MXC_GPIO1, MXC_GPIO_PIN_14, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO}
#define MAX32666_LCD_RESET_PIN             {MXC_GPIO1, MXC_GPIO_PIN_15, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO}
#define MAX32666_LCD_CS_PIN                {MXC_GPIO0, MXC_GPIO_PIN_22, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIO}
#define MAX32666_LCD_TE_PIN                {MXC_GPIO0, MXC_GPIO_PIN_31, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO
#define MAX32666_LCD_TOUCH_INT_PIN         {MXC_GPIO1, MXC_GPIO_PIN_10, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO
#define MAX32666_LCD_TOUCH_INT_MODE        MXC_GPIO_INT_FALLING                                                                     // TODO

#define MAX32666_SD_EN_PIN                 {MXC_GPIO1, MXC_GPIO_PIN_12, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX32666_CORE0_INT_PIN             {MXC_GPIO0, MXC_GPIO_PIN_16, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO
#define MAX32666_CORE0_INT_MODE            MXC_GPIO_INT_FALLING                                                                     // TODO
#define MAX32666_CORE1_INT_PIN             {MXC_GPIO0, MXC_GPIO_PIN_18, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO} // TODO

#define MAX32666_VIDEO_IO_PIN              {MXC_GPIO0, MXC_GPIO_PIN_20, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO} // TODO
#define MAX32666_AUDIO_IO_PIN              {MXC_GPIO0, MXC_GPIO_PIN_21, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO} // TODO

#define MAX32666_EXPANDER_INT_PIN          {MXC_GPIO0, MXC_GPIO_PIN_15, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO
#define MAX32666_EXPANDER_INT_MODE         MXC_GPIO_INT_FALLING                                                                     // TODO

#define MAX32666_CODEC_INT_PIN             {MXC_GPIO0, MXC_GPIO_PIN_23, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO
#define MAX32666_CODEC_INT_MODE            MXC_GPIO_INT_FALLING                                                                     // TODO

#define MAX32666_BUTTON1_INT_PIN           {MXC_GPIO1, MXC_GPIO_PIN_6, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}   // TODO
#define MAX32666_BUTTON1_INT_MODE          MXC_GPIO_INT_FALLING                                                                     // TODO

#define MAX32666_BUTTON2_INT_PIN           {MXC_GPIO1, MXC_GPIO_PIN_8, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}   // TODO
#define MAX32666_BUTTON2_INT_MODE          MXC_GPIO_INT_FALLING                                                                     // TODO

#define MAX32666_ACCEL_INT_PIN             {MXC_GPIO1, MXC_GPIO_PIN_11, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO
#define MAX32666_ACCEL_INT_MODE            MXC_GPIO_INT_FALLING                                                                     // TODO

#define MAX32666_PMIC_PFN2_PIN             {MXC_GPIO1, MXC_GPIO_PIN_9, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

// MAX32666 I2C
#define MAX32666_I2C                       MXC_I2C0_BUS0

// MAX32666 LCD
#define MAX32666_LCD_SPI                   MXC_SPI1
#define MAX32666_LCD_DMA_REQSEL_SPITX      MXC_DMA_REQUEST_SPI1TX
#define MAX32666_LCD_SPI_SPEED             16000000

#define MAX32666_LCD_DMA_CHANNEL           0
#define MAX32666_LCD_DMA_IRQ               DMA0_IRQn
#define MAX32666_LCD_DMA_IRQ_HAND          DMA0_IRQHandler
#define MAX32666_LCD_MAP                   MAP_A

// MAX32666 QSPI
#define MAX32666_QSPI                      MXC_SPI0
#define MAX32666_QSPI_DMA_REQSEL_SPITX     MXC_DMA_REQUEST_SPI0TX
#define MAX32666_QSPI_DMA_REQSEL_SPIRX     MXC_DMA_REQUEST_SPI0RX

#define MAX32666_QSPI_DMA_CHANNEL          1
#define MAX32666_QSPI_DMA_IRQ              DMA1_IRQn
#define MAX32666_QSPI_DMA_IRQ_HAND         DMA1_IRQHandler
#define MAX32666_QSPI_MAP                  MAP_B

// MAX32666 SD CARD
#define MAX32666_SD_BUS_VOLTAGE            MXC_SDHC_Bus_Voltage_3_3
#define MAX32666_SD_CLK_DIV                0x0b0  // Maximum divide ratio
#define MAX32666_SD_INIT_RETRY             10

// MAX32666 Hardware semaphores
#define MAX32666_SEMAPHORE_PRINT           1
#define MAX32666_COMMBUF_BUFFER            2

// MAX32666 Timers
#define MAX32666_BLE_TIMER                 MXC_TMR0
#define MAX32666_BLE_SLEEP_TIMER           MXC_TMR1
#define MAX32666_LED_TIMER                 MXC_TMR2

// Communication buffer
#define MAX32666_COMMBUF_ARRAY_SIZE        10

/*** MAX78000 AUDIO ***/
// MAX78000 AUDIO PINS
#define MAX78000_AUDIO_CNN_BOOST_PIN       {MXC_GPIO2, MXC_GPIO_PIN_5, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_AUDIO_AUDIO_OSC_PIN       {MXC_GPIO0, MXC_GPIO_PIN_31, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_AUDIO_HOST_INT_PIN        {MXC_GPIO0, MXC_GPIO_PIN_12, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_AUDIO_LED_RED_PIN         {MXC_GPIO2, MXC_GPIO_PIN_0, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}
#define MAX78000_AUDIO_LED_GREEN_PIN       {MXC_GPIO2, MXC_GPIO_PIN_1, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}
#define MAX78000_AUDIO_LED_BLUE_PIN        {MXC_GPIO2, MXC_GPIO_PIN_2, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_AUDIO_HOST_IO_PIN         {MXC_GPIO1, MXC_GPIO_PIN_6, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

#define MAX78000_AUDIO_EXPANDER_IN_PIN     {MXC_GPIO1, MXC_GPIO_PIN_7, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO} // TODO

#define MAX78000_AUDIO_PMIC_MPC0_PIN       {MXC_GPIO3, MXC_GPIO_PIN_0, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

#define MAX78000_AUDIO_BUTTON_INT_PIN      {MXC_GPIO0, MXC_GPIO_PIN_2, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

#define MAX78000_AUDIO_DEBUG_SEL_PIN       {MXC_GPIO2, MXC_GPIO_PIN_2, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

// MAX78000 AUDIO QSPI
#define MAX78000_AUDIO_QSPI                MXC_SPI0
#define MAX78000_AUDIO_QSPI_DMA_CHANNEL    1
#define MAX78000_AUDIO_QSPI_DMA_IRQ        DMA1_IRQn
#define MAX78000_AUDIO_QSPI_DMA_IRQ_HAND   DMA1_IRQHandler

/*** MAX78000 VIDEO ***/
// MAX78000 VIDEO PINS
#define MAX78000_VIDEO_FLASH_PIN           {MXC_GPIO0, MXC_GPIO_PIN_19, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_PULL_DOWN, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_VIDEO_CAMERA_PIN          {MXC_GPIO0, MXC_GPIO_PIN_3, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_VIDEO_HOST_INT_PIN        {MXC_GPIO0, MXC_GPIO_PIN_12, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_VIDEO_LED_RED_PIN         {MXC_GPIO2, MXC_GPIO_PIN_0, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}
#define MAX78000_VIDEO_LED_GREEN_PIN       {MXC_GPIO2, MXC_GPIO_PIN_1, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}
#define MAX78000_VIDEO_LED_BLUE_PIN        {MXC_GPIO2, MXC_GPIO_PIN_2, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}

#define MAX78000_VIDEO_HOST_IO_PIN         {MXC_GPIO1, MXC_GPIO_PIN_6, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

#define MAX78000_VIDEO_EXPANDER_IN_PIN     {MXC_GPIO1, MXC_GPIO_PIN_7, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO} // TODO

#define MAX78000_VIDEO_PMIC_MPC2_PIN       {MXC_GPIO3, MXC_GPIO_PIN_0, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO
#define MAX78000_VIDEO_PMIC_MPC1_PIN       {MXC_GPIO3, MXC_GPIO_PIN_1, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

#define MAX78000_VIDEO_BUTTON_INT_PIN      {MXC_GPIO0, MXC_GPIO_PIN_2, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

#define MAX78000_VIDEO_DEBUG_SEL_PIN       {MXC_GPIO2, MXC_GPIO_PIN_2, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO}  // TODO

// MAX78000 VIDEO QSPI
#define MAX78000_VIDEO_QSPI                MXC_SPI0
#define MAX78000_VIDEO_QSPI_DMA_CHANNEL    1
#define MAX78000_VIDEO_QSPI_DMA_IRQ        DMA1_IRQn
#define MAX78000_VIDEO_QSPI_DMA_IRQ_HAND   DMA1_IRQHandler

// MAX78000 VIDEO CAMERA
#define MAX78000_VIDEO_CAMERA_DMA_CHANNEL  0
#define MAX78000_VIDEO_CAMERA_DMA_IRQ      DMA0_IRQn
#define MAX78000_VIDEO_CAMERA_DMA_IRQ_HAND DMA0_IRQHandler

// UTILITY
#define GPIO_SET(x)         MXC_GPIO_OutSet(x.port, x.mask)
#define GPIO_CLR(x)         MXC_GPIO_OutClr(x.port, x.mask)

#define GET_RTC_MS()  ((MXC_RTC_GetSecond() * 1000) + (( MXC_RTC_GetSubSecond() / 4096.0)*1000))


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef enum {
//  QSPI Packet                                 QSPI Packet Payload Description
    QSPI_PACKET_TYPE_VIDEO_VERSION_CMD = 0,  // None
    QSPI_PACKET_TYPE_VIDEO_VERSION_RES,      // version_t

    QSPI_PACKET_TYPE_AUDIO_VERSION_CMD,      // None
    QSPI_PACKET_TYPE_AUDIO_VERSION_RES,      // version_t

    QSPI_PACKET_TYPE_VIDEO_SERIAL_CMD,       // None
    QSPI_PACKET_TYPE_VIDEO_SERIAL_RES,       // serial_num_t

    QSPI_PACKET_TYPE_AUDIO_SERIAL_CMD,       // None
    QSPI_PACKET_TYPE_AUDIO_SERIAL_RES,       // serial_num_t

    QSPI_PACKET_TYPE_VIDEO_DATA_RES,                // 240x240 RGB565 Image
    QSPI_PACKET_TYPE_VIDEO_CLASSIFICATION_RES,      // classification_result_t
    QSPI_PACKET_TYPE_VIDEO_STATISTICS_RES,          // max78000_statistics_t

    QSPI_PACKET_TYPE_AUDIO_DATA_RES,                // TODO
    QSPI_PACKET_TYPE_AUDIO_CLASSIFICATION_RES,      // classification_result_t
    QSPI_PACKET_TYPE_AUDIO_STATISTICS_RES,          // max78000_statistics_t

    QSPI_PACKET_TYPE_VIDEO_FACEID_EMBED_UPDATE_CMD, // None
    QSPI_PACKET_TYPE_VIDEO_FACEID_EMBED_UPDATE_RES, // faceid_embed_update_status_e

    QSPI_PACKET_TYPE_VIDEO_ENABLE_CMD,       // None
    QSPI_PACKET_TYPE_VIDEO_DISABLE_CMD,      // None
    QSPI_PACKET_TYPE_AUDIO_ENABLE_CMD,       // None
    QSPI_PACKET_TYPE_AUDIO_DISABLE_CMD,      // None

    QSPI_PACKET_TYPE_VIDEO_ENABLE_CNN_CMD,   // None
    QSPI_PACKET_TYPE_VIDEO_DISABLE_CNN_CMD,  // None
    QSPI_PACKET_TYPE_AUDIO_ENABLE_CNN_CMD,   // None
    QSPI_PACKET_TYPE_AUDIO_DISABLE_CNN_CMD,  // None

    QSPI_PACKET_TYPE_VIDEO_ENABLE_FLASH_LED_CMD,  // None
    QSPI_PACKET_TYPE_VIDEO_DISABLE_FLASH_LED_CMD, // None

    QSPI_PACKET_TYPE_LAST
} qspi_packet_type_e;

typedef enum {
    PACKET_TYPE_COMMAND = 0,
    PACKET_TYPE_PAYLOAD
} packet_type_e;

typedef enum {
//  Command                             Command Payload Description
    // Communication
    COMMAND_ABORT_CMD = 0,           // None
    COMMAND_INVALID_RES,             // Invalid command code
    COMMAND_NOP_CMD,                 // None

    // Version
    COMMAND_GET_VERSION_CMD,         // None
    COMMAND_GET_VERSION_RES,         // device_version_t
    COMMAND_GET_SERIAL_NUM_CMD,      // None
    COMMAND_GET_SERIAL_NUM_RES,      // device_serial_num_t

    // SD Card
    COMMAND_GET_SD_INSERTED_CMD,     // None
    COMMAND_GET_SD_INSERTED_RES,     // sd_status_e
    COMMAND_WRITE_SD_FILE_CMD,       // File name size (uint8_t) + File name string + File content
    COMMAND_WRITE_SD_FILE_RES,       // sd_status_e
    COMMAND_READ_SD_FILE_CMD,        // File name string
    COMMAND_READ_SD_FILE_RES,        // sd_status_e + File content
    COMMAND_GET_SD_CONTENT_CMD,      // None
    COMMAND_GET_SD_CONTENT_RES,      // (file_info_header_t + File name string) * Number of files
    COMMAND_GET_SD_FREE_SPACE_CMD,   // None
    COMMAND_GET_SD_FREE_SPACE_RES,   // Free space size (uint32_t)
    COMMAND_DELETE_SD_FILE_CMD,      // File name string
    COMMAND_DELETE_SD_FILE_RES,      // sd_status_e
    COMMAND_FORMAT_SD_CMD,           // None
    COMMAND_FORMAT_SD_RES,           // sd_status_e

    // External Flash
    COMMAND_WRITE_EXT_FILE_CMD,      // File name size (uint8_t) + File name string + File content
    COMMAND_WRITE_EXT_FILE_RES,      // ext_status_e
    COMMAND_READ_EXT_FILE_CMD,       // File name string
    COMMAND_READ_EXT_FILE_RES,       // ext_status_e + File content
    COMMAND_GET_EXT_CONTENT_CMD,     // None
    COMMAND_GET_EXT_CONTENT_RES,     // (file_info_header_t + File name string) * Number of files
    COMMAND_GET_EXT_FREE_SPACE_CMD,  // None
    COMMAND_GET_EXT_FREE_SPACE_RES,  // Free space size (uint32_t)
    COMMAND_DELETE_EXT_FILE_CMD,     // File name string
    COMMAND_DELETE_EXT_FILE_RES,     // ext_status_e
    COMMAND_FORMAT_EXT_CMD,          // None
    COMMAND_FORMAT_EXT_RES,          // ext_status_e

    // Firmware Update from SD Card
    COMMAND_FW_UPDATE_MAX32666_SD_CMD,        // File name string
    COMMAND_FW_UPDATE_MAX32666_SD_RES,        // fw_update_status_e
    COMMAND_FW_UPDATE_MAX78000_SD_VIDEO_CMD,  // File name string
    COMMAND_FW_UPDATE_MAX78000_SD_VIDEO_RES,  // fw_update_status_e
    COMMAND_FW_UPDATE_MAX78000_SD_AUDIO_CMD,  // File name string
    COMMAND_FW_UPDATE_MAX78000_SD_AUDIO_RES,  // fw_update_status_e
    COMMAND_FW_UPDATE_COMBINED_SD_CMD,        // File name string
    COMMAND_FW_UPDATE_COMBINED_SD_RES,        // fw_update_status_e

    // Firmware Update from External Flash
    COMMAND_FW_UPDATE_MAX32666_EXT_CMD,       // File name string
    COMMAND_FW_UPDATE_MAX32666_EXT_RES,       // fw_update_status_e
    COMMAND_FW_UPDATE_MAX78000_EXT_VIDEO_CMD, // File name string
    COMMAND_FW_UPDATE_MAX78000_EXT_VIDEO_RES, // fw_update_status_e
    COMMAND_FW_UPDATE_MAX78000_EXT_AUDIO_CMD, // File name string
    COMMAND_FW_UPDATE_MAX78000_EXT_AUDIO_RES, // fw_update_status_e
    COMMAND_FW_UPDATE_COMBINED_EXT_CMD,       // File name string
    COMMAND_FW_UPDATE_COMBINED_EXT_RES,       // fw_update_status_e

    // FaceID Embeddings Update
    COMMAND_FACEID_EMBED_UPDATE_CMD,          // FaceID embeddings binary content
    COMMAND_FACEID_EMBED_UPDATE_RES,          // faceid_embed_update_status_e
    COMMAND_FACEID_EMBED_UPDATE_SD_CMD,       // File name string
    COMMAND_FACEID_EMBED_UPDATE_SD_RES,       // faceid_embed_update_status_e
    COMMAND_FACEID_EMBED_UPDATE_EXT_CMD,      // File name string
    COMMAND_FACEID_EMBED_UPDATE_EXT_RES,      // faceid_embed_update_status_e

    // Device Settings
    COMMAND_ENABLE_BLE_CMD,                       // None
    COMMAND_DISABLE_BLE_CMD,                      // None
    COMMAND_SHUT_DOWN_DEVICE_CMD,                 // None
    COMMAND_ENABLE_MAX78000_AUDIO_CMD,            // None
    COMMAND_DISABLE_MAX78000_AUDIO_CMD,           // None
    COMMAND_ENABLE_MAX78000_VIDEO_CMD,            // None
    COMMAND_DISABLE_MAX78000_VIDEO_CMD,           // None
    COMMAND_ENABLE_MAX78000_VIDEO_CNN_CMD,        // None
    COMMAND_DISABLE_MAX78000_VIDEO_CNN_CMD,       // None
    COMMAND_ENABLE_MAX78000_VIDEO_FLASH_LED_CMD,  // None
    COMMAND_DISABLE_MAX78000_VIDEO_FLASH_LED_CMD, // None
    COMMAND_ENABLE_MAX78000_VIDEO_AUDIO_POWER,    // None
    COMMAND_DISABLE_MAX78000_VIDEO_AUDIO_POWER,   // None
    COMMAND_ENABLE_LCD_CMD,                       // None
    COMMAND_DISABLE_LCD_CMD,                      // None
    COMMAND_ENABLE_LCD_STATISCTICS_CMD,           // None
    COMMAND_DISABLE_LCD_STATISCTICS_CMD,          // None
    COMMAND_ENABLE_LCD_PROBABILITY_CMD,           // None
    COMMAND_DISABLE_LCD_PROBABILITY_CMD,          // None
    COMMAND_ENABLE_SEND_STATISTICS_CMD,           // None
    COMMAND_DISABLE_SEND_STATISTICS_CMD,          // None
    COMMAND_ENABLE_SEND_CLASSIFICATION_CMD,       // None
    COMMAND_DISABLE_SEND_CLASSIFICATION_CMD,      // None

    // Statistics
    COMMAND_GET_STATISTICS_RES,      // device_statistics_t

    // Classification
    COMMAND_GET_MAX78000_VIDEO_CLASSIFICATION_RES, // classification_result_t
    COMMAND_GET_MAX78000_AUDIO_CLASSIFICATION_RES, // classification_result_t

    // Debugger Selection
    COMMAND_SET_DEBUGGER_CMD,        // debugger_select_e
    COMMAND_GET_DEBUGGER_CMD,        // None
    COMMAND_GET_DEBUGGER_RES,        // debugger_select_e

    COMMAND_LAST
} command_e;

typedef enum {
    SD_STATUS_OK = 0,
    SD_STATUS_DISK_ERR,
    SD_STATUS_INT_ERR,
    SD_STATUS_NOT_READY,
    SD_STATUS_NO_FILE,
    SD_STATUS_NO_PATH,
    SD_STATUS_INVALID_NAME,
    SD_STATUS_DENIED,
    SD_STATUS_EXIST,
    SD_STATUS_INVALID_OBJECT,
    SD_STATUS_NOT_ENABLED,
    SD_STATUS_NO_FILESYSTEM,
    SD_STATUS_MKFS_ABORTED,
    SD_STATUS_TIMEOUT,
    SD_STATUS_LOCKED,
    SD_STATUS_NOT_ENOUGH_CORE,
    SD_STATUS_TOO_MANY_OPEN_FILES,
    SD_STATUS_INVALID_PARAMETER,

    SD_STATUS_LAST
} sd_status_e;

typedef enum {
    EXT_STATUS_OK = 0,
    //TODO

    EXT_STATUS_LAST
} ext_status_e;

typedef enum {
    FW_UPDATE_STATUS_ = 0,
    FW_UPDATE_STATUS_SUCCESS,
    FW_UPDATE_STATUS_NO_FILE,
    FW_UPDATE_STATUS_MAX32666_FAIL,
    FW_UPDATE_STATUS_MAX78000_VIDEO_FAIL,
    FW_UPDATE_STATUS_MAX78000_AUDIO_FAIL,
    FW_UPDATE_STATUS_COMBINED_FAIL,

    FW_UPDATE_STATUS_LAST
} fw_update_status_e;

typedef enum {
    FACEID_EMBED_UPDATE_STATUS_SUCCESS = 0,
    FACEID_EMBED_UPDATE_STATUS_NO_FILE,
    FACEID_EMBED_UPDATE_STATUS_FAIL,

    FACEID_EMBED_UPDATE_STATUS_LAST
} faceid_embed_update_status_e;

typedef enum {
    DEBUGGER_SELECT_MAX32666_CORE1 = 0,
    DEBUGGER_SELECT_MAX78000_VIDEO,
    DEBUGGER_SELECT_MAX78000_AUDIO,

    DEBUGGER_SELECT_LAST
} debugger_select_e;

typedef enum {
    CLASSIFICATION_NOTHING = 0,
    CLASSIFICATION_UNKNOWN,
    CLASSIFICATION_LOW_CONFIDENCE,
    CLASSIFICATION_DETECTED,

    CLASSIFICATION_LAST
} classification_e;

typedef uint8_t serial_num_t[13];

typedef struct __attribute__((packed)) {
    uint32_t start_symbol;
    uint32_t packet_size;
    uint8_t packet_type;
} qspi_packet_header_t;

#define PACKET_SEQ_MASK   0x7F
typedef struct __attribute__((packed)) {
    uint8_t type : 1;
    uint8_t seq  : 7;
} packet_info_t;

typedef struct __attribute__((packed)) {
    packet_info_t packet_info;
    uint8_t command;
    uint32_t command_size;
} command_packet_header_t;

typedef struct __attribute__((packed)) {
    packet_info_t packet_info;
} payload_packet_header_t;

typedef struct __attribute__((packed)) {
    command_packet_header_t header;
    uint8_t payload[COMMUNICATION_MAX_PACKET_SIZE - sizeof(command_packet_header_t)];
} command_packet_t;

typedef struct __attribute__((packed)) {
    payload_packet_header_t header;
    uint8_t payload[COMMUNICATION_MAX_PACKET_SIZE - sizeof(payload_packet_header_t)];
} payload_packet_t;

typedef struct __attribute__((packed)) {
    uint8_t size;
    uint8_t status;
    union {
        packet_info_t packet_info;
        command_packet_t command_packet;
        payload_packet_t payload_packet;
    } packet;
} packet_container_t;

typedef struct __attribute__((packed)) {
    uint8_t major;
    uint8_t minor;
    uint32_t build;
} version_t;

typedef struct __attribute__((packed)) {
    version_t max32666;
    version_t max78000_video;
    version_t max78000_audio;
} device_version_t;

typedef struct __attribute__((packed)) {
    serial_num_t max32666;
    serial_num_t max78000_video;
    serial_num_t max78000_audio;
} device_serial_num_t;

typedef struct __attribute__((packed)) {
    uint32_t cnn_duration_ms;
    uint32_t capture_duration_ms;
    uint32_t communication_duration_ms;
    uint32_t total_duration_ms;
} max78000_statistics_t;

typedef struct __attribute__((packed)) {
    max78000_statistics_t max78000_video;
    max78000_statistics_t max78000_audio;
    float lcd_fps;
    float battery_level;
    uint32_t max78000_video_power_mw;
    uint32_t max78000_audio_power_mw;
} device_statistics_t;

typedef struct __attribute__((packed)) {
    float probabily;
    classification_e classification;
    char result[15];
} classification_result_t;

typedef struct __attribute__((packed)) {
    uint8_t file_name_size;
    uint32_t file_size;
    // char [] file_name
} file_info_header_t;

#endif /* _MAXREFDES178_DEFINTIIONS_H_ */
