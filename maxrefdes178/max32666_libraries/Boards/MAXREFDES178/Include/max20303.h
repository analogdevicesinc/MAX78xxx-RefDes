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
* @file   max20303.h
* @brief  TODO
*/

#ifndef _MAX20303_H_
#define _MAX20303_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define MAX20303_LDO_OUTPUT_ON           1
#define MAX20303_LDO_OUTPUT_OFF          0

#define MAX20303_BUCK_OUTPUT_ON          1
#define MAX20303_BUCK_OUTPUT_OFF         0

#define MAX20303_BOOST_OUTPUT_ON         1
#define MAX20303_BOOST_OUTPUT_OFF        0

#define MAX20303_BUCKBOOST_OUTPUT_ON     1
#define MAX20303_BUCKBOOST_OUTPUT_OFF    0

#define MAX20303_LED_OUTPUT_ON           1
#define MAX20303_LED_OUTPUT_OFF          0


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------

/**
 * @brief   Initializes the I2C peripheral for PMIC communication if needed
 * @param      initialize_i2c is determines whether the I2C peripheral to be initialized or not
 * @returns    #E_NO_ERROR if everything is successful,
 *             @ref MXC_Error_Codes if an error occurred
 */
int MAX20303_initialize(uint8_t initialize_i2c);

/**
 * @brief  Setup for LDO1 of MAX20303 PMIC to enable or disable MAX78000 board 1.8V digital supply.
 * LDO1 is configured as load-switch. Before calling this function, PMIC should be initialized via
 * MAX20303_initialize() function
 * @param    ldo1_onoff  Non-zero value configures LDO1 as load-switch but configures the state as "ON".
 * Value 0 still configures LDO1 as load-switch and sets the state as "OFF.
 /
 * @returns    This function has no return value
 */
void MAX20303_setldo1(uint8_t ldo1_onoff);

/**
 * @brief  Setup for LDO2 of MAX20303 PMIC to enable or disable MAX78000 board 3.3V digital supply.
 * LDO2 is configured as an LDO. Before calling this function, PMIC should be initialized via
 * MAX20303_initialize() function
 * @param    ldo2_onoff  Non-zero value configures LDO1 as load-switch but configures the state as "ON".
 * Value 0 still configures LDO2 as an LDO and sets the state as "OFF.
 /
 * @returns    This function has no return value
 */
void MAX20303_setldo2(uint8_t ldo2_onoff);

/**
 * @brief  Setup for Buck1 of MAX20303 PMIC to enable or disable MAX78000 board 1.8V digital supply.
 * Before calling this function, PMIC should be initialized via MAX20303_initialize() function
 * @param  buck1_onoff   Non-zero value configures Buck1 voltage as 1.8V but configures the state as "ON".
 * Value 0 still configures LDO1 as load-switch and sets the state as "OFF.
 /
 * @returns    This function has no return value
 */
void MAX20303_setbuck1(uint8_t buck1_onoff);

/**
 * @brief  Setup for Buck2 of MAX20303 PMIC to enable or disable MAX78000 board 2.8V digital supply.
 * Before calling this function, PMIC should be initialized via MAX20303_initialize() function
 * @param  buck2_onoff    Non-zero value configures Buck2 voltage to 2.8V and sets the state as "ON".
 * Value 0 still configures Buck2 voltage as 2.8V but sets the state as "OFF"
 /
 * @returns    This function has no return value
 */
void MAX20303_setbuck2(uint8_t buck2_onoff);

/**
 * @brief  Setup for Boost Converter of MAX20303 PMIC to enable or disable LCD backlight.
 * Before calling this function, PMIC should be initialized via MAX20303_initialize() function
 * @param  boost_onoff   Non-zero value enables boost converter
 * Value 0 still configures Boost voltage as 9.6V but configures the state as "OFF"
 * @param  boost_output_level Sets the voltage output level of the boost converter. Please
 * refer to MAX20303 PMIC datasheet for details.
 *
 * @returns    This function has no return value
 */
void MAX20303_setboost(uint8_t boost_onoff, uint8_t boost_output_level);


/**
 * @brief  Setup for BuckBoost Converter of MAX20303 PMIC.
 * Before calling this function, PMIC should be initialized via MAX20303_initialize() function
 * @param  buckboost_onoff   Non-zero value enables buck-boost converter
 *
 * @returns    This function has no return value
 */
void MAX20303_setbuckboost(uint8_t buckboost_onoff);

/**
 * @brief   Controls PMIC's RGB LED - Red Color. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      ledStatus value 0 turns off the Red LED, non-zero value turns on the Red LED
 * @returns    This function has no return value
 */
void MAX20303_led_red(uint8_t ledStatus);

/**
 * @brief   Controls PMIC's RGB LED - Green Color. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      ledStatus value 0 turns off the Green LED, non-zero value turns on the Red LED
 * @returns    This function has no return value
 */
void MAX20303_led_green(uint8_t ledStatus);

/**
 * @brief   Controls PMIC's RGB LED - Blue Color. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      ledStatus value 0 turns off the Blue LED, non-zero value turns on the Red LED
 * @returns    This function has no return value
 */
void MAX20303_led_blue(uint8_t ledStatus);

#endif /* _MAX20303_H_ */
