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
#include "spixfc_regs.h"
#include "spixfm_regs.h"

/***** Function Prototypes *****/
int MXC_SPIXF_RevA_Init (uint32_t cmdval, uint32_t frequency);
void MXC_SPIXF_RevA_Shutdown (void);
void MXC_SPIXF_RevA_IOCtrl (mxc_spixf_ds_t sclk_ds, mxc_spixf_ds_t ss_ds, mxc_spixf_ds_t sdio_ds, mxc_spixf_pup_t pupdctrl);
int MXC_SPIXF_RevA_Clocks (uint32_t len, uint8_t deass);
int MXC_SPIXF_RevA_Transaction (mxc_spixf_req_t *req);
int MXC_SPIXF_RevA_TransactionAsync (mxc_spixf_req_t *req);
int MXC_SPIXF_RevA_AbortAsync (mxc_spixf_req_t *req);
void MXC_SPIXF_RevA_Handler (void);
int MXC_SPIXF_RevA_ReadyForSleep (void);
int MXC_SPIXF_RevA_EnableInt (uint32_t mask);
int MXC_SPIXF_RevA_DisableInt (uint32_t mask);
int MXC_SPIXF_RevA_ClearFlags (uint32_t flags);
int MXC_SPIXF_RevA_GetFlags (void);

//Low level
int MXC_SPIXF_RevA_SetMode (mxc_spixf_mode_t mode);
mxc_spixf_mode_t MXC_SPIXF_RevA_GetMode (void);
int MXC_SPIXF_RevA_SetSSPolActiveHigh (void);
int MXC_SPIXF_RevA_SetSSPolActiveLow (void);
int MXC_SPIXF_RevA_GetSSPolarity (void);
int MXC_SPIXF_RevA_SetSPIFrequency (unsigned int hz);
uint32_t MXC_SPIXF_RevA_GetSPIFrequency (void);
int MXC_SPIXF_RevA_SetSSActiveTime (mxc_spixf_ssact_t ssact);
mxc_spixf_ssact_t MXC_SPIXF_RevA_GetSSActiveTime (void);
int MXC_SPIXF_RevA_SetSSInactiveTime (mxc_spixf_ssiact_t ssiact);
mxc_spixf_ssiact_t MXC_SPIXF_RevA_GetSSInactiveTime (void);
int MXC_SPIXF_RevA_SetCmdWidth (mxc_spixf_spiwidth_t width);
mxc_spixf_spiwidth_t MXC_SPIXF_RevA_GetCmdWidth (void);
int MXC_SPIXF_RevA_SetAddrWidth (mxc_spixf_spiwidth_t width);
mxc_spixf_spiwidth_t MXC_SPIXF_RevA_GetAddrWidth (void);
int MXC_SPIXF_RevA_SetDataWidth (mxc_spixf_spiwidth_t width);
mxc_spixf_spiwidth_t MXC_SPIXF_RevA_GetDataWidth (void);
int MXC_SPIXF_RevA_Set4ByteAddr (void);
int MXC_SPIXF_RevA_Set3ByteAddr (void);
unsigned int MXC_SPIXF_RevA_GetBytesPerAddr (void);
int MXC_SPIXF_RevA_SetModeClk (uint8_t mdclk);
uint8_t MXC_SPIXF_RevA_GetModeClk (void);
int MXC_SPIXF_RevA_SetCmdModeEveryTrans (void);
int MXC_SPIXF_RevA_SetCmdModeFirstTrans (void);
mxc_spixf_cmd_t MXC_SPIXF_RevA_GetCmdMode (void);
int MXC_SPIXF_RevA_BBDataOutputEnable (uint8_t mask);
int MXC_SPIXF_RevA_BBDataOutputDisable (uint8_t mask);
uint8_t MXC_SPIXF_RevA_BBDataOutputIsEnabled (void);
uint8_t MXC_SPIXF_RevA_GetBBDataOutputValue (void);
uint8_t MXC_SPIXF_RevA_GetBBDataInputValue (void);
int MXC_SPIXF_RevA_SetModeData (uint16_t data);
uint16_t MXC_SPIXF_RevA_GetModeData (void);
int MXC_SPIXF_RevA_SetSCKInverted (void);
int MXC_SPIXF_RevA_SetSCKNonInverted (void);
int MXC_SPIXF_RevA_GetSCKInverted (void);
int MXC_SPIXF_RevA_SCKFeedbackEnable (void);
int MXC_SPIXF_RevA_SCKFeedbackDisable (void);
int MXC_SPIXF_RevA_SCKFeedbackIsEnabled (void);
int MXC_SPIXF_RevA_SetSCKSampleDelay (uint8_t delay);
uint8_t MXC_SPIXF_RevA_GetSCKSampleDelay (void);
int MXC_SPIXF_RevA_SetCmdValue (uint8_t cmdval);
uint8_t MXC_SPIXF_RevA_GetCmdValue (void);
void MXC_SPIXF_RevA_SetPageSize (mxc_spixf_page_size_t size);
mxc_spixf_page_size_t MXC_SPIXF_RevA_GetPageSize (void);
int MXC_SPIXF_RevA_SimpleRXEnabled (void);
int MXC_SPIXF_RevA_SimpleRXDisable (void);
int MXC_SPIXF_RevA_SimpleRXIsEnabled (void);
int MXC_SPIXF_RevA_SimpleModeEnable (void);
int MXC_SPIXF_RevA_SimpleModeDisable (void);
int MXC_SPIXF_RevA_SimpleModeIsEnabled (void);
int MXC_SPIXF_RevA_SampleOutputEnable (uint8_t mask);
int MXC_SPIXF_RevA_SampleOutputDisable (uint8_t mask);
uint8_t MXC_SPIXF_RevA_SampleOutputIsEnabled (void);
uint8_t MXC_SPIXF_RevA_GetSampleOutputValue (void);
void MXC_SPIXF_RevA_SetSCKDriveHigh (void);
void MXC_SPIXF_RevA_SetSCKDriveLow (void);
uint8_t MXC_SPIXF_RevA_GetSCKDrive (void);
void MXC_SPIXF_RevA_SetSSDriveOutputHigh (void);
void MXC_SPIXF_RevA_SetSSDriveOutputLow (void);
uint8_t MXC_SPIXF_RevA_GetSSDriveOutput (void);
int MXC_SPIXF_RevA_BitBangModeEnable (void);
int MXC_SPIXF_RevA_BitBangModeDisable (void);
int MXC_SPIXF_RevA_BitBangModeIsEnabled (void);
int MXC_SPIXF_RevA_RXFIFOEnable (void);
int MXC_SPIXF_RevA_RXFIFODisable (void);
int MXC_SPIXF_RevA_RXFIFOIsEnabled (void);
int MXC_SPIXF_RevA_TXFIFOEnable (void);
int MXC_SPIXF_RevA_TXFIFODisable (void);
int MXC_SPIXF_RevA_TXFIFOIsEnabled (void);
int MXC_SPIXF_RevA_Enable (void);
int MXC_SPIXF_RevA_Disable (void);
int MXC_SPIXF_RevA_IsEnabled (void);
void MXC_SPIXF_RevA_SetIoctrlSDIODriveHigh();
void MXC_SPIXF_RevA_SetIoctrlSDIODriveLow();
uint8_t MXC_SPIXF_RevA_GetIoctrlSDIODrive (void);
void MXC_SPIXF_RevA_SetIoctrlSCLKDriveHigh();
void MXC_SPIXF_RevA_SetIoctrlSCLKDriveLow();
uint8_t MXC_SPIXF_RevA_GetIoctrlSCLKDrive (void);
void MXC_SPIXF_RevA_SetIoctrlSSDriveHigh();
void MXC_SPIXF_RevA_SetIoctrlSSDriveLow();
uint8_t MXC_SPIXF_RevA_GetIoctrlSSDrive (void);
void MXC_SPIXF_RevA_SetPuPdCtrl (mxc_spixf_pup_t pupd);
uint8_t MXC_SPIXF_RevA_GetPuPdCtrl (void);
int MXC_SPIXF_RevA_SetBusIdle (unsigned int busidle);
unsigned int MXC_SPIXF_RevA_GetBusIdle (void);
