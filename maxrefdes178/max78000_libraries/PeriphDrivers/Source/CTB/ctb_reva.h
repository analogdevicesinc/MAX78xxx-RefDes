/* ****************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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

#include "ctb.h"

/* ************************************************************************* */
/* Global Control/Configuration functions                                    */
/* ************************************************************************* */

int MXC_CTB_RevA_Init (uint32_t features);
void MXC_CTB_RevA_EnableInt ();
void MXC_CTB_RevA_DisableInt ();
int MXC_CTB_RevA_Ready (void);
void MXC_CTB_RevA_DoneClear (uint32_t features);
uint32_t MXC_CTB_RevA_Done (void);
void MXC_CTB_RevA_Reset (uint32_t features);
void MXC_CTB_RevA_CacheInvalidate (void);
int MXC_CTB_RevA_Shutdown (uint32_t features);
uint32_t MXC_CTB_RevA_GetEnabledFeatures (void);
void MXC_CTB_RevA_Handler (void);

/************************************/
/* CTB DMA - Used for all features  */
/************************************/

void MXC_CTB_RevA_DMA_SetReadSource (mxc_ctb_dma_read_source_t source);
mxc_ctb_dma_read_source_t MXC_CTB_RevA_DMA_GetReadSource (void);
void MXC_CTB_RevA_DMA_SetWriteSource (mxc_ctb_dma_write_source_t source);
mxc_ctb_dma_write_source_t MXC_CTB_RevA_DMA_GetWriteSource (void);
void MXC_CTB_RevA_DMA_SetSource (uint8_t* source);
void MXC_CTB_RevA_DMA_SetDestination (uint8_t* dest);
int MXC_CTB_RevA_DMA_SetupOperation (mxc_ctb_dma_req_t* req);
int MXC_CTB_RevA_DMA_DoOperation (mxc_ctb_dma_req_t* req);
void MXC_CTB_RevA_DMA_StartTransfer (uint32_t length);

/* ************************************************************************* */
/* True Random Number Generator (TRNG) functions                             */
/* ************************************************************************* */

int MXC_CTB_RevA_TRNG_RandomInt (void);
int MXC_CTB_RevA_TRNG_Random (uint8_t* data, uint32_t len);
void MXC_CTB_RevA_TRNG_RandomAsync (uint8_t* data, uint32_t len, mxc_ctb_complete_cb_t callback);

/* ************************************************************************* */
/* Error Correction Code (ECC) functions                                     */
/* ************************************************************************* */

/*******************************/
/* Low Level Functions         */
/*******************************/

void MXC_CTB_RevA_ECC_Enable ();
void MXC_CTB_RevA_ECC_Disable ();
uint32_t MXC_CTB_RevA_ECC_GetResult (void);

/*******************************/
/* High Level Functions        */
/*******************************/

int MXC_CTB_RevA_ECC_Compute (mxc_ctb_ecc_req_t* req);
int MXC_CTB_RevA_ECC_ErrorCheck (mxc_ctb_ecc_req_t* req);
void MXC_CTB_RevA_ECC_ComputeAsync (mxc_ctb_ecc_req_t* req);
void MXC_CTB_RevA_ECC_ErrorCheckAsync (mxc_ctb_ecc_req_t* req);

/* ************************************************************************* */
/* Cyclic Redundancy Check (CRC) functions                                   */
/* ************************************************************************* */

/*******************************/
/* Low Level Functions         */
/*******************************/

void MXC_CTB_RevA_CRC_SetDirection (mxc_ctb_crc_bitorder_t bitOrder);
mxc_ctb_crc_bitorder_t MXC_CTB_RevA_CRC_GetDirection (void);
void MXC_CTB_RevA_CRC_SetPoly (uint32_t poly);
uint32_t MXC_CTB_RevA_CRC_GetPoly (void);
uint32_t MXC_CTB_RevA_CRC_GetResult (void);

/*******************************/
/* High Level Functions        */
/*******************************/

int MXC_CTB_RevA_CRC_Compute (mxc_ctb_crc_req_t* req);
void MXC_CTB_RevA_CRC_ComputeAsync (mxc_ctb_crc_req_t* req);

/* ************************************************************************* */
/* Hash functions                                                            */
/* ************************************************************************* */

/***********************/
/* Low Level Functions */
/***********************/

void MXC_CTB_RevA_Hash_SetFunction (mxc_ctb_hash_func_t function);
mxc_ctb_hash_func_t MXC_CTB_RevA_Hash_GetFunction (void);
void MXC_CTB_RevA_Hash_SetAutoPad (int pad);
int MXC_CTB_RevA_Hash_GetAutoPad (void);
void MXC_CTB_RevA_Hash_GetResult (uint8_t* digest, int* len);
void MXC_CTB_RevA_Hash_SetMessageSize (uint32_t size);
void MXC_CTB_RevA_Hash_SetSource (mxc_ctb_hash_source_t source);
mxc_ctb_hash_source_t MXC_CTB_RevA_Hash_GetSource (void);
void MXC_CTB_RevA_Hash_InitializeHash (void);

/************************/
/* High Level Functions */
/************************/

int MXC_CTB_RevA_Hash_Compute (mxc_ctb_hash_req_t* req);
void MXC_CTB_RevA_Hash_ComputeAsync (mxc_ctb_hash_req_t* req);

/* ************************************************************************* */
/* Cipher functions                                                          */
/* ************************************************************************* */

/************************/
/* Low Level Functions  */
/************************/

void MXC_CTB_RevA_Cipher_SetMode (mxc_ctb_cipher_mode_t mode);
mxc_ctb_cipher_mode_t MXC_CTB_RevA_Cipher_GetMode (void);
void MXC_CTB_RevA_Cipher_SetCipher (mxc_ctb_cipher_t cipher);
mxc_ctb_cipher_t MXC_CTB_RevA_Cipher_GetCipher (void);
void MXC_CTB_RevA_Cipher_SetKeySource (mxc_ctb_cipher_key_t source);
mxc_ctb_cipher_key_t MXC_CTB_RevA_Cipher_GetKeySource (void);
void MXC_CTB_RevA_Cipher_LoadKey (void);
void MXC_CTB_RevA_Cipher_SetOperation (mxc_ctb_cipher_operation_t operation);
void MXC_CTB_RevA_Cipher_SetKey (uint8_t* key, uint32_t len);
void MXC_CTB_RevA_Cipher_SetIV (uint8_t* iv, uint32_t len);
void MXC_CTB_RevA_Cipher_GetIV (uint8_t* ivOut, uint32_t len);

/************************/
/* High Level Functions */
/************************/

int MXC_CTB_RevA_Cipher_Encrypt (mxc_ctb_cipher_req_t* req);
int MXC_CTB_RevA_Cipher_Decrypt (mxc_ctb_cipher_req_t* req);
void MXC_CTB_RevA_Cipher_EncryptAsync (mxc_ctb_cipher_req_t* req);
void MXC_CTB_RevA_Cipher_DecryptAsync (mxc_ctb_cipher_req_t* req);
