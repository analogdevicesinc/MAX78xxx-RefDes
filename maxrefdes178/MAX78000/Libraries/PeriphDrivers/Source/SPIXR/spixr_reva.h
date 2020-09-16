/* ****************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 *************************************************************************** */

/****** Includes *******/
#include "mxc_device.h"
#include "mxc_sys.h"
#include "spixr_regs.h"
#include "spixr.h"

/***** Function Prototypes *****/

int MXC_SPIXR_RevA_ReadRXFIFO (uint8_t* buf, int len);
int MXC_SPIXR_RevA_WriteTXFIFO (uint8_t* buf, int len);
void MXC_SPIXR_RevA_SetSS (int ssIdx);
int MXC_SPIXR_RevA_GetSS (void);
void MXC_SPIXR_RevA_SetSSCtrl (int stayActive);
int MXC_SPIXR_RevA_GetSSCtrl (void);
void MXC_SPIXR_RevA_Enable (void);
void MXC_SPIXR_RevA_Disable (void);
int MXC_SPIXR_RevA_IsEnabled (void);
void MXC_SPIXR_RevA_ThreeWireModeEnable (void);
void MXC_SPIXR_RevA_ThreeWireModeDisable (void);
int MXC_SPIXR_RevA_ThreeWireModeIsEnabled (void);
int MXC_SPIXR_RevA_GetTXFIFOCount (void);
int MXC_SPIXR_RevA_GetRXFIFOCount (void);
int MXC_SPIXR_RevA_SetWidth (mxc_spixr_width_t width);
int MXC_SPIXR_RevA_SetSPIMode (mxc_spixr_mode_t mode);
int MXC_SPIXR_RevA_SetSSPolarity (int active);
void MXC_SPIXR_RevA_SetSSTiming (unsigned int ssIActDelay, unsigned int postActive, unsigned int preActive);
int MXC_SPIXR_RevA_SetFrequency (int hz);
int MXC_SPIXR_RevA_GetFrequency (void);
int MXC_SPIXR_RevA_GetIntFlags (void);
void MXC_SPIXR_RevA_EnableInt (int flags);
void MXC_SPIXR_RevA_DisableInt (int flags);
int MXC_SPIXR_RevA_GetWakeUpFlags (void);
void MXC_SPIXR_RevA_EnableWakeUp (int flags);
void MXC_SPIXR_RevA_DisableWakeUp (int flags);
void MXC_SPIXR_RevA_ExMemEnable (void);
void MXC_SPIXR_RevA_ExMemDisable (void);
void MXC_SPIXR_RevA_ExMemUseDummy (int delay255);
void MXC_SPIXR_RevA_ExMemSetWriteCommand (uint8_t command);
uint8_t MXC_SPIXR_RevA_ExMemGetWriteCommand (void);
void MXC_SPIXR_RevA_ExMemSetReadCommand (uint8_t command);
uint8_t MXC_SPIXR_RevA_ExMemGetReadCommand (void);
int MXC_SPIXR_RevA_Busy (void);
int MXC_SPIXR_RevA_Init (mxc_spixr_cfg_t* cfg) ;
int MXC_SPIXR_RevA_Shutdown (void);
void MXC_SPIXR_RevA_SendCommand (uint8_t* cmd, uint32_t length, uint32_t tx_num_char);
void MXC_SPIXR_RevA_TXFIFOEnable (void);
void MXC_SPIXR_RevA_TXFIFODisable (void);
int MXC_SPIXR_RevA_TXFIFOIsEnabled (void);
void MXC_SPIXR_RevA_DmaTXFIFOEnable (void);
void MXC_SPIXR_RevA_DmaTXFIFODisable (void);
int MXC_SPIXR_RevA_DmaTXFIFOIsEnabled (void);
void MXC_SPIXR_RevA_RXFIFOEnable (void);
void MXC_SPIXR_RevA_RXFIFODisable (void);
int MXC_SPIXR_RevA_RXFIFOIsEnabled (void);
void MXC_SPIXR_RevA_DmaRXFIFOEnable (void);
void MXC_SPIXR_RevA_DmaRXFIFODisable (void);
int MXC_SPIXR_RevA_DmaRXFIFOIsEnabled (void);
void MXC_SPIXR_RevA_TXFIFOClear (void);
void MXC_SPIXR_RevA_RXFIFOClear (void);