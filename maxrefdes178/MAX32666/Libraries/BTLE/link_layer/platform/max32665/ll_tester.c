/*******************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-06-05 18:22:48 +0300 (Wed, 05 Jun 2019) $
 * $Revision: 43683 $
 *
 ******************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief API used for DTM 
 */
/*************************************************************************************************/

#ifdef LL_ENABLE_TESTER

#include "ll_tester_api.h"
#include "lctr_int_conn.h"
#include "wsf_trace.h"
#include "wsf_assert.h"

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
llTesterCtrlBlock_t  llTesterCb;

/*************************************************************************************************/
void LlTesterInit(void)
{
    
}

/*************************************************************************************************/
void LctrForceTxAcl(uint8_t *pAclBuf)
{
    LL_TRACE_ERR0("LctrForceTxAcl not yet implemented");
    WSF_ASSERT(0);
}

/*************************************************************************************************/
void LctrProcessRxTxAck(lctrConnCtx_t *pCtx, uint8_t *pRxBuf, uint8_t **pNextRxBuf, bool_t *pTxPduIsAcked)
{
    LL_TRACE_ERR0("LctrProcessRxTxAck not yet implemented");
    WSF_ASSERT(0);
}

#endif /* #ifdef LL_ENABLE_TESTER */
