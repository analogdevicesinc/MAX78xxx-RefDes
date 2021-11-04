/*******************************************************************************
 * Copyright (C) 2020-2021 Maxim Integrated Products, Inc., All rights Reserved.
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

#include <board.h>
#include <flc.h>
#include <math.h>
#include <mxc_device.h>
#include <mxc_errors.h>
#include <nvic_table.h>
#include <icc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "max78000_debug.h"
#include "max78000_video_embedding_process.h"
#include "maxrefdes178_definitions.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "embed"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef struct  __attribute__((packed)) {
    uint8_t numberOfSubjects;
    uint16_t lengthOfEmbeddings;
    uint16_t numberOfEmbeddings;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uint16_t lengthOfSubjectNames;

}tsFaceIDFile;

typedef struct  __attribute__((packed)) sDistance {
        uint8_t subID;
        int32_t distance;
}tsDistance;


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern uint32_t _embeddings_start_, _embeddings_end_;
static const uint8_t *embeddings = (uint8_t *) &_embeddings_start_;

static tsMeanDistance gMeanDistance[FACEID_MAX_SUBJECT];
static tsMinDistance gMinDistance[sizeof(tsMinDistance) * 3];
static int8_t gClosestSubId[closest_sub_buffer_size];
static uint8_t gMinDistanceCounter[FACEID_MAX_SUBJECT];
static tsDistance gDistance[FACEID_MAX_SUBJECT * FACEID_MAX_PHOTO_PER_SUBJECT];

static tsFaceIDFile *pDatabaseInfo = NULL;
static uint32_t gClosestSubIdBufIdx = 0;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------

/*  embeddings db layout:
 *  1 byte : number of subjects (S)
 *  2 bytes: length of embeddings (L)
 *  2 bytes: number of embeddings (N)
 *  2 bytes: length of image width (W)
 *  2 bytes: length of image height (H)
 *  2 bytes: length of subject names (K)
 *  K bytes: subject names
 *  (L+1)*N bytes: embeddings
 *     1 byte : subject id
 *     L bytes: embedding
 *  (W*H*3)*N bytes: image
 *
 *  embeddings maximum sizes:
 *  1         1 byte : number of subjects (S)        6
 *  2-3       2 bytes: length of embeddings (L)      512
 *  4-5       2 bytes: number of embeddings (N)      6*8=48
 *  6-7       2 bytes: length of image width (W)     120
 *  8-9       2 bytes: length of image height (H)    160
 *  10-11     2 bytes: length of subject names (K)   (15+1)*6 + 1 = 97
 *  12-108    K=97 bytes: subject names
 *  109-24732 (L+1)*N=24624 bytes: embeddings
 */

int init_database(void)
{
	pDatabaseInfo = (tsFaceIDFile *)embeddings;

    for(int i=0; i<closest_sub_buffer_size; ++i){
        gClosestSubId[i] = -1;
    }

    for(int i=0; i<pDatabaseInfo->numberOfSubjects; ++i){
        gMinDistanceCounter[i] = 0;
    }

	return 0;
}

int uninit_database(void)
{
    pDatabaseInfo = NULL;
    gClosestSubIdBufIdx = 0;

    return E_NO_ERROR;
}

void FLC0_IRQHandler(void)
{
    uint32_t temp;
    temp = MXC_FLC0->intr;

    if (temp & MXC_F_FLC_INTR_DONE) {
        MXC_FLC0->intr &= ~MXC_F_FLC_INTR_DONE;
    }

    if (temp & MXC_F_FLC_INTR_AF) {
        MXC_FLC0->intr &= ~MXC_F_FLC_INTR_AF;
    }
}

int update_database(uint8_t *db, uint32_t db_size)
{
    int ret = E_NO_ERROR;

    if (db_size > ((&_embeddings_end_ - &_embeddings_start_) * 4)) {
        PR_ERROR("db_size too big %d > %d", db_size, ((&_embeddings_end_ - &_embeddings_start_) * 4));
        return E_BAD_PARAM;
    }

    // Set flash clock divider to generate a 1MHz clock from the APB clock
    // APB clock is 54MHz on the real silicon
    MXC_FLC0->clkdiv = 24;

    // Setup and enable interrupt
    NVIC_SetVector(FLC0_IRQn, FLC0_IRQHandler);
    NVIC_EnableIRQ(FLC0_IRQn);
    __enable_irq();

    MXC_FLC_EnableInt(MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);

    MXC_ICC_Disable(MXC_ICC0);

    for (uint32_t page_addr = (uint32_t)&_embeddings_start_; page_addr < (uint32_t)&_embeddings_end_; page_addr += MXC_FLASH_PAGE_SIZE) {
        ret = MXC_FLC_PageErase(page_addr);
        if (ret != E_NO_ERROR) {
            PR_ERROR("MXC_FLC_PageErase failed %d %d", page_addr, ret);
            goto bail;
        }
    }

    ret = MXC_FLC_Write((uint32_t)&_embeddings_start_, db_size, (uint32_t* )db);
    if (ret != E_NO_ERROR) {
        PR_ERROR("MXC_FLC_Write failed %d", ret);
        goto bail;
    }

    for (int i = 0; i < db_size; i++) {
        if (db[i] != embeddings[i]) {
            PR_ERROR("verify fail at %d %02hhX != %02hhX", i, db[i], embeddings[i]);
            goto bail;
        }
    }

bail:
    MXC_ICC_Enable(MXC_ICC0);

    return ret;
}

char *get_subject_name(int ID)
{
	char *point = (char *)(pDatabaseInfo+1);

	if (ID == 0) {
		return point;
	}

	int counter = 0;

	for(int i = 0; i < pDatabaseInfo->lengthOfSubjectNames; i++){
		point++;
		if (*point == '\0') {
			counter++;
			if (counter == ID) {
				point++;
				return point;
			}
		}
	}

	return 0;
}

uint8_t get_subject_count(void)
{
    return pDatabaseInfo->numberOfSubjects;
}

uint16_t get_subject_names_len(void)
{
    return pDatabaseInfo->lengthOfSubjectNames;
}

void get_min_dist_counter(uint8_t **counter, uint8_t *counter_len)
{
    *counter = gMinDistanceCounter;
    *counter_len = pDatabaseInfo->numberOfSubjects;
}

tsMinDistance *get_min_distance(){
    return gMinDistance;
}

int calculate_minDistance(const uint8_t *embedding)
{
    int8_t *theEmbedding = (int8_t *)embedding;
    int8_t *theEmbeddingOrigin = theEmbedding;

    tsDistance *dist = gDistance;
    tsMeanDistance *meanDist = gMeanDistance;

    int8_t *pData =  (int8_t *)((uint32_t)(pDatabaseInfo+1) + pDatabaseInfo->lengthOfSubjectNames);

    // Calculate min distance for each embedding
    for (int i = 0; i < pDatabaseInfo->numberOfEmbeddings; i++) {
        int total = 0;
        dist->subID = (uint8_t)(*(pData++));
        theEmbedding = theEmbeddingOrigin;
        for (int j = 0; j < pDatabaseInfo->lengthOfEmbeddings;j++) {
            total += abs((*(theEmbedding++))-(*(pData++)));
        }

        dist->distance = total;
        dist++;
    }

    dist = gDistance;

    for (int i = 0; i < pDatabaseInfo->numberOfSubjects; i++) {
        meanDist[i].subID = i;
        meanDist[i].number = 0;
        meanDist[i].distance = 0;
    }

    for (int i = 0; i < pDatabaseInfo->numberOfEmbeddings; i++) {
        meanDist = gMeanDistance + dist->subID;
        meanDist->distance += dist->distance;
        meanDist->number++;
        dist++;
    }

    meanDist = gMeanDistance;

    for (int i = 0; i < pDatabaseInfo->numberOfSubjects; i++) {
        meanDist->distance = meanDist->distance / meanDist->number;
        meanDist++;
    }

    meanDist = gMeanDistance;

#define MAX_DISTANCE 1000000

    for (int i = 0; i < 3; i++) {
        gMinDistance[i].subID = 0xFF;
        gMinDistance[i].distance = MAX_DISTANCE;
    }

    for (int i = 0; i < pDatabaseInfo->numberOfSubjects; i++)
    {
        if (meanDist[i].distance < gMinDistance[0].distance) {         /* Check if current element is less than firstMin, then update first, second and third */
            gMinDistance[2].distance = gMinDistance[1].distance;
            gMinDistance[2].subID = gMinDistance[1].subID;
            gMinDistance[1].distance = gMinDistance[0].distance;
            gMinDistance[1].subID = gMinDistance[0].subID;
            gMinDistance[0].distance = meanDist[i].distance;
            gMinDistance[0].subID  = meanDist[i].subID;
        } else if (meanDist[i].distance < gMinDistance[1].distance) {    /* Check if current element is less than secmin then update second and third */
            gMinDistance[2].distance = gMinDistance[1].distance;
            gMinDistance[2].subID = gMinDistance[1].subID;
            gMinDistance[1].distance = meanDist[i].distance;
            gMinDistance[1].subID  = meanDist[i].subID;
        } else if (meanDist[i].distance < gMinDistance[2].distance) {  /* Check if current element is less than then update third */
            gMinDistance[2].distance = meanDist[i].distance;
            gMinDistance[2].subID  = meanDist[i].subID;
        }

    }

    uint32_t bufferIdx = gClosestSubIdBufIdx % (closest_sub_buffer_size);
    if (gClosestSubId[bufferIdx] >= 0){
        --gMinDistanceCounter[gClosestSubId[bufferIdx]];
    }

    if (gMinDistance[0].distance < thresh_for_unknown_subject){
        gClosestSubId[bufferIdx] = gMinDistance[0].subID;
        ++gMinDistanceCounter[gClosestSubId[bufferIdx]];
    } else {
        gClosestSubId[bufferIdx] = -1;
    }
    ++gClosestSubIdBufIdx;
    
    PR_DEBUG("Results:");
    PR_DEBUG("1. : %d, distance: %d", gMinDistance[0].subID, gMinDistance[0].distance);
    PR_DEBUG("2. : %d, distance: %d", gMinDistance[1].subID, gMinDistance[1].distance);
    PR_DEBUG("3. : %d, distance: %d", gMinDistance[2].subID, gMinDistance[2].distance);
    
//    printf("\t");
//    for(int i=0; i<closest_sub_buffer_size; ++i){
//        printf("%d, ", pClosestSubId[i]);
//    }
//    printf("\nIdx: %d, Buffer Idx: %d\n", closestSubIdBufIdx, bufferIdx);
//
//    for(int i=0; i<pDatabaseInfo->numberOfSubjects; ++i){
//        printf("\tId %d: %d\n", i, pMinDistanceCounter[i]);
//    }
//    printf("\n\n");

    return (gClosestSubIdBufIdx % 3);
}
