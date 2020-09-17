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
#include "tmr_utils.h"
#include "i2c.h"

#include "pmic.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define I2C_MASTER              MXC_I2C0_BUS0
#define MAX20303_SLAVE_ADDR     (0x50 >> 1)
#define MAX20303_SLAVE_WR_ADDR  ((MAX20303_SLAVE_ADDR << 1))
#define MAX20303_SLAVE_RD_ADDR  ((MAX20303_SLAVE_ADDR << 1) | 1)


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int MAX20303_writeReg(unsigned char reg, unsigned char value);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
static int MAX20303_writeReg(const uint8_t reg, const uint8_t value)
{
    const uint8_t cmdData[2] = {reg, value};
    return I2C_MasterWrite(I2C_MASTER, MAX20303_SLAVE_WR_ADDR, cmdData, 2, 0);
}

void I2C0_IRQHandler(void)
{
    I2C_Handler(I2C_MASTER);
}

void pmic_ldo1()
{
    printf("LDO1 Setup\n");
    MAX20303_writeReg(0x0F, 0x05);  //REG_AP_DATOUT0  // LDO1Md:1 Load-switch mode.  LDO1En:01   Enabled
    MAX20303_writeReg(0x10, 0x34);  //REG_AP_DATOUT1  // LDO1Vset : 0x34 = 52.  ( 52 * 25mv) + 0.5V = 1.8V
    MAX20303_writeReg(0x17, 0x40);  //REG_AP_CMDOUT = LDO1_Config_Write

    TMR_Delay(MXC_TMR0, MSEC(500), 0);
}

void pmic_buckboost()
{
    printf("Buck-boost Setup\n");
    MAX20303_writeReg(0x0F, 0x00);  //REG_AP_DATOUT0  // Reserved. Set to 0x00
    MAX20303_writeReg(0x10, 0x07);  //REG_AP_DATOUT1  // BBStISet: 111 : 350mA
    MAX20303_writeReg(0x11, 0x19);  //REG_AP_DATOUT1  // BBStVSet: 11001 : 5V
    MAX20303_writeReg(0x12, 0x79);  //REG_AP_DATOUT1  // Inductor: 4.7uH; BBSt Enabled
    MAX20303_writeReg(0x17, 0x70);  //REG_AP_CMDOUT = BBst_Config_Write

    TMR_Delay(MXC_TMR0, MSEC(500), 0);
}

void pmic_turnoff_buck1()
{
    printf("Turn Off Buck1\n");
    MAX20303_writeReg(0x0F, 0x7E);  //REG_AP_DATOUT0  //
    MAX20303_writeReg(0x10, 0x00);  //REG_AP_DATOUT1  // Buck1Vset = 0x00=00    ( (00x25mV) + 0.8V = 0.8V)
    MAX20303_writeReg(0x11, 0x1F);  //REG_AP_DATOUT2  // Buck1Iset = 0b1111>375mA,  Buck1IZCSet=0b01>20mA   Buck1 ISet and Zero Crossing Threshold
    MAX20303_writeReg(0x12, 0x00);  //REG_AP_DATOUT3  // Buck1En = 0   Buck2 disabled
    MAX20303_writeReg(0x17, 0x35);  //REG_AP_CMDOUT = Buck1_Config_Write

    TMR_Delay(MXC_TMR0, MSEC(500), 0);
}

void pmic_initialize_buck1()
{
    printf("Buck1VSet : 1.8V \n");
    MAX20303_writeReg(0x0F, 0x7E);  //REG_AP_DATOUT0  //
    MAX20303_writeReg(0x10, 0x28);  //REG_AP_DATOUT1  // Buck1Vset = 0x28=40    ( (40x25mV) + 0.8V = 1.8V)
    MAX20303_writeReg(0x11, 0x1F);  //REG_AP_DATOUT2  // Buck1Iset = 0b1111>375mA,  Buck1IZCSet=0b01>20mA   Buck1 ISet and Zero Crossing Threshold
    MAX20303_writeReg(0x12, 0x01);  //REG_AP_DATOUT3  // Buck1En = 1   Buck2 enabled
    MAX20303_writeReg(0x17, 0x35);  //REG_AP_CMDOUT = Buck2_Config_Write
}

void pmic_turnoff_buck2()
{
    printf("Turn Off Buck2\n");
    MAX20303_writeReg(0x0F, 0x7E);  //REG_AP_DATOUT0  //
    MAX20303_writeReg(0x10, 0x00);  //REG_AP_DATOUT1  // Buck2Vset = 0x00=00    ( (00x50mV) + 0.8V = 0.8V)
    MAX20303_writeReg(0x11, 0x2F);  //REG_AP_DATOUT2  // Buck2Iset = 0b1111>375mA,  Buck2IZCSet=0b10>30mA   Buck2 ISet and Zero Crossing Threshold
    MAX20303_writeReg(0x12, 0x00);  //REG_AP_DATOUT3  // Buck2En = 0   Buck2 disabled
    MAX20303_writeReg(0x17, 0x3A);  //REG_AP_CMDOUT = Buck2_Config_Write
}

void pmic_initialize_buck2()
{
    printf("Buck2VSet : 2.8V \n");
    MAX20303_writeReg(0x0F, 0x7E);  //REG_AP_DATOUT0  //
    MAX20303_writeReg(0x10, 0x28);  //REG_AP_DATOUT1  // Buck2Vset = 0x28=40    ( (40x50mV) + 0.8V = 2.8V)
    MAX20303_writeReg(0x11, 0x2F);  //REG_AP_DATOUT2  // Buck2Iset = 0b1111>375mA,  Buck2IZCSet=0b10>30mA   Buck2 ISet and Zero Crossing Threshold
    MAX20303_writeReg(0x12, 0x01);  //REG_AP_DATOUT3  // Buck2Ven = 1   Buck2 enabled
    MAX20303_writeReg(0x17, 0x3A);  //REG_AP_CMDOUT = Buck2_Config_Write
}

void pmic_led_red(uint8_t ledStatus)
{
    if (ledStatus == 0) {
        MAX20303_writeReg(0x2D, 0x01);
    }
    else {
        MAX20303_writeReg(0x2D, 0x21);
    }
}

void pmic_led_green(uint8_t ledStatus)
{
    if (ledStatus == 0) {
        MAX20303_writeReg(0x2E, 0x01);
    }
    else {
        MAX20303_writeReg(0x2E, 0x21);
    }
}

void pmic_led_blue(uint8_t ledStatus)
{
    if (ledStatus == 0) {
        MAX20303_writeReg(0x2F, 0x01);
    }
    else {
        MAX20303_writeReg(0x2F, 0x21);
    }
}

void pmic_lcd_backlight(unsigned char onoff)
{
    MAX20303_writeReg(0x12, 0x0F);  //REG_AP_DATOUT4  // 00 : 5V

    if (onoff == 0) {
        MAX20303_writeReg(0x0F, 0x00);  //REG_AP_DATOUT0  // Boost Disabled - BSTEn = 0x00
    }
    else {
        MAX20303_writeReg(0x0F, 0x01);  //REG_AP_DATOUT0  // Boost Enabled - BSTEn = 0x01
    }

    MAX20303_writeReg(0x13, 0x03);  //REG_AP_DATOUT4  // BstSet 0x03 - not sure if this is writeable
    MAX20303_writeReg(0x17, 0x30);  //REG_AP_CMDOUT = Bset_Config_Write

    TMR_Delay(MXC_TMR0, MSEC(550), 0);  // Keep Reset at low for 15ms
}

void pmic_init()
{
    TMR_Delay(MXC_TMR0, MSEC(550), 0);  // Keep Reset at low for 15ms

    printf("\nI2C Shutdown\n");
    I2C_Shutdown(I2C_MASTER);

    printf("\nI2C Init\n");
    I2C_Init(I2C_MASTER, I2C_FAST_MODE, NULL);
    NVIC_EnableIRQ(I2C0_IRQn);

    printf("Buck1VSet : 1.8V \n");
    MAX20303_writeReg(0x10, 0x28);  //REG_AP_DATOUT1  // Buck1Vset = 0x28=40    ( (40x25mV) + 0.8V = 1.8V)
    MAX20303_writeReg(0x11, 0x1F);  //REG_AP_DATOUT2  // Buck1Iset = 0b1111>375mA,  Buck1IZCSet=0b01>20mA   Buck1 ISet and Zero Crossing Threshold
    MAX20303_writeReg(0x12, 0x01);  //REG_AP_DATOUT3  // Buck1Ven = 1   Buck2 enabled
    MAX20303_writeReg(0x17, 0x35);  //REG_AP_CMDOUT = Buck2_Config_Write

    TMR_Delay(MXC_TMR0, MSEC(500), 0);  // Keep Reset at low for 15ms

    printf("Buck2VSet : 2.8V \n");
    MAX20303_writeReg(0x10, 0x2F);  //REG_AP_DATOUT1  // Buck2Vset = 0x28=40    ( (40x50mV) + 0.8V = 2.8V)
    MAX20303_writeReg(0x11, 0x2F);  //REG_AP_DATOUT2  // Buck2Iset = 0b1111>375mA,  Buck2IZCSet=0b10>30mA   Buck2 ISet and Zero Crossing Threshold
    MAX20303_writeReg(0x12, 0x01);  //REG_AP_DATOUT3  // Buck2Ven = 1   Buck2 enabled
    MAX20303_writeReg(0x17, 0x3A);  //REG_AP_CMDOUT = Buck2_Config_Write

    TMR_Delay(MXC_TMR0, MSEC(1000), 0);  // Keep Reset at low for 15ms

    MAX20303_writeReg(0x2D, 0x21);

    TMR_Delay(MXC_TMR0, MSEC(550), 0);  // Keep Reset at low for 15ms

    MAX20303_writeReg(0x2D, 0x01);

    TMR_Delay(MXC_TMR0, MSEC(150), 0);  // Keep Reset at low for 15ms

    MAX20303_writeReg(0x2E, 0x21);

    TMR_Delay(MXC_TMR0, MSEC(550), 0);  // Keep Reset at low for 15ms

    MAX20303_writeReg(0x2E, 0x01);

    TMR_Delay(MXC_TMR0, MSEC(150), 0);  // Keep Reset at low for 15ms

    MAX20303_writeReg(0x2F, 0x21);

    TMR_Delay(MXC_TMR0, MSEC(550), 0);  // Keep Reset at low for 15ms

    MAX20303_writeReg(0x2F, 0x01);

    TMR_Delay(MXC_TMR0, MSEC(150), 0);  // Keep Reset at low for 15ms
}

void pmic_worker()
{
    pmic_led_green(0);
    TMR_Delay(MXC_TMR0, MSEC(500), 0);

    pmic_led_green(1);
    TMR_Delay(MXC_TMR0, MSEC(500), 0);
}
