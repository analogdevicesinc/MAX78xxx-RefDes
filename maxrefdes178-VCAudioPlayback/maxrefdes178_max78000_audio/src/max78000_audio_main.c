/*******************************************************************************
 * Copyright (C) 2016-2023 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-09-05 16:46:11 -0500 (Wed, 05 Sep 2018) $
 * $Revision: 37695 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_delay.h"
#include "mxc_device.h"
#include "board.h"
#include "dma.h"
#include "gpio.h"
#include "i2c.h"
#include "i2s.h"
#include "icc.h"
#include "led.h"
#include "nvic_table.h"
#include "pb.h"
#include "tmr.h"

#include "audio.h"
#include "kws.h"
#include "max78000_audio_cnn.h"
#include "max78000_tornadocnn.h"
#include "maxrefdes178_utility.h"

/***** Globals *****/
static WavFile_t audioFile;
static int32_t ml_data[NUM_OUTPUTS];
static q15_t ml_softmax[NUM_OUTPUTS];
static uint8_t pAI85Buffer[SAMPLE_SIZE];
static uint16_t thresholdLow = THRESHOLD_LOW;
static const char keywords[NUM_OUTPUTS][10] = { "UP", "DOWN", "LEFT", "RIGHT", "STOP",
                                         "GO", "YES", "NO", "ON", "OFF", "ONE", "TWO", "THREE", "FOUR", "FIVE",
                                         "SIX", "SEVEN", "EIGHT", "NINE", "ZERO", "Unknown"
                                       };

// *****************************************************************************
int main(void)
{
	MXC_Delay(MXC_DELAY_MSEC(500));

	int err;
	mxc_gpio_cfg_t audClkEn = MAX78000_AUDIO_AUDIO_OSC_PIN;
	mxc_gpio_cfg_t micEn    = MAX78000_AUDIO_MIC_EN_PIN;
	mxc_gpio_cfg_t micSel   = MAX78000_AUDIO_MIC_SEL_PIN;
	mxc_gpio_cfg_t cnnBoost = MAX78000_AUDIO_CNN_BOOST_PIN;

    uint32_t sampleCounter = 0;
    mxc_tmr_unit_t units;
    uint8_t pChunkBuff[CHUNK];
    uint16_t avg = 0;
    uint16_t ai85Counter = 0;
    uint16_t wordCounter = 0;
    uint16_t avgSilenceCounter = 0;
    classification_result_t classification_result = {0};
    AudioCmd_t audioCmd = NO_CMD;

	MXC_ICC_Enable(MXC_ICC0);

    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    /* Enable peripheral, enable CNN interrupt, turn on CNN clock */
    cnn_enable(MXC_S_GCR_PCLKDIV_CNNCLKSEL_PCLK, MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1);

    /* Configure Microphone, Codec and CNN GPIOs */
    GPIO_SET(cnnBoost);
    MXC_GPIO_Config(&cnnBoost);		// CNN

    MXC_GPIO_Config(&micEn);
    MXC_GPIO_Config(&micSel);		// Mic

	MXC_GPIO_Config(&audClkEn);		// Audio Codec/I2S Clock
	GPIO_SET(audClkEn);
	MXC_Delay(MXC_DELAY_MSEC(5));

	/* Bring CNN state machine into consistent state */
    memset(pAI85Buffer, 0x0, sizeof(pAI85Buffer));
    cnn_init();
    cnn_load_weights();
    cnn_configure();

	// Initialize Host Comms QSPI
	MXC_DMA_Init();
	qspi_slave_init();

	while(1) {

		GPIO_SET(micEn);
		GPIO_CLR(micSel);
		KWS_i2sInit();

		LED_Off(0);
		LED_Off(2);
		LED_On(1);

	    /* Wait for keyword to be detected */
		do {
			audioCmd = NO_CMD;

		    /* Read from Mic driver to get CHUNK worth of samples, otherwise next sample*/
		    if (MicReadChunk(pChunkBuff, &avg) == 0) {
		        continue;
		    }

		    sampleCounter += CHUNK;

		    uint8_t ret=0;

		    /* add sample, rearrange buffer */
		    ret = AddTranspose(pChunkBuff, pAI85Buffer, CHUNK, SAMPLE_SIZE, TRANSPOSE_WIDTH);

		    /* increment number of stored samples */
		    ai85Counter += CHUNK;

		    /* if there is silence after at least 1/3 of samples passed, increment number of times back to back silence to find end of keyword */
		    if ((avg < thresholdLow) && (ai85Counter >= SAMPLE_SIZE / 3)) {
		        avgSilenceCounter ++;
		    } else {
		        avgSilenceCounter = 0;
		    }


		    /* if this is the last sample and there are not enough samples to
		     * feed to CNN, or if it is long silence after keyword,  append with zero (for reading file)
		     */
		    if (avgSilenceCounter > SILENCE_COUNTER_THRESHOLD)
		    {
		        memset(pChunkBuff,0,CHUNK);
		        ret = 0;
		        while (!ret) {
		            ret = AddTranspose(pChunkBuff, pAI85Buffer, CHUNK, SAMPLE_SIZE, TRANSPOSE_WIDTH);
		            ai85Counter += CHUNK;
		        }
		    }

		    /* if enough samples are collected, start CNN */
		    if (ai85Counter >= SAMPLE_SIZE) {
		        int16_t out_class = -1;
		        double probability = 0;

		        /* reset counters */
		        ai85Counter = 0;
		        avgSilenceCounter = 0;

		        /* new word */
		        wordCounter++;

		        /* sanity check, last transpose should have returned 1, as enough samples should have already been added */
		        if (ret != 1) {
		            fail();
		        }

		        /* load to CNN */
		        if (!cnn_load_data(pAI85Buffer)) {
		            fail();
		        }

		        /* Start CNN */
		        if (!cnn_start()) {
		            fail();
		        }

		        /* Wait for CNN  to complete */
		        while (cnn_time == 0) {
		            __WFI();
		        }

		        /* read data */
		        cnn_unload((uint32_t *)ml_data);

		        /* Get time */
		        MXC_TMR_GetTime(MXC_TMR0, cnn_time, (uint32_t*) &cnn_time, &units);

		        switch (units) {
		        case TMR_UNIT_NANOSEC:
		            cnn_time /= 1000;
		            break;
		        case TMR_UNIT_MILLISEC:
		            cnn_time *= 1000;
		            break;
		        case TMR_UNIT_SEC:
		            cnn_time *= 1000000;
		            break;
		        default:
		            break;
		        }

		        /* run softmax */
		        softmax_q17p14_q15((const q31_t*) ml_data, NUM_OUTPUTS, ml_softmax);

		        /* find detected class with max probability */
		        ret = check_inference(ml_softmax, ml_data, &out_class, &probability);

		        if (!ret) {
		            classification_result.classification = CLASSIFICATION_LOW_CONFIDENCE;
		        } else {
		            classification_result.classification = CLASSIFICATION_DETECTED;
		        }

		        if (strcmp(keywords[out_class], "Unknown") == 0) {
		            classification_result.classification = CLASSIFICATION_UNKNOWN;
		        }

		        memcpy(classification_result.result, keywords[out_class], sizeof(classification_result.result));
		        classification_result.probability = probability;

		        if(classification_result.classification == CLASSIFICATION_DETECTED) {
					if(strcmp(classification_result.result, "GO") == 0) {
						audioCmd = GO;
					}
					else if(strcmp(classification_result.result, "RIGHT") == 0) {
						audioCmd = RIGHT;
					}
					else if(strcmp(classification_result.result, "LEFT") == 0) {
						audioCmd = LEFT;
					}
		        }

		        Max = 0;
		        Min = 0;
		    }
		} while(audioCmd == NO_CMD);

		// Disable mic while Codec in use
		GPIO_CLR(micEn);
		GPIO_SET(micSel);

		LED_Off(0);
		LED_On(2);
		LED_Off(1);
		playing = 1;
		refill = 0;
		bufSelect = 0;

		// request and receive wav file statistics
		if(getAudioFileStats(&audioFile, classification_result) != E_NO_ERROR) {
			playing = 0;
			// break;
		}

		// Fill data buffer(s)
		for(int i = 0; i < NUM_DATA_BUF; i++) {
			if(fillDataBuf(&audioFile, i) < E_NO_ERROR) {
				playing = 0;
				break;
			}
			i2sTXBuf[i].lastTX = 0;
		}

		// Initialize Audio Codec
		max9867_init(&audioFile);

		// Initialize I2S module with parameters of audio recording
		if(codec_i2sInit(&audioFile) != E_NO_ERROR) {
			playing = 0;
			// break;
		}

		while(playing) {
			// Check for button press
			if(PB_Get(0)) {
				stopAudio();
				break;
			}

			// Refill data buffer if necessary
			if(refill && !i2sTXBuf[bufSelect].lastTX) {
				err = fillDataBuf(&audioFile, (NUM_DATA_BUF - bufSelect - 1));
				if(err < E_NO_ERROR) {
					playing = 0;
					break;
				}
				else if(err < I2S_TX_BUFFER_SIZE) {
					i2sTXBuf[NUM_DATA_BUF - bufSelect - 1].lastTX = 1;
				}
				refill = 0;
			}
		}

		//End audio playback
		playing = 0;
		refill = 0;
		bufSelect = 0;
		max9867_shutdown();
	}
}
