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

tsFaceIDFile *pDatabaseInfo = NULL;
tsDistance *pDistance = NULL;
tsMeanDistance *pMeanDistance = NULL;
tsMinDistance *pMinDistance = NULL;

int8_t *pClosestSubId = NULL;
uint32_t closestSubIdBufIdx = 0;
uint8_t *pMinDistanceCounter = NULL;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int init_database(void)
{
	pDatabaseInfo = (tsFaceIDFile *)embeddings;

	pDistance = (tsDistance *)malloc(sizeof(tsDistance)*pDatabaseInfo->numberOfEmbeddings);
	if (pDistance == NULL) {
		return E_NULL_PTR;
	}

	pMeanDistance = (tsMeanDistance *)malloc(sizeof(tsMeanDistance)*pDatabaseInfo->numberOfSubjects);
	if (pMeanDistance == NULL) {
		return E_NULL_PTR;
	}

	pMinDistance = (tsMinDistance *)malloc(sizeof(tsMinDistance)*3);
	if (pMinDistance == NULL) {
		return E_NULL_PTR;
	}

    pClosestSubId = (int8_t *)malloc(closest_sub_buffer_size);
    if (pClosestSubId == NULL) {
		return E_NULL_PTR;
	}
    for(int i=0; i<closest_sub_buffer_size; ++i){
        pClosestSubId[i] = -1;
    }

    pMinDistanceCounter = (uint8_t *)malloc(pDatabaseInfo->numberOfSubjects);
    if (pMinDistanceCounter == NULL) {
		return E_NULL_PTR;
	}
    for(int i=0; i<pDatabaseInfo->numberOfSubjects; ++i){
        pMinDistanceCounter[i] = 0;
    }

	return 0;
}

int uninit_database(void)
{
    free(pDistance);
    free(pMeanDistance);
    free(pMinDistance);
    free(pClosestSubId);
    free(pMinDistanceCounter);

    pDatabaseInfo = NULL;
    pDistance = NULL;
    pMeanDistance = NULL;
    pMinDistance = NULL;

    pClosestSubId = NULL;
    closestSubIdBufIdx = 0;
    pMinDistanceCounter = NULL;

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

char *get_subject(int ID)
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

void get_min_dist_counter(uint8_t **counter, uint8_t *counter_len){
    *counter = pMinDistanceCounter;
    *counter_len = pDatabaseInfo->numberOfSubjects;
}

tsMinDistance *get_min_distance(){
    return pMinDistance;
}

int calculate_minDistance(const uint8_t *embedding)
{
    int8_t *theEmbedding = (int8_t *)embedding;
    int8_t *theEmbeddingOrigin = theEmbedding;

    tsDistance *dist = pDistance;
    tsMeanDistance *meanDist = pMeanDistance;

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

    dist = pDistance;

    for (int i = 0; i < pDatabaseInfo->numberOfSubjects; i++) {
        meanDist[i].subID = i;
        meanDist[i].number = 0;
        meanDist[i].distance = 0;
    }

    for (int i = 0; i < pDatabaseInfo->numberOfEmbeddings; i++) {
        meanDist = pMeanDistance + dist->subID;
        meanDist->distance += dist->distance;
        meanDist->number++;
        dist++;
    }

    meanDist = pMeanDistance;

    for (int i = 0; i < pDatabaseInfo->numberOfSubjects; i++) {
        meanDist->distance = meanDist->distance / meanDist->number;
        meanDist++;
    }

    meanDist = pMeanDistance;

#define MAX_DISTANCE 1000000

    for (int i = 0; i < 3; i++) {
    	pMinDistance[i].subID = 0xFF;
    	pMinDistance[i].distance = MAX_DISTANCE;
    }

    for (int i = 0; i < pDatabaseInfo->numberOfSubjects; i++)
    {
        if (meanDist[i].distance < pMinDistance[0].distance) {         /* Check if current element is less than firstMin, then update first, second and third */
        	pMinDistance[2].distance = pMinDistance[1].distance;
        	pMinDistance[2].subID = pMinDistance[1].subID;
            pMinDistance[1].distance = pMinDistance[0].distance;
            pMinDistance[1].subID = pMinDistance[0].subID;
            pMinDistance[0].distance = meanDist[i].distance;
            pMinDistance[0].subID  = meanDist[i].subID;
        } else if (meanDist[i].distance < pMinDistance[1].distance) {    /* Check if current element is less than secmin then update second and third */
        	pMinDistance[2].distance = pMinDistance[1].distance;
        	pMinDistance[2].subID = pMinDistance[1].subID;
            pMinDistance[1].distance = meanDist[i].distance;
            pMinDistance[1].subID  = meanDist[i].subID;
        } else if (meanDist[i].distance < pMinDistance[2].distance) {  /* Check if current element is less than then update third */
        	pMinDistance[2].distance = meanDist[i].distance;
        	pMinDistance[2].subID  = meanDist[i].subID;
        }

    }

    uint32_t bufferIdx = closestSubIdBufIdx % (closest_sub_buffer_size);
    if (pClosestSubId[bufferIdx] >= 0){
        --pMinDistanceCounter[pClosestSubId[bufferIdx]];
    }

    if (pMinDistance[0].distance < thresh_for_unknown_subject){
        pClosestSubId[bufferIdx] = pMinDistance[0].subID;
        ++pMinDistanceCounter[pClosestSubId[bufferIdx]];
    } else {
        pClosestSubId[bufferIdx] = -1;
    }
    ++closestSubIdBufIdx;
    
    PR_DEBUG("Results:");
    PR_DEBUG("1. : %d, distance: %d", pMinDistance[0].subID, pMinDistance[0].distance);
    PR_DEBUG("2. : %d, distance: %d", pMinDistance[1].subID, pMinDistance[1].distance);
    PR_DEBUG("3. : %d, distance: %d", pMinDistance[2].subID, pMinDistance[2].distance);
    
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

    return (closestSubIdBufIdx % 3);
}
