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
#include <mxc_delay.h>
#include <mxc_errors.h>
#include <stdio.h>

#include "max32666_debug.h"
#include "max32666_data.h"
#include "max32666_i2c.h"
#include "max32666_pmic.h"
#include "max32666_timer_led_button.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "pmic"

//// MAX20303 PMIC Definitions
// MAX20303 Device ID register value
#define MAX20303_HARDWARE_ID             0x02

// MAX20303 PMIC Registers
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
#define MAX20303_APREG_CHARHER_CONTROL_WRITE             0x1A
#define MAX20303_APREG_CHARHER_CONTROL_READ              0x1B

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
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
enum eMax20303RegStatus0ChgStat
{
    MAX20303_STATUS0_CHG_STAT_OFF = 0x0,       /**< Charger off */
    MAX20303_STATUS0_CHG_STAT_SUSPENDED_TEMP,  /**< Charging suspended due to temperature */
    MAX20303_STATUS0_CHG_STAT_PRE_CHARGE,      /**< Pre-charge in progress */
    MAX20303_STATUS0_CHG_STAT_FAST_CHARGE_CC,  /**< Fast-charge constant current mode in progress */
    MAX20303_STATUS0_CHG_STAT_FAST_CHARGE_CV,  /**< Fast-charge constant voltage mode in progress */
    MAX20303_STATUS0_CHG_STAT_MAINTAIN,        /**< Maintain charge in progress */
    MAX20303_STATUS0_CHG_STAT_MAINTAIN_DONE,   /**< Maintain charger timer done */
    MAX20303_STATUS0_CHG_STAT_FAULT,           /**< Charger fault condition */
};

typedef union
{
    uint8_t raw;
    struct
    {
        uint8_t ChgStat    : 4;   /**< Status of Charger Mode. */
        uint8_t ThmStat    : 4;   /**< Status of Thermistor Monitoring. */
    } bits;
} tuMax20303RegStatus0;

typedef union
{
    uint8_t raw;
    struct
    {
        uint8_t ChgTmo     : 1;   /**< Status of Time-Out Condition. */
        uint8_t ChgThmReg  : 1;   /**< Status of Thermal Regulation. */
        uint8_t ChgThmSd   : 1;   /**< Status of Thermal Shutdown. */
        uint8_t UsbOk      : 1;   /**< Status of CHGIN Input. */
        uint8_t UsbOVP     : 1;   /**< Status of CHGIN OVP. */
        uint8_t ILim       : 1;   /**< CHGIN Input Current Limit. */
        uint8_t            : 2;
    } bits;
} tuMax20303RegStatus1;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int pmic_enable_charger(void);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int pmic_init(void)
{
    int err;
	uint8_t regval;

    // Test connectivity by reading hardware ID
    if ((err = i2c_master_reg_read(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_HARDWARE_ID, &regval)) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }
    if (regval != MAX20303_HARDWARE_ID) {
        PR_ERROR("hw_id is not supported 0x%x", regval);
        return E_NOT_SUPPORTED;
    }

    // MAX20303J : Default configuration must modified for normal mode of operation
    // At Power-Up :
    // LOD1				is disabled , configured as LDO; Vout = 1.2V
    // LDO2				is disabled , configured as LDO; Vout = 3.2V
    // Buck1 converter	is enabled	, set to 1.8V
    // Buck2 converter	is disabled , set to 0.9V
    // Boost converter	is disabled , set to 20V

    // After Power-Up :
    // LDO1				configure as load-switch; Vout = 1.8V
    // LDO2				configure as LDO; Vout = 3.3V
    // Buck1 converter	set to 1.8V and enable
    // Buck2 converter	set to 2.8V and enable
    // Boost converter	set to 9.6V and enable when needed

    // LDO1 : in 1V8, out 1V8_AUX - AI 1V8: SRAM, power acc, button, SWD/UART switches, VREGO_A, camera, audio codec, microphone, SWCLK pullup
    // LDO2 : in SYS, out 3V3_AUX - AI 3V3: power acc, RGB LED, flash LED,
    // Buck1: out 1V8             - audio codec, LCD controller, ext flash, accel, button, expander, HDK switch, bootloader UART switch, I2C pullup
    // Buck2: out 2V8_AUX         - touch, LCD controller, AI 2V8: VREGI, VDDIOH, camera
    // Boost: out VBL             - LCD backlight

    // Configure LDO and Buck outputs
    if ((err = pmic_ldo2(1)) != E_NO_ERROR) {
        PR_ERROR("max20303_ldo2 failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(10));

    // i2c bus lock issue fix
    i2c_master_init();

    if ((err = pmic_buck1(1)) != E_NO_ERROR) {
        PR_ERROR("max20303_buck1 failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(10));

    if ((err = pmic_ldo1(1)) != E_NO_ERROR) {
        PR_ERROR("max20303_ldo1 failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(10));

    if ((err = pmic_buck2(0)) != E_NO_ERROR) {
        PR_ERROR("max20303_buck2 failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(100));

    if ((err = pmic_buck2(1)) != E_NO_ERROR) {
        PR_ERROR("max20303_buck2 failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(100));

    if ((err = pmic_boost(0, 0x10)) != E_NO_ERROR) {
        PR_ERROR("max20303_boost failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(10));

    // Turn off all PMIC LEDs at startup
    if ((err = pmic_led_red(0)) != E_NO_ERROR) {
        PR_ERROR("max20303_led_red failed %d", err);
        return err;
    }
    if ((err = pmic_led_blue(0)) != E_NO_ERROR) {
        PR_ERROR("max20303_led_blue failed %d", err);
        return err;
    }
    if ((err = pmic_led_green(0)) != E_NO_ERROR) {
        PR_ERROR("max20303_led_green failed %d", err);
        return err;
    }

    if ((err = pmic_enable_charger()) != E_NO_ERROR) {
        PR_ERROR("max20303_enable_charger failed %d", err);
        return err;
    }

    return E_NO_ERROR;
}

static int pmic_enable_charger(void)
{
    int err;
    uint8_t appdata[4];

    // write charger config
    // MAX20303J default is 0x0C, 0x75, 0xF6, 0x00
    appdata[0] = 0x0C;  // MtChgTmr=0min, FChgTmr=600min, PChgTmr=30min
    appdata[1] = 0x75;  // VPChg=3.15V, IPChg=0.1xIFChg, ChgDone=0.1xIFChg
    appdata[2] = 0xF3;  // ChgAutoStp=Auto-Stop enabled, ChgAutoRe=Auto-Restart enabled, BatReChg=BatReg-220mV, BatReg=4.20V
    appdata[3] = 0x00;  // SysMinVlt=3.6V
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, appdata, 4)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write_buf failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_CHARHER_CONFIG_WRITE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(10));
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_RESPONSE, appdata, 1)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }
    if (appdata[0] != MAX20303_APREG_CHARHER_CONFIG_WRITE) {
        PR_ERROR("incorrect response 0x%x", appdata[0]);
    }

    // write charger control
    // MAX20303J default is 0x00
    appdata[0] = 0x01;  // ThmEn=Thermal monitor disabled, ChgEn=Charger enabled
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, appdata, 1)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write_buf failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_CHARHER_CONTROL_WRITE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    MXC_Delay(MXC_DELAY_MSEC(10));
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_RESPONSE, appdata, 1)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }
    if (appdata[0] != MAX20303_APREG_CHARHER_CONTROL_WRITE) {
        PR_ERROR("incorrect response 0x%x", appdata[0]);
    }

//    // read charger config
//    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_CHARHER_CONFIG_READ)) != E_NO_ERROR) {
//        PR_ERROR("i2c_master_reg_write failed %d", err);
//        return err;
//    }
//    MXC_Delay(MXC_DELAY_MSEC(5));
//    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATAIN0, appdata, 4)) != E_NO_ERROR) {
//        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
//        return err;
//    }
//    PR_DEBUG("charger config 0x%x 0x%x 0x%x 0x%x", appdata[0], appdata[1], appdata[2], appdata[3]);
//
//    // read charger control
//    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_CHARHER_CONTROL_READ)) != E_NO_ERROR) {
//        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
//        return err;
//    }
//    MXC_Delay(MXC_DELAY_MSEC(5));
//    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATAIN0, appdata, 1)) != E_NO_ERROR) {
//        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
//        return err;
//    }
//    PR_DEBUG("charger control 0x%x", appdata[0]);

    return E_NO_ERROR;
}

int pmic_worker(void)
{
    int err;

//    tuMax20303RegStatus0 lMax20303RegStatus0 = {0};
//    if ((err = i2c_master_reg_read(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_STATUS0, &lMax20303RegStatus0.raw)) != E_NO_ERROR) {
//        PR_ERROR("i2c_reg_read failed %d", err);
//        return err;
//    }

    tuMax20303RegStatus1 lMax20303RegStatus1 = {0};
    if ((err = i2c_master_reg_read(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_STATUS1, &lMax20303RegStatus1.raw)) != E_NO_ERROR) {
        PR_ERROR("i2c_reg_read failed %d", err);
        return err;
    }

//    switch (lMax20303RegStatus0.bits.ChgStat) {
//    case MAX20303_STATUS0_CHG_STAT_OFF:
//        PR_DEBUG("Charger off");
//        break;
//    case MAX20303_STATUS0_CHG_STAT_SUSPENDED_TEMP:
//        PR_DEBUG("Charging suspended due to temperature");
//        break;
//    case MAX20303_STATUS0_CHG_STAT_PRE_CHARGE:
//        PR_DEBUG("Pre-charge in progress");
//        break;
//    case MAX20303_STATUS0_CHG_STAT_FAST_CHARGE_CC:
//        PR_DEBUG("Fast-charge constant current mode in progress");
//        break;
//    case MAX20303_STATUS0_CHG_STAT_FAST_CHARGE_CV:
//        PR_DEBUG("Fast-charge constant voltage mode in progress");
//        break;
//    case MAX20303_STATUS0_CHG_STAT_MAINTAIN:
//        PR_DEBUG("Maintain charge in progress");
//        break;
//    case MAX20303_STATUS0_CHG_STAT_MAINTAIN_DONE:
//        PR_DEBUG("Maintain charger timer done");
//        break;
//    case MAX20303_STATUS0_CHG_STAT_FAULT:
//        PR_DEBUG("Charger fault condition");
//        break;
//    }

    if (lMax20303RegStatus1.bits.UsbOk) {
        if (!device_status.usb_chgin) {
            pmic_enable_charger();
            timestamps.activity_detected = timer_ms_tick;
        }
        device_status.usb_chgin = 1;
    } else {
        device_status.usb_chgin = 0;
    }

    return E_NO_ERROR;
}

int pmic_led_red(int on)
{
    int err;

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_LED1_DIRECT, 0x21)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_LED1_DIRECT, 0x01)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }
    return E_NO_ERROR;
}

int pmic_led_green(int on)
{
    int err;

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_LED2_DIRECT, 0x21)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_LED2_DIRECT, 0x01)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }
    return E_NO_ERROR;
}

int pmic_led_blue(int on)
{
    int err;

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_LED0_DIRECT, 0x21)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_LED0_DIRECT, 0x01)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }
    return E_NO_ERROR;
}

int pmic_ldo1(int on)
{
    int err;

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x1D)) != E_NO_ERROR) {  // LDO1Md:1 Load-switch mode.  LDO1En:01   Enabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x1C)) != E_NO_ERROR) {  // LDO1Md:1 Load-switch mode.  LDO1En:00   Disabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT1, 0x34)) != E_NO_ERROR) {  // LDO1Vset : 0x34 = 52.  ( 52 * 25mv) + 0.5V = 1.8V
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_LDO1_CONFIG_WRITE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}

int pmic_ldo2(int on)
{
    int err;
    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x19)) != E_NO_ERROR) {  // LDO2Md:0 LDO mode.  LDO2En:01   Enabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x18)) != E_NO_ERROR) {  // LDO2Md:0 LDO mode.  LDO2En:00   Disabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT1, 0x18)) != E_NO_ERROR) {  // LDO2Vset : 0x18 = 24.  ( 24 * 100mv) + 0.9V = 3.3V
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_LDO2_CONFIG_WRITE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}

int pmic_buck1(int on)
{
    int err;

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x7E)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT1, 0x28)) != E_NO_ERROR) {  // Buck1Vset = 0x28=40    ( (40x25mV) + 0.8V = 1.8V)
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT2, 0x1F)) != E_NO_ERROR) {  // Buck1Iset = 0b1111>375mA,  Buck1IZCSet=0b01>20mA   Buck1 ISet and Zero Crossing Threshold
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT3, 0x01)) != E_NO_ERROR) {  // Buck1En = 1   Buck1 enabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT3, 0x00)) != E_NO_ERROR) {  // Buck1En = 0   Buck1 disabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_BUCK1_CONFIG_WRITE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}

int pmic_buck2(int on)
{
    int err;

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x7E)) != E_NO_ERROR) {  // Enable active discharge and FET scaling
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT1, 0x28)) != E_NO_ERROR) {  // Buck2Vset = 0x28=40    ( (40x50mV) + 0.8V = 2.8V)
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT2, 0x2F)) != E_NO_ERROR) {  // Buck2Iset = 0b1111>375mA,  Buck2IZCSet=0b10>30mA   Buck2 ISet and Zero Crossing Threshold
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT3, 0x01)) != E_NO_ERROR) {  // Buck2En = 1   Buck2 enabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT3, 0x00)) != E_NO_ERROR) {  // Buck2En = 0   Buck2 disabled
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }

    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_BUCK2_CONFIG_WRITE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}

int pmic_buckboost(int on)
{
    int err;

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x00)) != E_NO_ERROR) {  //  Reserved. Set to 0x00
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT1, 0x07)) != E_NO_ERROR) {  //  BBstIset : 350mAa Max
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT2, 0x19)) != E_NO_ERROR) {  //  BBstVset = 0x19=25  ( ( 25 * 100mV) + 2.5V = 5.00V)
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT3, 0x79)) != E_NO_ERROR) {  //  BuckBoost Enabled - BSTEn = 0x01
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT3, 0x78)) != E_NO_ERROR) {  //  BuckBoost Disabled - BSTEn = 0x00
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_BBST_CONFIG_WRITE)) != E_NO_ERROR) {  // = Bset_Config_Write
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}

int pmic_boost(int on, uint8_t boost_output_level)
{
    int err;

    //  Prevent setting boost voltage above 10.5V due to specs limitation of the LCD
    if (boost_output_level > 0x16) {
        return E_BAD_PARAM;
    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT3, boost_output_level)) != E_NO_ERROR) {  //  BstVset = 0x0F=15  ( ( 15 * 250mV) + 5 = 8.75V)
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    if (on) {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x01)) != E_NO_ERROR) {  //  Boost Enabled - BSTEn = 0x01
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    } else {
        if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0x00)) != E_NO_ERROR) {  //  Boost Disabled - BSTEn = 0x00
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT4, 0x03)) != E_NO_ERROR) {  //  BstSet 0x03 - not sure if this is writeable
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_BST_CONFIG_WRITE)) != E_NO_ERROR) {  // = Bset_Config_Write
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}

int pmic_power_off(void)
{
    int err;

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0xB2)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_POWEROFF)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}

int pmic_hard_reset(void)
{
    int err;

    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_DATOUT0, 0xB4)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    if ((err = i2c_master_reg_write(I2C_ADDR_MAX20303_PMIC, MAX20303_REG_AP_CMDOUT, MAX20303_APREG_HARDRESET)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }
    return E_NO_ERROR;
}
