/*******************************************************************************
 * Copyright (C) £2015 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-11-07 18:43:15 -0600 (Thu, 07 Nov 2019) $
 * $Revision: 48537 $
 *
 ******************************************************************************/

#include <stdio.h>
#include "board.h"
#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_assert.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "spixfc.h"
#include "tmr_utils.h"
#include "max20303.h"
#include "i2c.h"


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
    return I2C_MasterWrite(PMIC_I2C, MAX20303_ADDR, cmdData, 2, 0);
}


/**
 * @brief   Initializes the I2C peripheral for PMIC communication if needed
 * @param      initialize_i2c is determines whether the I2C peripheral to be initialized or not
 * @returns    #E_NO_ERROR if everything is successful,
 *             @ref MXC_Error_Codes if an error occurred
 */
void MAX20303_initialize( uint8_t initialize_i2c ) {

	if ( initialize_i2c ) {
	
		I2C_Shutdown( PMIC_I2C );
		I2C_Init( PMIC_I2C, I2C_FAST_MODE, NULL);
		NVIC_EnableIRQ( PMIC_I2C_IRQ );

	}

	// Turn off all PMIC LEDs at startup
	MAX20303_led_red( LED_OUTPUT_OFF );
	MAX20303_led_green( LED_OUTPUT_OFF );
	MAX20303_led_blue( LED_OUTPUT_OFF );



	// Configure LDO and Buck outputs
    TMR_Delay(MXC_TMR0, MSEC(1), 0);
	MAX20303_setbuck1( BUCK_OUTPUT_ON );
    TMR_Delay(MXC_TMR0, MSEC(1), 0);
	MAX20303_setbuck2( BUCK_OUTPUT_ON );


	TMR_Delay(MXC_TMR0, MSEC(1), 0);
	MAX20303_setboost( BOOST_OUTPUT_OFF, 0x10 );

	// Enable this for LCD backlight
	///TMR_Delay(MXC_TMR0, MSEC(10), 0);
	///MAX20303_setboost( BOOST_OUTPUT_ON, 0x10 );

}


/**
 * @brief   Controls PMIC's RGB LED - Red Color. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      ledStatus value 0 turns off the Red LED, non-zero value turns on the Red LED
 * @returns    This function has no return value
 */
void MAX20303_led_red( uint8_t ledStatus ) {

	if ( ledStatus == LED_OUTPUT_OFF )
		MAX20303_writeReg( REG_LED1_DIRECT, 0x01 );
	else
		MAX20303_writeReg( REG_LED1_DIRECT, 0x21 );

}

/**
 * @brief   Controls PMIC's RGB LED - Green Color. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      ledStatus value 0 turns off the Green LED, non-zero value turns on the Red LED
 * @returns    This function has no return value
 */
void MAX20303_led_green( uint8_t ledStatus ) {

	if ( ledStatus == LED_OUTPUT_OFF )
		MAX20303_writeReg( REG_LED2_DIRECT, 0x01 );
	else
		MAX20303_writeReg( REG_LED2_DIRECT, 0x21 );
}

/**
 * @brief   Controls PMIC's RGB LED - Blue Color. Before calling this function, PMIC should
 * be initialized via MAX20303_initialize() function
 * @param      ledStatus value 0 turns off the Blue LED, non-zero value turns on the Red LED
 * @returns    This function has no return value
 */
void MAX20303_led_blue( uint8_t ledStatus ) {

	if ( ledStatus == LED_OUTPUT_OFF )
		MAX20303_writeReg( REG_LED0_DIRECT, 0x01 );
	else
		MAX20303_writeReg( REG_LED0_DIRECT, 0x21 );
}

/**
 * @brief  Setup for LDO1 of MAX20303 PMIC to enable or disable MAX78000 board 1.8V digital supply.
 * LDO1 is configured as load-swittch. Before calling this function, PMIC should be initialized via
 * MAX20303_initialize() function
 * @param    ldo1_onoff  Non-zero value configures LDO1 as load-switch but configures the state as "ON".
 * Value 0 still configures LDO1 as load-switch and configures the state as "OFF.
 /
 * @returns    This function has no return value
 */
void MAX20303_setldo1( uint8_t ldo1_onoff )
{

	if ( ldo1_onoff == LDO_OUTPUT_OFF )
		MAX20303_writeReg(REG_AP_DATOUT0, 0x04);  // LDO1Md:1 Load-switch mode.  LDO1En:01   Disabled
	else
		MAX20303_writeReg(REG_AP_DATOUT0, 0x05);  // LDO1Md:1 Load-switch mode.  LDO1En:01   Enabled


    MAX20303_writeReg(REG_AP_DATOUT1, 0x34);      // LDO1Vset : 0x34 = 52.  ( 52 * 25mv) + 0.5V = 1.8V
    MAX20303_writeReg(REG_AP_CMDOUT, APREG_LDO1_CONFIG_WRITE);

}


/**
 * @brief  Setup for Buck1 of MAX20303 PMIC to enable or disable MAX78000 board 1.8V digital supply.
 * Before calling this function, PMIC should be initialized via MAX20303_initialize() function
 * @param  buck1_onoff   Non-zero value configures Buck1 voltage as 1.8V but configures the state as "ON".
 * Value 0 still configures LDO1 as load-switch and configures the state as "OFF.
 /
 * @returns    This function has no return value
 */
void MAX20303_setbuck1( uint8_t buck1_onoff)
{
    MAX20303_writeReg(REG_AP_DATOUT0, 0x7E);
    MAX20303_writeReg(REG_AP_DATOUT1, 0x28);  	  // Buck1Vset = 0x28=40    ( (40x25mV) + 0.8V = 1.8V)
    MAX20303_writeReg(REG_AP_DATOUT2, 0x1F);  	  // Buck1Iset = 0b1111>375mA,  Buck1IZCSet=0b01>20mA   Buck1 ISet and Zero Crossing Threshold

    if ( buck1_onoff == LDO_OUTPUT_OFF )
    	MAX20303_writeReg(REG_AP_DATOUT3, 0x00);  // Buck1En = 0   Buck1 disabled
    else
    	MAX20303_writeReg(REG_AP_DATOUT3, 0x01);  // Buck1En = 1   Buck1 enabled

    MAX20303_writeReg(REG_AP_CMDOUT, APREG_BUCK1_CONFIG_WRITE);
}


/**
 * @brief  Setup for Buck2 of MAX20303 PMIC to enable or disable MAX78000 board 2.8V digital supply.
 * Before calling this function, PMIC should be initialized via MAX20303_initialize() function
 * @param  buck2_onoff    Non-zero value configures Buck2 voltage to 2.8V and sets the state as "ON".
 * Value 0 still configures Buck2 voltage as 2.8V but configures the state as "OFF"
 /
 * @returns    This function has no return value
 */
void MAX20303_setbuck2( uint8_t buck2_onoff)
{
    MAX20303_writeReg(REG_AP_DATOUT0, 0x7E);
    MAX20303_writeReg(REG_AP_DATOUT1, 0x28);  	  // Buck2Vset = 0x28=40    ( (40x50mV) + 0.8V = 2.8V)
    MAX20303_writeReg(REG_AP_DATOUT2, 0x2F);  	  // Buck2Iset = 0b1111>375mA,  Buck2IZCSet=0b10>30mA   Buck2 ISet and Zero Crossing Threshold

    if ( buck2_onoff == LDO_OUTPUT_OFF )
    	MAX20303_writeReg(REG_AP_DATOUT3, 0x00);  //Buck2En = 0   Buck2 disabled
    else
    	MAX20303_writeReg(REG_AP_DATOUT3, 0x01);  //Buck2En = 1   Buck2 enabled

    MAX20303_writeReg(REG_AP_CMDOUT, APREG_BUCK2_CONFIG_WRITE);
}


/**
 * @brief  Setup for Boost Converter of MAX20303 PMIC to enable or disable LCD backlight.
 * Before calling this function, PMIC should be initialized via MAX20303_initialize() function
 * @param  boost_onoff   Non-zero value configures LDO1 as load-switch but configures the state as "ON".
 * Value 0 still configures Boost voltage as 9.6V but configures the state as "OFF"
 * @param  boost_output_level Sets the voltage output level of the boost converter. Please
 * refer to MAX20303 PMIC datasheet for details.
 *
 * @returns    This function has no return value
 */
void MAX20303_setboost( uint8_t boost_onoff, uint8_t boost_output_level ) {


	if ( boost_output_level > 0x16 )
		 boost_output_level = 0x16;							   //  Prevent setting boost voltage above 10.5V due to specs limitation of the LCD

	MAX20303_writeReg(REG_AP_DATOUT3, boost_output_level);	   //  BstVset = 0x0F=15  ( ( 15 * 250mV) + 5 = 8.75V)

	if ( boost_onoff == BOOST_OUTPUT_OFF )
		MAX20303_writeReg(REG_AP_DATOUT0, 0x00);			   //  Boost Disabled - BSTEn = 0x00
	else
		MAX20303_writeReg(REG_AP_DATOUT0, 0x01);			   //  Boost Enabled - BSTEn = 0x01

	MAX20303_writeReg(REG_AP_DATOUT4, 0x03);				   //  BstSet 0x03 - not sure if this is writeable

	MAX20303_writeReg(REG_AP_CMDOUT, APREG_BST_CONFIG_WRITE);  // = Bset_Config_Write


}



void MAX20303_worker()
{
	MAX20303_led_green( LED_OUTPUT_OFF );
    TMR_Delay(MXC_TMR0, MSEC(500), 0);

    MAX20303_led_green( LED_OUTPUT_ON );
    TMR_Delay(MXC_TMR0, MSEC(500), 0);
}
