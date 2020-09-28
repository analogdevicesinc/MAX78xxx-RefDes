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
 * $Date: 2020-09-02 22:38:26 +0300 (Wed, 02 Sep 2020) $
 * $Revision: 55308 $
 *
 ******************************************************************************/


/*
* FIPS-197 compliant AES implementation
*
* Copyright (C) 2006-2013, Brainspark B.V.
*
* This file is part of PolarSSL (http://www.polarssl.org)
* Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
*
* All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/*
* The AES block cipher was designed by Vincent Rijmen and Joan Daemen.
*
* http://csrc.nist.gov/encryption/aes/rijndael/Rijndael.pdf
* http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
*/

/*************************************************************************************************/
/*!
 *  \brief
 */
/*************************************************************************************************/

#include <string.h>
#include "ll_math.h"
#include "wsf_assert.h"
#include "max32665.h"
#include "gcr_regs.h"
#include "tpu_regs.h"
#include "flc_regs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define MXC_F_TPU_CTRL_DONE_FLAGS       (MXC_F_TPU_CTRL_DMA_DONE | MXC_F_TPU_CTRL_GLS_DONE | \
                                        MXC_F_TPU_CTRL_HSH_DONE | MXC_F_TPU_CTRL_CPH_DONE\
                                        | MXC_F_TPU_CTRL_MAA_DONE)

#define AES_BLOCK_SIZE                  16
#define AES_KEY_SIZE_BITS               128
#define INFO_BLOCK_ADDR                 0x10800000
#define HW_AES_DISABLE_OFFSET           0x198
#define HW_AES_DISABLE_VALUE            0xAA00AA00

/*
* 32-bit integer manipulation macros (little endian)
*/
#define GET_UINT32_LE(n,b,i) \
{ \
(n) = ( (uint32_t) (b)[(i) ] ) \
| ( (uint32_t) (b)[(i) + 1] << 8 ) \
| ( (uint32_t) (b)[(i) + 2] << 16 ) \
| ( (uint32_t) (b)[(i) + 3] << 24 ); \
}

#define PUT_UINT32_LE(n,b,i) \
{ \
(b)[(i) ] = (unsigned char) ( (n) ); \
(b)[(i) + 1] = (unsigned char) ( (n) >> 8 ); \
(b)[(i) + 2] = (unsigned char) ( (n) >> 16 ); \
(b)[(i) + 3] = (unsigned char) ( (n) >> 24 ); \
}

/*
* Tables generation code
*/
#define ROTL8(x) ( ( x << 8 ) & 0xFFFFFFFF ) | ( x >> 24 )
#define XTIME(x) ( ( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ) )
#define MUL(x,y) ( ( x && y ) ? pow[(log[x]+log[y]) % 255] : 0 )

/*
* AES Forward Round
*/
#define AES_FROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3) \
{ \
X0 = *RK++ ^ FT0[ ( Y0 ) & 0xFF ] ^ \
FT1[ ( Y1 >> 8 ) & 0xFF ] ^ \
FT2[ ( Y2 >> 16 ) & 0xFF ] ^ \
FT3[ ( Y3 >> 24 ) & 0xFF ]; \
\
X1 = *RK++ ^ FT0[ ( Y1 ) & 0xFF ] ^ \
FT1[ ( Y2 >> 8 ) & 0xFF ] ^ \
FT2[ ( Y3 >> 16 ) & 0xFF ] ^ \
FT3[ ( Y0 >> 24 ) & 0xFF ]; \
\
X2 = *RK++ ^ FT0[ ( Y2 ) & 0xFF ] ^ \
FT1[ ( Y3 >> 8 ) & 0xFF ] ^ \
FT2[ ( Y0 >> 16 ) & 0xFF ] ^ \
FT3[ ( Y1 >> 24 ) & 0xFF ]; \
\
X3 = *RK++ ^ FT0[ ( Y3 ) & 0xFF ] ^ \
FT1[ ( Y0 >> 8 ) & 0xFF ] ^ \
FT2[ ( Y1 >> 16 ) & 0xFF ] ^ \
FT3[ ( Y2 >> 24 ) & 0xFF ]; \
}

typedef struct {
    int nr; /*!< number of rounds */
    uint32_t *rk; /*!< AES round keys */
    uint32_t buf[68]; /*!< unaligned data */
} aes_context;

/**************************************************************************************************
  Variables
**************************************************************************************************/
/*
* Forward S-box & tables
*/
static unsigned char FSb[256];
static uint32_t FT0[256];
static uint32_t FT1[256];
static uint32_t FT2[256];
static uint32_t FT3[256];

/*
* Round constants
*/
static uint32_t RCON[10];

static int aes_init_done = 0;

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
static void aes_gen_tables( void )
{
    int i, x, y, z;
    int pow[256];
    int log[256];

    /*
    * compute pow and log tables over GF(2^8)
    */
    for( i = 0, x = 1; i < 256; i++ ) {
        pow[i] = x;
        log[x] = i;
        x = ( x ^ XTIME( x ) ) & 0xFF;
    }

    /*
    * calculate the round constants
    */
    for( i = 0, x = 1; i < 10; i++ ) {
        RCON[i] = (uint32_t) x;
        x = XTIME( x ) & 0xFF;
    }

    /*
    * generate the forward S-boxes
    */
    FSb[0x00] = 0x63;

    for( i = 1; i < 256; i++ ) {
        x = pow[255 - log[i]];

        y = x;
        y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y;
        y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y;
        y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y;
        y = ( (y << 1) | (y >> 7) ) & 0xFF;
        x ^= y ^ 0x63;

        FSb[i] = (unsigned char) x;
    }

    /*
    * generate the forward table
    */
    for( i = 0; i < 256; i++ ) {
        x = FSb[i];
        y = XTIME( x ) & 0xFF;
        z = ( y ^ x ) & 0xFF;

        FT0[i] = ( (uint32_t) y ) ^
                 ( (uint32_t) x << 8 ) ^
                 ( (uint32_t) x << 16 ) ^
                 ( (uint32_t) z << 24 );

        FT1[i] = ROTL8( FT0[i] );
        FT2[i] = ROTL8( FT1[i] );
        FT3[i] = ROTL8( FT2[i] );
    }
}

/*
* AES key schedule (encryption)
*/
/*************************************************************************************************/
static int aes_setkey_enc( aes_context *ctx, const unsigned char *key, unsigned int keysize )
{
    unsigned int i;
    uint32_t *RK;

    if( aes_init_done == 0 ) {
        aes_gen_tables();
        aes_init_done = 1;
    }

    switch( keysize ) {
        case 128:
            ctx->nr = 10;
            break;
        case 192:
            ctx->nr = 12;
            break;
        case 256:
            ctx->nr = 14;
            break;
        default :
            return( -1 );
    }

    ctx->rk = RK = ctx->buf;

    for( i = 0; i < (keysize >> 5); i++ ) {
        GET_UINT32_LE( RK[i], key, i << 2 );
    }

    switch( ctx->nr ) {
        case 10:

            for( i = 0; i < 10; i++, RK += 4 ) {
                RK[4] = RK[0] ^ RCON[i] ^
                        ( (uint32_t) FSb[ ( RK[3] >> 8 ) & 0xFF ] ) ^
                        ( (uint32_t) FSb[ ( RK[3] >> 16 ) & 0xFF ] << 8 ) ^
                        ( (uint32_t) FSb[ ( RK[3] >> 24 ) & 0xFF ] << 16 ) ^
                        ( (uint32_t) FSb[ ( RK[3] ) & 0xFF ] << 24 );

                RK[5] = RK[1] ^ RK[4];
                RK[6] = RK[2] ^ RK[5];
                RK[7] = RK[3] ^ RK[6];
            }
            break;

        case 12:

            for( i = 0; i < 8; i++, RK += 6 ) {
                RK[6] = RK[0] ^ RCON[i] ^
                        ( (uint32_t) FSb[ ( RK[5] >> 8 ) & 0xFF ] ) ^
                        ( (uint32_t) FSb[ ( RK[5] >> 16 ) & 0xFF ] << 8 ) ^
                        ( (uint32_t) FSb[ ( RK[5] >> 24 ) & 0xFF ] << 16 ) ^
                        ( (uint32_t) FSb[ ( RK[5] ) & 0xFF ] << 24 );

                RK[7] = RK[1] ^ RK[6];
                RK[8] = RK[2] ^ RK[7];
                RK[9] = RK[3] ^ RK[8];
                RK[10] = RK[4] ^ RK[9];
                RK[11] = RK[5] ^ RK[10];
            }
            break;

        case 14:

            for( i = 0; i < 7; i++, RK += 8 ) {
                RK[8] = RK[0] ^ RCON[i] ^
                        ( (uint32_t) FSb[ ( RK[7] >> 8 ) & 0xFF ] ) ^
                        ( (uint32_t) FSb[ ( RK[7] >> 16 ) & 0xFF ] << 8 ) ^
                        ( (uint32_t) FSb[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^
                        ( (uint32_t) FSb[ ( RK[7] ) & 0xFF ] << 24 );

                RK[9] = RK[1] ^ RK[8];
                RK[10] = RK[2] ^ RK[9];
                RK[11] = RK[3] ^ RK[10];

                RK[12] = RK[4] ^
                         ( (uint32_t) FSb[ ( RK[11] ) & 0xFF ] ) ^
                         ( (uint32_t) FSb[ ( RK[11] >> 8 ) & 0xFF ] << 8 ) ^
                         ( (uint32_t) FSb[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^
                         ( (uint32_t) FSb[ ( RK[11] >> 24 ) & 0xFF ] << 24 );

                RK[13] = RK[5] ^ RK[12];
                RK[14] = RK[6] ^ RK[13];
                RK[15] = RK[7] ^ RK[14];
            }
            break;

        default:

            break;
    }

    return( 0 );
}

/*
* AES-ECB block encryption/decryption
*/
/*************************************************************************************************/
static int aes_crypt_ecb_enc( aes_context *ctx,
                              const unsigned char input[16],
                              unsigned char output[16] )
{
    int i;
    uint32_t *RK, X0, X1, X2, X3, Y0, Y1, Y2, Y3;

    RK = ctx->rk;

    GET_UINT32_LE( X0, input, 0 );
    X0 ^= *RK++;
    GET_UINT32_LE( X1, input, 4 );
    X1 ^= *RK++;
    GET_UINT32_LE( X2, input, 8 );
    X2 ^= *RK++;
    GET_UINT32_LE( X3, input, 12 );
    X3 ^= *RK++;

    /* AES_ENCRYPT */
    for( i = (ctx->nr >> 1) - 1; i > 0; i-- ) {
        AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
        AES_FROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
    }

    AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

    X0 = *RK++ ^ \
         ( (uint32_t) FSb[ ( Y0 ) & 0xFF ] ) ^
         ( (uint32_t) FSb[ ( Y1 >> 8 ) & 0xFF ] << 8 ) ^
         ( (uint32_t) FSb[ ( Y2 >> 16 ) & 0xFF ] << 16 ) ^
         ( (uint32_t) FSb[ ( Y3 >> 24 ) & 0xFF ] << 24 );

    X1 = *RK++ ^ \
         ( (uint32_t) FSb[ ( Y1 ) & 0xFF ] ) ^
         ( (uint32_t) FSb[ ( Y2 >> 8 ) & 0xFF ] << 8 ) ^
         ( (uint32_t) FSb[ ( Y3 >> 16 ) & 0xFF ] << 16 ) ^
         ( (uint32_t) FSb[ ( Y0 >> 24 ) & 0xFF ] << 24 );

    X2 = *RK++ ^ \
         ( (uint32_t) FSb[ ( Y2 ) & 0xFF ] ) ^
         ( (uint32_t) FSb[ ( Y3 >> 8 ) & 0xFF ] << 8 ) ^
         ( (uint32_t) FSb[ ( Y0 >> 16 ) & 0xFF ] << 16 ) ^
         ( (uint32_t) FSb[ ( Y1 >> 24 ) & 0xFF ] << 24 );

    X3 = *RK++ ^ \
         ( (uint32_t) FSb[ ( Y3 ) & 0xFF ] ) ^
         ( (uint32_t) FSb[ ( Y0 >> 8 ) & 0xFF ] << 8 ) ^
         ( (uint32_t) FSb[ ( Y1 >> 16 ) & 0xFF ] << 16 ) ^
         ( (uint32_t) FSb[ ( Y2 >> 24 ) & 0xFF ] << 24 );

    PUT_UINT32_LE( X0, output, 0 );
    PUT_UINT32_LE( X1, output, 4 );
    PUT_UINT32_LE( X2, output, 8 );
    PUT_UINT32_LE( X3, output, 12 );

    return( 0 );
}

/* Software implementation of AES */
/*************************************************************************************************/
static void LlMathAesEcbSw(const uint8_t *pKey, uint8_t *pOut, const uint8_t *pIn)
{
    int i;
    uint8_t temp[AES_BLOCK_SIZE];
    aes_context aesContext;

    /* Byte swap the key */
    for(i = 0; i < AES_BLOCK_SIZE; i++) {
        temp[i] = pKey[AES_BLOCK_SIZE-1-i];
    }
    aes_setkey_enc(&aesContext, (const unsigned char *)temp, AES_KEY_SIZE_BITS);

    /* Byte swap the input */
    for(i = 0; i < AES_BLOCK_SIZE; i++) {
        temp[i] = pIn[AES_BLOCK_SIZE-1-i];
    }
    aes_crypt_ecb_enc(&aesContext, (const unsigned char*)temp, (unsigned char*)pOut);

    /* Byte swap the output */
    for(i = 0; i < AES_BLOCK_SIZE; i++) {
        temp[i] = pOut[AES_BLOCK_SIZE-1-i];
    }

    /* Copy the byte swapped output to the output buffer */
    memcpy(pOut, temp, AES_BLOCK_SIZE);
}

/*************************************************************************************************/
static bool_t hwAesEnabled(void)
{
    /* SDMA uses soft AES so ARM can use TPU un-interrupted */
#ifdef __CROSSWORKS
    return FALSE;
#else
    
    /* Check to see if info block is unlocked */
    /* Flash at offset 0x20 will appear erased */
    volatile uint32_t infoData = *(volatile uint32_t*)(INFO_BLOCK_ADDR + 0x30);

    if(infoData != 0xFFFFFFFF) {

        /* Write the unlock sequence */
        MXC_FLC0->acntl = 0x3a7f5ca3;
        MXC_FLC0->acntl = 0xa1e34f20;
        MXC_FLC0->acntl = 0x9608b2c1;

        /* Flush all instruction caches */
        MXC_GCR->scon |= MXC_F_GCR_SCON_CCACHE_FLUSH;

        /* Wait for flush to complete */
        while(MXC_GCR->scon & MXC_F_GCR_SCON_CCACHE_FLUSH) {}
    }

    /* Read the HW_AES diable register */
    infoData = *(volatile uint32_t*)(INFO_BLOCK_ADDR + HW_AES_DISABLE_OFFSET);
    if(infoData != HW_AES_DISABLE_VALUE) {
        return TRUE;
    }

    return FALSE;
#endif
}

/*************************************************************************************************/
void LlMathAesEcb(const uint8_t *pKey, uint8_t *pOut, const uint8_t *pIn)
{
    uint32_t perckcn0, clkcn;

    /* Use 32-bit array for alignment and easy data movement */
    /* SDMA will not do a 32-bit copy with un-aligned pointers */
    /* TPU requires 32-bit copy */
    uint32_t temp_32[4];

    /* Determine if harware AES is available */
    if(!hwAesEnabled()) {

#ifdef ENABLE_SOFT_AES
        /* Use software AES if hardware is unavailable */
        LlMathAesEcbSw(pKey, pOut, pIn);
        return;
#endif

        /* Assert if software and hardware AES are unavailable */
        WSF_ASSERT(0);
        return;
    }

    /* Save the state of the crypto clock enable bit */
    perckcn0 = MXC_GCR->perckcn0;

    /* Save the state of the HIRC EN */
    clkcn = MXC_GCR->clkcn;

    /* Enable CRYPTO clock */
    if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC_EN) == 0) {
        MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    }

    /* Disable CRYPTO clock gate */
    if (MXC_GCR->perckcn0 & MXC_F_GCR_PERCKCN0_CRYPTOD) {
        MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_CRYPTOD);
    }

    /* Reset Crypto block and clear state */
    MXC_TPU->ctrl = MXC_F_TPU_CTRL_RST;

    /* Set the legacy bit */
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_FLAG_MODE;

    /* Byte swap the input and output */
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_BSO;
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_BSI;

    MXC_TPU->cipher_ctrl = (MXC_S_TPU_CIPHER_CTRL_MODE_ECB |
                            MXC_S_TPU_CIPHER_CTRL_CIPHER_AES128 | 
                            MXC_S_TPU_CIPHER_CTRL_SRC_CIPHERKEY);

    /* Clear all done flags */
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DONE_FLAGS;

    /* Load the Key */
    memcpy(temp_32, pKey, AES_BLOCK_SIZE);
    MXC_TPU->cipher_key[0] = temp_32[3];
    MXC_TPU->cipher_key[1] = temp_32[2];
    MXC_TPU->cipher_key[2] = temp_32[1];
    MXC_TPU->cipher_key[3] = temp_32[0];

    /* Wait until ready for data */
    while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_RDY));

    /* Copy data to start the operation */
    memcpy(temp_32, pIn, AES_BLOCK_SIZE);
    MXC_TPU->din[0] = temp_32[3];
    MXC_TPU->din[1] = temp_32[2];
    MXC_TPU->din[2] = temp_32[1];
    MXC_TPU->din[3] = temp_32[0];

    /* Wait until operation is complete */
    while (!(MXC_TPU->ctrl & MXC_F_TPU_CTRL_CPH_DONE));

    /* Clear all done flags */
    MXC_TPU->ctrl |= MXC_F_TPU_CTRL_DONE_FLAGS;

    /* Copy data out */
    temp_32[3] = MXC_TPU->dout[0];
    temp_32[2] = MXC_TPU->dout[1];
    temp_32[1] = MXC_TPU->dout[2];
    temp_32[0] = MXC_TPU->dout[3];
    memcpy(pOut, temp_32, AES_BLOCK_SIZE);

    /* Restore clock settings */
    MXC_GCR->perckcn0 = perckcn0;
    MXC_GCR->clkcn = clkcn;
}
