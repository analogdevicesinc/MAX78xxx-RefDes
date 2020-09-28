/* *****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-08-28 17:03:02 -0500 (Tue, 28 Aug 2018) $
 * $Revision: 37424 $
 *
 **************************************************************************** */

/* **** Includes **** */
#include "owm.h"

/* **** Definitions **** */

/* **** Globals **** */

/* **** Functions **** */

/* ************************************************************************* */
int MXC_OWM_RevA_Init (const mxc_owm_cfg_t *cfg);
void MXC_OWM_RevA_Shutdown (void);
int MXC_OWM_RevA_Reset (void);
int MXC_OWM_RevA_TouchByte (uint8_t data);
int MXC_OWM_RevA_WriteByte (uint8_t data);
int MXC_OWM_RevA_ReadByte (void);
int MXC_OWM_RevA_TouchBit (uint8_t bit);
int MXC_OWM_RevA_WriteBit (uint8_t bit);
int MXC_OWM_RevA_ReadBit (void);
int MXC_OWM_RevA_Write (uint8_t* data, int len);
int MXC_OWM_RevA_Read (uint8_t* data, int len);
int MXC_OWM_RevA_ReadROM (uint8_t* ROMCode);
int MXC_OWM_RevA_MatchROM (uint8_t* ROMCode);
int MXC_OWM_RevA_ODMatchROM (uint8_t* ROMCode);
int MXC_OWM_RevA_SkipROM (void);
int MXC_OWM_RevA_ODSkipROM (void);
int MXC_OWM_RevA_Resume (void);
int MXC_OWM_RevA_SearchROM (int newSearch, uint8_t* ROMCode);
void MXC_OWM_RevA_ClearFlags (uint32_t mask);
unsigned MXC_OWM_RevA_GetFlags (void);
void MXC_OWM_RevA_SetExtPullup (int enable);
void MXC_OWM_RevA_SetOverdrive (int enable);
void MXC_OWM_RevA_EnableInt (int flags);
void MXC_OWM_RevA_DisableInt (int flags);
int MXC_OWM_RevA_SetForcePresenceDetect (int enable);
int MXC_OWM_RevA_SetInternalPullup (int enable);
int MXC_OWM_RevA_SetExternalPullup (mxc_owm_ext_pu_t ext_pu_mode);
int MXC_OWM_RevA_SystemClockUpdated (void);
int MXC_OWM_RevA_SetSearchROMAccelerator (int enable);
int MXC_OWM_RevA_BitBang_Init (int initialState);
int MXC_OWM_RevA_BitBang_Read (void);
int MXC_OWM_RevA_BitBang_Write (int state);
int MXC_OWM_RevA_BitBang_Disable (void);

