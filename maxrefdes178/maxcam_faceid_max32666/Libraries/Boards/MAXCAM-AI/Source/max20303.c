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
#include "board.h"
#include "tmr_utils.h"
#include "i2c.h"

#include "max20303.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define MAX20303_I2C_ADDR                0x50
#define MAX20303_HARDWARE_ID             0x02

// Registers
#define MAX20303_REG_HARDWARE_ID         0x00     // HardwareID Register
#define MAX20303_REG_FIRMWARE_REV        0x01     // FirmwareID Register
#define MAX20303_REG_INT0                0x03     // Int0 Register
#define MAX20303_REG_INT1                0x04     // Int1 Register
#define MAX20303_REG_INT2                0x05     // Int2 Register
#define MAX20303_REG_STATUS0             0x06     // Status Register 0
#define MAX20303_REG_STATUS1             0x07     // Status Register 1
#define MAX20303_REG_STATUS2             0x08     // Status Register 2
#define MAX20303_REG_STATUS3             0x09     // Status Register 2
#define MAX20303_REG_SYSTEM_ERROR        0x0B     // SystemError Register
#define MAX20303_REG_INT_MASK0           0x0C     // IntMask0 Register
#define MAX20303_REG_INT_MASK1           0x0D     // IntMask1 Register
#define MAX20303_REG_INT_MASK2           0x0E     // IntMask1 Register
#define MAX20303_REG_AP_DATOUT0          0x0F     // APDataOut0 Register
#define MAX20303_REG_AP_DATOUT1          0x10     // APDataOut1 Register
#define MAX20303_REG_AP_DATOUT2          0x11     // APDataOut2 Register
#define MAX20303_REG_AP_DATOUT3          0x12     // APDataOut3 Register
#define MAX20303_REG_AP_DATOUT4          0x13     // APDataOut4 Register
#define MAX20303_REG_AP_DATOUT5          0x14     // APDataOut5 Register
#define MAX20303_REG_AP_DATOUT6          0x15     // APDataOut6 Register
#define MAX20303_REG_AP_CMDOUT           0x17     // APCmdOut Register
#define MAX20303_REG_AP_RESPONSE         0x18     // APResponse Register
#define MAX20303_REG_AP_DATAIN0          0x19
#define MAX20303_REG_AP_DATAIN1          0x1A
#define MAX20303_REG_AP_DATAIN2          0x1B
#define MAX20303_REG_AP_DATAIN3          0x1C
#define MAX20303_REG_AP_DATAIN4          0x1D
#define MAX20303_REG_AP_DATAIN5          0x1E
#define MAX20303_REG_LDO_DIRECT          0x20
#define MAX20303_REG_MPC_DIRECTWRITE     0x21
#define MAX20303_REG_MPC_DIRECTRED       0x22
#define MAX20303_REG_LED_STEP_DIRECT     0x2C
#define MAX20303_REG_LED0_DIRECT         0x2D
#define MAX20303_REG_LED1_DIRECT         0x2E
#define MAX20303_REG_LED2_DIRECT         0x2F
#define MAX20303_REG_LDO1_CONFIG_WRITE   0x40
#define MAX20303_REG_LDO1_CONFIG_READ    0x41
#define MAX20303_REG_LDO2_CONFIG_WRITE   0x42
#define MAX20303_REG_LDO2_CONFIG_READ    0x43

#define MAX20303_APREG_GPIO_CONFIG_WRITE                 0x01
#define MAX20303_APREG_GPIO_CONFIG_READ                  0x02
#define MAX20303_APREG_GPIO_CONTROL_WRITE                0x03
#define MAX20303_APREG_GPIO_CONTROL_READ                 0x04

#define MAX20303_APREG_MPC_CONFIG_WRITE                  0x06
#define MAX20303_APREG_MPC_CONFIG_READ                   0x07
#define MAX20303_APREG_INPUTCURRENT_CONFIG_WRITE         0x10
#define MAX20303_APREG_INPUTCURRENT_CONFIG_READ          0x11
#define MAX20303_APREG_THERMALSHUTDOWN_CONFIG_READ       0x12
#define MAX20303_APREG_CHARHER_CONFIG_WRITE              0x14
#define MAX20303_APREG_CHARHER_CONFIG_READ               0x15
#define MAX20303_APREG_CHARGERTHERMALLIMITS_CONFIG_WRITE 0x16
#define MAX20303_APREG_CHARGERTHERMALLIMITS_CONFIG_READ  0x17

#define MAX20303_APREG_BST_CONFIG_WRITE                  0x30
#define MAX20303_APREG_BST_CONFIG_READ                   0x31
#define MAX20303_APREG_BUCK1_CONFIG_WRITE                0x35
#define MAX20303_APREG_BUCK1_CONFIG_READ                 0x36
#define MAX20303_APREG_BUCK1_DVSCONFIG_WRITE             0x37
#define MAX20303_APREG_BUCK2_CONFIG_WRITE                0x3A
#define MAX20303_APREG_BUCK2_CONFIG_READ                 0x3B
#define MAX20303_APREG_BUCK2_DVSCONFIG_WRITE             0x3C
#define MAX20303_APREG_LDO1_CONFIG_WRITE                 0x40
#define MAX20303_APREG_LDO1_CONFIG_READ                  0x41
#define MAX20303_APREG_LDO2_CONFIG_WRITE                 0x42
#define MAX20303_APREG_LDO2_CONFIG_READ                  0x43
#define MAX20303_APREG_CHARGEPUMP_CONFIG_WRITE           0x46
#define MAX20303_APREG_CHARGEPUMP_CONFIG_READ            0x47
#define MAX20303_APREG_SFOUT_CONFIG_WRITE                0x48
#define MAX20303_APREG_SFOUT_CONFIG_READ                 0x49

#define MAX20303_APREG_BBST_CONFIG_WRITE                 0x70
#define MAX20303_APREG_BBST_CONFIG_READ                  0x71

#define MAX20303_APREG_POWEROFF                          0x80
#define MAX20303_APREG_SOFTRESET                         0x81
#define MAX20303_APREG_HARDRESET                         0x82
#define MAX20303_APREG_STAYON                            0x83
#define MAX20303_APREG_POWEROFF_COMMAND_DELAY            0x84


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int MAX20303_writeReg(const uint8_t reg, const uint8_t value);
//static int MAX20303_readReg(const uint8_t reg, uint8_t * buf);

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
/**
 * @brief   Writes a single PMIC register. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      reg is the address of MAX20303 register
 * @param      value is the register value to be written
 * @returns    This function has no return value.
 */
static int MAX20303_writeReg(const uint8_t reg, const uint8_t value)
{
    const uint8_t cmdData[2] = {reg, value};
    return I2C_MasterWrite(PMIC_I2C, MAX20303_I2C_ADDR, cmdData, 2, 0);
}

/*TODO: implement read register*/
/**
 * @brief   Read a single PMIC register. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      reg is the address of MAX20303 register
 * @param      value is the register value to be read
 * @returns    This function has no return value.
 *//*
static int MAX20303_readReg(const uint8_t reg, uint8_t * buf)
{
    const uint8_t cmdData[1] = {reg};

}*/

int MAX20303_initialize(uint8_t initialize_i2c) {

/*    int err;
	uint8_t hardwareID;*/

	if (initialize_i2c) {
        I2C_Shutdown(PMIC_I2C);
        I2C_Init(PMIC_I2C, I2C_FAST_MODE, NULL);
        NVIC_EnableIRQ(PMIC_I2C_IRQ);
    }

    // Turn off all PMIC LEDs at startup
    MAX20303_led_red(MAX20303_LED_OUTPUT_OFF);
    MAX20303_led_green(MAX20303_LED_OUTPUT_OFF);
    MAX20303_led_blue(MAX20303_LED_OUTPUT_OFF);

/*
 * TODO : implement I2C read reg
    if ((err = MAX20303_readReg( MAX20303_HARDWARE_ID , &hardwareID )) != E_NO_ERROR ) {

    	printf("\r\nError: %d \r\n", err );
    	return err;
    }

    if ( hardwareID != MAX20303_HARDWARE_ID ) {

    	printf("\r\nHardware ID: %x", hardwareID );
    	return E_NOT_SUPPORTED;
    }
*/
    // MAX20303J : Default configuration must modified for normal mode of operation
    // At Power-Up :
    // LOD1				is disabled , configured as LDO; Vout = 1.2V
    // LDO2				is disabled , configured as LDO; Vout = 3.2V
    // Buck1 converter	is enabled	, set to 1.8V
    // Buck2 converter	is disabled , set to 0.9V
    // Boost converter	is disabled , set to 20V

    // After Power-Up :
    // LDO1				configure as load-switch
    // LDO2				configure as LDO; Vout = 3.3V
    // Buck1 converter	set to 1.8V and enable
    // Buck2 converter	set to 2.8V and enable
    // Boost converter	set to 9.6V and enable when needed

    // Configure LDO and Buck outputs
    MAX20303_setbuck1(MAX20303_BUCK_OUTPUT_ON);
    TMR_Delay(MXC_TMR0, MSEC(10), 0);

    MAX20303_setbuck2(MAX20303_BUCK_OUTPUT_ON);
    TMR_Delay(MXC_TMR0, MSEC(10), 0);

    MAX20303_setboost( MAX20303_BOOST_OUTPUT_OFF, 0x10 );
    TMR_Delay(MXC_TMR0, MSEC(10), 0);

    MAX20303_setldo1(MAX20303_LDO_OUTPUT_ON);
    TMR_Delay(MXC_TMR0, MSEC(10), 0);
    MAX20303_setldo1(MAX20303_LDO_OUTPUT_ON);
    TMR_Delay(MXC_TMR0, MSEC(10), 0);

    MAX20303_setldo2(MAX20303_LDO_OUTPUT_ON);
    TMR_Delay(MXC_TMR0, MSEC(10), 0);
    MAX20303_setldo2(MAX20303_LDO_OUTPUT_ON);
    TMR_Delay(MXC_TMR0, MSEC(10), 0);
    MAX20303_setldo2(MAX20303_LDO_OUTPUT_ON);
    TMR_Delay(MXC_TMR0, MSEC(10), 0);

    return E_NO_ERROR;
}

void MAX20303_led_red(uint8_t ledStatus) {

    if (ledStatus == MAX20303_LED_OUTPUT_OFF) {
        MAX20303_writeReg( MAX20303_REG_LED1_DIRECT, 0x01 );
    }
    else {
        MAX20303_writeReg( MAX20303_REG_LED1_DIRECT, 0x21 );
    }
}

void MAX20303_led_green( uint8_t ledStatus ) {

    if (ledStatus == MAX20303_LED_OUTPUT_OFF) {
        MAX20303_writeReg( MAX20303_REG_LED2_DIRECT, 0x01 );
    } else {
        MAX20303_writeReg( MAX20303_REG_LED2_DIRECT, 0x21 );
    }
}

void MAX20303_led_blue(uint8_t ledStatus) {

    if (ledStatus == MAX20303_LED_OUTPUT_OFF) {
        MAX20303_writeReg( MAX20303_REG_LED0_DIRECT, 0x01 );
    } else {
        MAX20303_writeReg( MAX20303_REG_LED0_DIRECT, 0x21 );
    }
}

void MAX20303_setldo1(uint8_t ldo1_onoff)
{
    if (ldo1_onoff == MAX20303_LDO_OUTPUT_OFF) {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x1C);  // LDO1Md:1 Load-switch mode.  LDO1En:00   Disabled
    } else {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x1D);  // LDO1Md:1 Load-switch mode.  LDO1En:01   Enabled
    }

    MAX20303_writeReg(MAX20303_REG_AP_DATOUT1, 0x34);      // LDO1Vset : 0x34 = 52.  ( 52 * 25mv) + 0.5V = 1.8V
    MAX20303_writeReg(MAX20303_REG_AP_CMDOUT, MAX20303_APREG_LDO1_CONFIG_WRITE);
}

void MAX20303_setldo2(uint8_t ldo1_onoff)
{
    if (ldo1_onoff == MAX20303_LDO_OUTPUT_OFF) {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x18);  // LDO2Md:0 LDO mode.  LDO2En:00   Disabled
    } else {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x19);  // LDO2Md:0 LDO mode.  LDO2En:01   Enabled
    }

    MAX20303_writeReg(MAX20303_REG_AP_DATOUT1, 0x18);      // LDO2Vset : 0x18 = 24.  ( 24 * 100mv) + 0.9V = 3.3V
    MAX20303_writeReg(MAX20303_REG_AP_CMDOUT, MAX20303_APREG_LDO2_CONFIG_WRITE);
}

void MAX20303_setbuck1(uint8_t buck1_onoff)
{
    MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x7E);
    MAX20303_writeReg(MAX20303_REG_AP_DATOUT1, 0x28);	// Buck1Vset = 0x28=40    ( (40x25mV) + 0.8V = 1.8V)
    MAX20303_writeReg(MAX20303_REG_AP_DATOUT2, 0x1F);	// Buck1Iset = 0b1111>375mA,  Buck1IZCSet=0b01>20mA   Buck1 ISet and Zero Crossing Threshold

    if (buck1_onoff == MAX20303_LDO_OUTPUT_OFF) {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT3, 0x00);  // Buck1En = 0   Buck1 disabled
    } else {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT3, 0x01);  // Buck1En = 1   Buck1 enabled
    }

    MAX20303_writeReg(MAX20303_REG_AP_CMDOUT, MAX20303_APREG_BUCK1_CONFIG_WRITE);
}

void MAX20303_setbuck2(uint8_t buck2_onoff)
{
    MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x7E);	// Enable active discharge and FET scaling
    MAX20303_writeReg(MAX20303_REG_AP_DATOUT1, 0x28);	// Buck2Vset = 0x28=40    ( (40x50mV) + 0.8V = 2.8V)
    MAX20303_writeReg(MAX20303_REG_AP_DATOUT2, 0x2F);	// Buck2Iset = 0b1111>375mA,  Buck2IZCSet=0b10>30mA   Buck2 ISet and Zero Crossing Threshold

    if (buck2_onoff == MAX20303_LDO_OUTPUT_OFF) {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT3, 0x00);  //Buck2En = 0   Buck2 disabled
    } else {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT3, 0x01);  //Buck2En = 1   Buck2 enabled
    }

    MAX20303_writeReg(MAX20303_REG_AP_CMDOUT, MAX20303_APREG_BUCK2_CONFIG_WRITE);
}

void MAX20303_setboost(uint8_t boost_onoff, uint8_t boost_output_level)
{
    if (boost_output_level > 0x16) {
        boost_output_level = 0x16;							   //  Prevent setting boost voltage above 10.5V due to specs limitation of the LCD
    }

    MAX20303_writeReg(MAX20303_REG_AP_DATOUT3, boost_output_level);	   //  BstVset = 0x0F=15  ( ( 15 * 250mV) + 5 = 8.75V)

    if (boost_onoff == MAX20303_BOOST_OUTPUT_OFF) {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x00);			   //  Boost Disabled - BSTEn = 0x00
    } else {
        MAX20303_writeReg(MAX20303_REG_AP_DATOUT0, 0x01);			   //  Boost Enabled - BSTEn = 0x01
    }

    MAX20303_writeReg(MAX20303_REG_AP_DATOUT4, 0x03);				   //  BstSet 0x03 - not sure if this is writeable
    MAX20303_writeReg(MAX20303_REG_AP_CMDOUT, MAX20303_APREG_BST_CONFIG_WRITE);  // = Bset_Config_Write
}
