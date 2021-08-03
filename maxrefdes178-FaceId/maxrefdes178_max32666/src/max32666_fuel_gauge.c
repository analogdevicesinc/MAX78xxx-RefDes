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
#include <math.h>
#include <mxc_delay.h>
#include <mxc_errors.h>
#include <stdio.h>

#include "max32666_debug.h"
#include "max32666_data.h"
#include "max32666_fuel_gauge.h"
#include "max32666_i2c.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "fuel"

//// MAX17048 Fuel Gauge Definitions
// MAX17048 Fuel gauge registers
#define MAX17048_REG_VCELL                       0x02
#define MAX17048_REG_SOC                         0x04
#define MAX17048_REG_MODE                        0x06
#define MAX17048_REG_VERSION                     0x08
#define MAX17048_REG_HIBRT                       0x0A
#define MAX17048_REG_CONFIG                      0x0C
#define MAX17048_REG_OCV                         0x0E
#define MAX17048_REG_VALRT                       0x14
#define MAX17048_REG_CRATE                       0x16
#define MAX17048_REG_VRESET                      0x18
#define MAX17048_REG_STATUS                      0x1A
#define MAX17048_REG_UNLOCK                      0x3E
#define MAX17048_REG_TABLE_START                 0x40
#define MAX17048_REG_TABLE_END                   0x7F
#define MAX17048_REG_CMD                         0xFE
#define MAX17048_REG_RCOMP_SEG_START             0x80
#define MAX17048_REG_RCOMP_SEG_END               0x8F

#define MAX17048_REG_SIZE                        2
#define MAX17048_MAX_BURST_LEN                   4
#define MAX17048_MODEL_INIT_DELAY                150UL
#define MAX17048_MODEL_UNLOCK_MSB                0x4A
#define MAX17048_MODEL_UNLOCK_LSB                0x57
#define MAX17048_VERSION_MSB                     0x00
#define MAX17048_VERSION_LSB                     0x12
#define MAX17048_RCOMP_SEG_MSB                   0x01  // ini file
#define MAX17048_RCOMP_SEG_LSB                   0x00  // ini file
#define MAX17048_OCV_TEST_MSB                    0xDB  // ini file
#define MAX17048_OCV_TEST_LSB                    0xE0  // ini file
#define MAX17048_SOC_CHECK_A                     108   // ini file
#define MAX17048_SOC_CHECK_B                     110   // ini file
#define MAX17048_RCOMP0                          50    // ini file

#define MAX17048_HIBRT_HIBTHR                    0x80
#define MAX17048_HIBRT_ACTTHR                    0x30


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
// ini file
static const uint8_t modelTable[] = { 0xA5, 0x10, 0xAB, 0x30, 0xAE, 0x10, 0xB0,
        0x70, 0xB2, 0x80, 0xB3, 0xA0, 0xB4, 0x90, 0xB6, 0x20, 0xB8, 0x90, 0xBB,
        0xC0, 0xBE, 0xD0, 0xC2, 0xC0, 0xC5, 0x80, 0xC8, 0x40, 0xCB, 0x70, 0xD1,
        0xE0, 0x03, 0x00, 0x06, 0x70, 0x09, 0xB0, 0x0A, 0xF0, 0x11, 0xF0, 0x11,
        0xF0, 0x15, 0xF0, 0x0E, 0xF0, 0x09, 0xB0, 0x0A, 0x40, 0x0A, 0x20, 0x0A,
        0x00, 0x08, 0x10, 0x0A, 0x00, 0x04, 0x00, 0x04, 0x00 };


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t VCellMsb;
        uint8_t VCellLsb;
    } bits;
} tuMax17048RegVcell;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t SocMsb;
        uint8_t SocLsb;
    } bits;
} tuMax17048RegSoc;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t             :4;
        uint8_t HibStat     :1;
        uint8_t EnSleep     :1;
        uint8_t QuickStart  :1;
        uint8_t             :1;
        uint8_t             :8;
    } bits;
} tuMax17048RegMode;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t VersionMsb;
        uint8_t VersionLsb;
    } bits;
} tuMax17048RegVersion;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t HibThr      :8;
        uint8_t ActThr      :8;
    } bits;
} tuMax17048RegHibrt;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t RCOMP       :8;
        uint8_t ATHD        :5;
        uint8_t ALRT        :1;
        uint8_t ALSC        :1;
        uint8_t SLEEP       :1;
    } bits;
} tuMax17048RegConfig;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t OcvMsb;
        uint8_t OcvLsb;
    } bits;
} tuMax17048RegOcv;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t VRESET      :7;
        uint8_t Dis         :1;
        uint8_t ID          :8;
    } bits;
} tuMax17048RegVreset;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t RI          :1;
        uint8_t VH          :1;
        uint8_t VL          :1;
        uint8_t VR          :1;
        uint8_t HD          :1;
        uint8_t SC          :1;
        uint8_t EnVR        :1;
        uint8_t             :1;
        uint8_t             :8;
    } bits;
} tuMax17048RegStatus;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t UnlockMsb;
        uint8_t UnlockLsb;
    } bits;
} tuMax17048RegUnlock;

typedef union
{
    uint8_t raw[MAX17048_REG_SIZE];
    struct
    {
        uint8_t CmdMsb;
        uint8_t CmdLsb;
    } bits;
} tuMax17048RegCmd;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
static int fuel_gauge_update_model(void);
static int fuel_gauge_soc(uint8_t *soc);
static int fuel_gauge_vcell(float *vcell);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int fuel_gauge_init(void)
{
    int err;
    uint8_t soc;
    float vcell;

    // Try to read fuel gauge version
    tuMax17048RegVersion lMax17048RegVersion = { 0 };
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_VERSION, lMax17048RegVersion.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("Fuel gauge is not responding");
        device_status.fuel_gauge_working = 0;
        return E_NO_ERROR;
    } else {
        if (!((lMax17048RegVersion.bits.VersionMsb == MAX17048_VERSION_MSB) && (lMax17048RegVersion.bits.VersionLsb == MAX17048_VERSION_LSB))) {
            PR_ERROR("Invalid fuel gauge version 0x%x 0x%x", lMax17048RegVersion.bits.VersionMsb, lMax17048RegVersion.bits.VersionLsb);
            return E_NOT_SUPPORTED;
        }
        device_status.fuel_gauge_working = 1;
    }

    tuMax17048RegStatus lMax17048RegStatus = { 0 };
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_STATUS, lMax17048RegStatus.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }

    /* If reset indicator bit is set */
    if (lMax17048RegStatus.bits.RI)
    {
        PR_INFO("FuelGauge was reset");
        /* Clear reset indicator bit */
        lMax17048RegStatus.bits.RI = 0;
        if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_STATUS, lMax17048RegStatus.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    /* Read battery voltage */
    if ((err = fuel_gauge_vcell(&vcell)) != E_NO_ERROR) {
        PR_ERROR("max17048_vcell failed %d", err);
        return err;
    }
    PR_INFO("vcell %f", (double)vcell);


    /* Update battery model */
    if ((err = fuel_gauge_update_model()) != E_NO_ERROR) {
        PR_ERROR("max17048_write_model failed %d", err);
        return err;
    }

    /* Set RCOMP and config */
    tuMax17048RegConfig lMax17048RegConfig = { 0 };
    lMax17048RegConfig.bits.RCOMP = MAX17048_RCOMP0;
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_CONFIG, lMax17048RegConfig.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /* Read battery state of charge */
    if ((err = fuel_gauge_soc(&soc)) != E_NO_ERROR) {
        PR_ERROR("max17048_soc failed %d", err);
        return err;
    }
    PR_INFO("soc %d", soc);

    fuel_gauge_worker();

    return E_NO_ERROR;
}

int fuel_gauge_worker(void)
{
    fuel_gauge_soc(&device_status.statistics.battery_soc);
    fuel_gauge_vcell(&device_status.vcell);

    return E_NO_ERROR;
}

static int fuel_gauge_update_model(void)
{
    int err;

    /* Steps C1-C12 from User Guide */

    /*
     * INFO: Step 1. Unlock model access
     * This enables access to the OCV and table registers
     */
    tuMax17048RegUnlock lMax17048RegUnlock = { 0 };
    lMax17048RegUnlock.bits.UnlockMsb = MAX17048_MODEL_UNLOCK_MSB;
    lMax17048RegUnlock.bits.UnlockLsb = MAX17048_MODEL_UNLOCK_LSB;
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_UNLOCK, lMax17048RegUnlock.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /*
     * INFO: Step 2. Read OCV
     * The OCV Register will be modified during the process of loading the custom
     * model. Read and store this value so that it can be written back to the
     * device after the model has been loaded.
     */
    tuMax17048RegOcv lMax17048RegOcvOrig = { 0 };
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_OCV, lMax17048RegOcvOrig.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }

    /*
     * INFO: Step 2.5. Verify Model Access Unlocked
     * If Model Access was correctly unlocked in Step 1, then the OCV bytes read
     * in Step 2 will not be 0xFF. If the values of both bytes are 0xFF,
     * that indicates that Model Access was not correctly unlocked and the
     * sequence should be repeated from Step 1.
     */
    if((lMax17048RegOcvOrig.bits.OcvMsb == 0xff) && (lMax17048RegOcvOrig.bits.OcvLsb == 0xff))
    {
        PR_ERROR("Model access cannot be unlocked.");
        return E_BAD_STATE;
    }

    /*
     * INFO: Step 5. Write the model
     * Once the model is unlocked, the host software must write the 64 byte model
     * to the device. The model is located between memory 0x40 and 0x7F.
     * The model is available in the INI file provided with your performance
     * report. See the end of this document for an explanation of the INI file.
     * Note that the table registers are write-only and will always read
     * 0xFF. Step 9 will confirm the values were written correctly.
     */
    for (uint8_t addr = MAX17048_REG_TABLE_START; addr < MAX17048_REG_TABLE_END; addr += MAX17048_MAX_BURST_LEN) {
        if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, addr, (uint8_t *) &modelTable[addr - MAX17048_REG_TABLE_START], MAX17048_MAX_BURST_LEN)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    /*
     * INFO: Step 5.1. Write RCOMPSeg (MAX17048/MAX17049 only)
     * MAX17048 and MAX17049 have expanded RCOMP range to battery with high
     * resistance due to low temperature or chemistry design. RCOMPSeg can be
     * used to expand the range of RCOMP. For these devices, RCOMPSeg should be
     * written along with the default model. For INI files without RCOMPSeg
     * specified, use RCOMPSeg = 0x0080.
     */
    uint8_t RCOMPSeg[MAX17048_REG_SIZE] = {MAX17048_RCOMP_SEG_MSB, MAX17048_RCOMP_SEG_LSB};
    for (uint8_t addr = MAX17048_REG_RCOMP_SEG_START; addr < MAX17048_REG_RCOMP_SEG_END; addr += MAX17048_REG_SIZE) {
        if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, addr, RCOMPSeg, MAX17048_MAX_BURST_LEN)) != E_NO_ERROR) {
            PR_ERROR("i2c_master_reg_write failed %d", err);
            return err;
        }
    }

    /*
     * INFO: Step 7. Write OCV
     * This OCV should produce the SOC_Check values in Step 9
     */
    tuMax17048RegOcv lMax17048RegOcv = { 0 };
    lMax17048RegOcv.bits.OcvMsb = MAX17048_OCV_TEST_MSB;
    lMax17048RegOcv.bits.OcvLsb = MAX17048_OCV_TEST_LSB;
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_OCV, lMax17048RegOcv.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /*
     * INFO: Step 7.1. Disable Hibernate (MAX17048/49 only)
     * The IC updates SOC less frequently in hibernate mode, so make sure it
     * is not hibernating
     */
    tuMax17048RegHibrt lMax17048RegHibrt = { 0 };
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_HIBRT, lMax17048RegHibrt.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /*
     * INFO: Step 7.2. Lock model access
     * To allow the ModelGauge algorithm to run in MAX17048/49/58/59 only, the model
     * must be locked. This is harmless but unnecessary for MAX17040/1/3/4
     */
    lMax17048RegUnlock.bits.UnlockMsb = 0x00;
    lMax17048RegUnlock.bits.UnlockLsb = 0x00;
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_UNLOCK, lMax17048RegUnlock.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /*
     * INFO: Step 8.  Delay between 150ms and 600ms
     * This delay must be between 150ms and 600ms. Delaying beyond 600ms could
     * cause the verification to fail.
     */
    MXC_Delay(MXC_DELAY_MSEC(MAX17048_MODEL_INIT_DELAY));

    /*
     * INFO: Step 9. Read SOC Register and compare to expected result
     * There will be some variation in the SOC register due to the ongoing
     * activity of the ModelGauge algorithm. Therefore, the upper byte of the SOC
     * register is verified to be within a specified range to verify that the
     * model was loaded correctly. This value is not an indication of the state of
     * the actual battery. Please note that INI_SOCCheckA and INI_SOCCheckB has a
     * fixed LSB of 1/256% for both 18 and 19 bit models.
     */
    tuMax17048RegSoc lMax17048RegSoc = { 0 };
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_SOC, lMax17048RegSoc.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }

    if((lMax17048RegSoc.bits.SocMsb < MAX17048_SOC_CHECK_A) || (lMax17048RegSoc.bits.SocMsb > MAX17048_SOC_CHECK_B))
    {
        PR_ERROR("Expected SOC does not match! %u", lMax17048RegSoc.bits.SocMsb);
        return E_BAD_STATE;
    }

    /*
     * INFO: Step 9.1 Unlock model access
     * To write OCV, MAX17048/49/58/59 requires model access to be unlocked.
     */
    lMax17048RegUnlock.bits.UnlockMsb = MAX17048_MODEL_UNLOCK_MSB;
    lMax17048RegUnlock.bits.UnlockLsb = MAX17048_MODEL_UNLOCK_LSB;
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_UNLOCK, lMax17048RegUnlock.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /*
     * INFO: Step 10. Restore CONFIG and OCV
     * It is up to the application how to configure the LSB of the CONFIG
     * register; any byte value is valid
     */
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_OCV, lMax17048RegOcvOrig.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }


    /*
     * INFO: Step 10.1, Restore Hibernate (MAX17048/49 only)
     * Remember to restore your desired Hibernate configuration after the
     * model was verified.
     */
    lMax17048RegHibrt.bits.HibThr = MAX17048_HIBRT_HIBTHR;
    lMax17048RegHibrt.bits.ActThr = MAX17048_HIBRT_ACTTHR;
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_HIBRT, lMax17048RegHibrt.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /*
     * INFO: Step 11. Lock Model Access
     */
    lMax17048RegUnlock.bits.UnlockMsb = 0x00;
    lMax17048RegUnlock.bits.UnlockLsb = 0x00;
    if ((err = i2c_master_reg_write_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_UNLOCK, lMax17048RegUnlock.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_write failed %d", err);
        return err;
    }

    /*
     * INFO: Step 12. Wait 150ms Minimum
     * This delay must be at least 150mS before reading the SOC Register to allow
     * the correct value to be calculated by the device.
     */
    MXC_Delay(MXC_DELAY_MSEC(MAX17048_MODEL_INIT_DELAY));

    return E_NO_ERROR;
}

static int fuel_gauge_soc(uint8_t *soc)
{
    int err;
    float fSoC = 0;

    tuMax17048RegSoc lMax17048RegSoc = { 0 };
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_SOC, lMax17048RegSoc.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }

    fSoC = ((lMax17048RegSoc.bits.SocMsb << 8) + lMax17048RegSoc.bits.SocLsb) / 256.0f;
    *soc = round(fSoC);

    if (*soc > 100) {
        *soc = 100;
    }

    PR_DEBUG("soc %d %f", *soc, (double) fSoC);

    return E_NO_ERROR;
}

static int fuel_gauge_vcell(float *vcell)
{
    int err;

    tuMax17048RegVcell lMax17048RegVcell = { 0 };
    if ((err = i2c_master_reg_read_buf(I2C_ADDR_MAX17048_FUEL_GAUGE, MAX17048_REG_VCELL, lMax17048RegVcell.raw, MAX17048_REG_SIZE)) != E_NO_ERROR) {
        PR_ERROR("i2c_master_reg_read_buf failed %d", err);
        return err;
    }

    *vcell  = ((lMax17048RegVcell.bits.VCellMsb << 8) + lMax17048RegVcell.bits.VCellLsb) * 0.078125f;
    *vcell /= 1000.0f;

    PR_DEBUG("vcell %f", (double) *vcell);

    return E_NO_ERROR;
}
