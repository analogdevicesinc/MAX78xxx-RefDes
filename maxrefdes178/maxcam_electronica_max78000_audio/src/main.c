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
 *******************************************************************************/

/**
 * @file    main.c
 * @brief   Main for KWS20
 * @details 
 *         
 *          
 */


/* **** Includes **** */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "mxc_sys.h"
#include "bbfc_regs.h"
#include "fcr_regs.h"
#include "icc.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "nvic_table.h"
#include "i2s_regs.h"
#include "board.h"
#include "i2s.h"
#include "tmr.h"
#include "dma.h"
#include "cnn.h"
#include "mxc.h"

#include "qspi_dma.h"
#include "maxcam_debug.h"
#include "faceid_definitions.h"
#include "version.h"

/* **** Definitions **** */
#define S_MODULE_NAME   "main"


/* Enable/Disable Features */
//#define ENABLE_PRINT_ENVELOPE            // enables printing average waveform envelope for samples
//#define ENABLE_CLASSIFICATION_DISPLAY	 // enables printing classification result
#define ENABLE_SILENCE_DETECTION		 // Starts collecting only after avg > THRESHOLD_HIGH, otherwise starts from first sample

/*-----------------------------*/
/* keep following unchanged */
#define SAMPLE_SIZE 		16384 	// size of input vector for CNN, keep it multiple of 128
#define CHUNK				128  	// number of data points to read at a time and average for threshold, keep multiple of 128
#define TRANSPOSE_WIDTH		128 	// width of 2d data model to be used for transpose
#define NUM_OUTPUTS 		21		// number of classes
#define I2S_RX_BUFFER_SIZE	64		// I2S buffer size
#define TFT_BUFF_SIZE		50		// TFT buffer size
/*-----------------------------*/

/* Adjustables */
#define SAMPLE_SCALE_FACTOR    		4		// multiplies 16-bit samples by this scale factor before converting to 8-bit
#define THRESHOLD_HIGH				350  	// voice detection threshold to find beginning of a keyword
#define THRESHOLD_LOW				100  	// voice detection threshold to find end of a keyword
#define SILENCE_COUNTER_THRESHOLD 	20 		// [>20] number of back to back CHUNK periods with avg < THRESHOLD_LOW to declare the end of a word
#define PREAMBLE_SIZE				30*CHUNK// how many samples before beginning of a keyword to include
#define INFERENCE_THRESHOLD   		49 		// min probability (0-100) to accept an inference

/* Peripherals */
#define QSPI_ID             MXC_SPI0

#define DMA_CHANNEL_QSPI            1
#define DMA_CHANNEL_QSPI_IRQ        DMA1_IRQn
#define DMA_CHANNEL_QSPI_IRQ_HAND   DMA1_IRQHandler

#define GPIO_SET(x)         MXC_GPIO_OutSet(x.port, x.mask)
#define GPIO_CLR(x)         MXC_GPIO_OutClr(x.port, x.mask)

/* **** Globals **** */
extern uint32_t cnn_time;
static int32_t ml_data[NUM_OUTPUTS];
static q15_t ml_softmax[NUM_OUTPUTS];
uint8_t pAI85Buffer[SAMPLE_SIZE];
uint8_t pPreambleCircBuffer[PREAMBLE_SIZE];
int16_t Max, Min;
uint16_t thresholdHigh = THRESHOLD_HIGH;
uint16_t thresholdLow = THRESHOLD_LOW;

volatile uint8_t i2s_flag = 0;
int32_t i2s_rx_buffer[I2S_RX_BUFFER_SIZE];
mxc_gpio_cfg_t qspi_int;

/* **** Constants **** */
typedef enum _mic_processing_state {
    STOP = 0,     /* No processing  */
    SILENCE = 1,  /* Threshold not detected yet  */
    KEYWORD = 2   /* Threshold has been detected, gathering keyword samples */
} mic_processing_state;

/* Set of detected words */
char keywords[NUM_OUTPUTS][10] = { "UP", "DOWN", "LEFT", "RIGHT", "STOP",
        "GO", "YES", "NO", "ON", "OFF", "ONE", "TWO", "THREE", "FOUR", "FIVE",
        "SIX", "SEVEN", "EIGHT", "NINE", "ZERO", "Unknown" };

void i2s_isr(void)
{
    i2s_flag = 1;
    /* Clear I2S interrupt flag */
    MXC_I2S_ClearFlags(MXC_F_I2S_INTFL_RX_THD_CH0);
}

/* **** Functions Prototypes **** */
int8_t MicReader(int16_t *sample);
uint8_t MicReadChunk(uint8_t *pBuff, uint16_t * avg);
uint8_t AddTranspose(uint8_t *pIn, uint8_t *pOut, uint16_t inSize,
        uint16_t outSize, uint16_t width);
uint8_t check_inference(q15_t *ml_soft, int32_t *ml_data,
        int16_t *out_class, double *out_prob);
void I2SInit();
void HPF_init(void);
int16_t HPF(int16_t input);
static void send_result_to_me14(char *result, uint32_t len);


/* **************************************************************************** */
void DMA_CHANNEL_QSPI_IRQ_HAND(void)
{
    qspi_dma_slave_int_handler(QSPI_ID, DMA_CHANNEL_QSPI);
}

int main(void)
{
    uint32_t sampleCounter = 0;
    mxc_tmr_unit_t units;

    uint8_t pChunkBuff[CHUNK];

    uint16_t avg = 0;
    uint16_t preambleCounter = 0;
    uint16_t ai85Counter = 0;
    uint16_t wordCounter = 0;

    uint16_t avgSilenceCounter = 0;

    mic_processing_state procState = STOP;

    /* Enable cache */
    MXC_ICC_Enable(MXC_ICC0);

    /* Switch to 100 MHz clock */
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    SystemCoreClockUpdate();

    /* Reset all domains, restore power to CNN */
    MXC_BBFC->reg3 = 0xf; // Reset
    MXC_BBFC->reg1 = 0xf; // Mask
    MXC_BBFC->reg0 = 0xf; // Power
    MXC_BBFC->reg2 = 0x0; // Iso
    MXC_BBFC->reg3 = 0x0; // Reset

    MXC_GCR->pclkdiv &= ~(MXC_F_GCR_PCLKDIV_CNNCLKDIV
            | MXC_F_GCR_PCLKDIV_CNNCLKSEL);
    MXC_GCR->pclkdiv |= MXC_S_GCR_PCLKDIV_CNNCLKDIV_DIV1; // CNN clock: 100 MHz div 2
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_CNN); // Enable CNN clock

    qspi_int.port = MXC_GPIO0;
    qspi_int.mask = MXC_GPIO_PIN_12;
    qspi_int.pad = MXC_GPIO_PAD_NONE;
    qspi_int.func = MXC_GPIO_FUNC_OUT;
    GPIO_SET(qspi_int);
    MXC_GPIO_Config(&qspi_int);

    /* Configure P2.5, turn on the CNN Boost */
    mxc_gpio_cfg_t gpio_cnn_boost;
    gpio_cnn_boost.port = MXC_GPIO2;
    gpio_cnn_boost.mask = MXC_GPIO_PIN_5;
    gpio_cnn_boost.pad = MXC_GPIO_PAD_NONE;
    gpio_cnn_boost.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_cnn_boost);
    MXC_GPIO_OutSet(gpio_cnn_boost.port, gpio_cnn_boost.mask);

    /* Configure P0.31, turn on the audio OSC */
    mxc_gpio_cfg_t gpio_audio_osc;
    gpio_audio_osc.port = MXC_GPIO0;
    gpio_audio_osc.mask = MXC_GPIO_PIN_31;
    gpio_audio_osc.pad = MXC_GPIO_PAD_NONE;
    gpio_audio_osc.func = MXC_GPIO_FUNC_OUT;
    MXC_GPIO_Config(&gpio_audio_osc);
    MXC_GPIO_OutSet(gpio_audio_osc.port, gpio_audio_osc.mask);

    PR_INFO("maxcam_electronica_max78000_audio v%d.%d.%d", S_VERSION_MAJOR, S_VERSION_MINOR, S_VERSION_BUILD);

    memset(pAI85Buffer, 0x0, sizeof(pAI85Buffer));
    memset(pPreambleCircBuffer, 0x0, sizeof(pPreambleCircBuffer));

    PR_DEBUG("pChunkBuff: %d", sizeof(pChunkBuff));
    PR_DEBUG("pPreambleCircBuffer: %d", sizeof(pPreambleCircBuffer));
    PR_DEBUG("pAI85Buffer: %d", sizeof(pAI85Buffer));

    mxc_spi_pins_t qspi_pins;
    qspi_pins.clock = TRUE;
    qspi_pins.miso = TRUE;
    qspi_pins.mosi = TRUE;
    qspi_pins.sdio2 = TRUE;
    qspi_pins.sdio3 = TRUE;
    qspi_pins.ss0 = TRUE;
    qspi_pins.ss1 = FALSE;
    qspi_pins.ss2 = FALSE;

    qspi_dma_slave_init(QSPI_ID, qspi_pins);

    if (MXC_DMA_Init() != E_NO_ERROR) {
        PR_ERROR("DMA INIT ERROR");
    }

    NVIC_EnableIRQ(DMA_CHANNEL_QSPI_IRQ);

    /* load kernels */
    PR_INFO("*** CNN Kernel load ***");
    if (!cnn_load_kernel()) {
        PR_ERROR("ERROR: Loading Kernel!");
        fail();
    }
    /* switch to silence state*/
    procState = SILENCE;

    /* initialize I2S interface to Mic */
    I2SInit();

    PR_INFO("** READY ***");

    /* Read samples */
    while (1) {
        /* Read from Mic driver to get CHUNK worth of samples, otherwise next sample*/
        if (MicReadChunk(pChunkBuff, &avg) == 0) {
            continue;
        }

        sampleCounter += CHUNK;

#ifdef ENABLE_SILENCE_DETECTION       // disable to start collecting data immediately.

        /* copy the preamble data*/
        /* add the new chunk to the end of circular buffer*/
        memcpy(&pPreambleCircBuffer[preambleCounter], pChunkBuff,
                sizeof(uint8_t) * CHUNK);

        /* increment circular buffer pointer*/
        preambleCounter = (preambleCounter + CHUNK) % (PREAMBLE_SIZE);

        /* Display average envelope as a bar */
#ifdef ENABLE_PRINT_ENVELOPE
        printf("%.6d|",sampleCounter);
        for (int i = 0; i < avg / 10; i++)
            printf("=");
        if (avg >= thresholdHigh)
            printf("*");
        printf("[%d]\n",avg);
#endif

        /* if we have not detected voice, check the average*/
        if (procState == SILENCE) {

            /* compute average, proceed if greater than threshold */
            if (avg >= thresholdHigh) {
                /* switch to keyword data collection*/
                procState = KEYWORD;
                PR_DEBUG("%.6d Word starts from index: %d, avg:%d > %d",
                        sampleCounter, sampleCounter - PREAMBLE_SIZE - CHUNK,
                        avg, thresholdHigh);

                /* reorder circular buffer according to time at the beginning of pAI85Buffer */
                if (preambleCounter == 0) {
                    /* copy latest samples afterwards */
                    if (AddTranspose(&pPreambleCircBuffer[0], pAI85Buffer,
                            PREAMBLE_SIZE, SAMPLE_SIZE, TRANSPOSE_WIDTH))
                        PR_ERROR("ERROR: Transpose ended early");
                } else {
                    /* copy oldest samples to the beginning*/
                    if (AddTranspose(&pPreambleCircBuffer[preambleCounter],
                            pAI85Buffer, PREAMBLE_SIZE - preambleCounter,
                            SAMPLE_SIZE, TRANSPOSE_WIDTH))
                        PR_ERROR("ERROR: Transpose ended early");

                    /* copy latest samples afterwards */
                    if (AddTranspose(&pPreambleCircBuffer[0], pAI85Buffer,
                            preambleCounter, SAMPLE_SIZE, TRANSPOSE_WIDTH))
                        PR_ERROR("ERROR: Transpose ended early");

                }

                /* preamble is copied and state is changed, start adding keyword samples next run */
                ai85Counter += PREAMBLE_SIZE;
                continue;

            }
        }
        /* if it is in data collection, add samples to buffer*/
        else if (procState == KEYWORD)
#endif  //#ifdef ENABLE_SILENCE_DETECTION

        {
            uint8_t ret=0;

            /* add sample, rearrange buffer */
            ret = AddTranspose(pChunkBuff, pAI85Buffer, CHUNK, SAMPLE_SIZE,
                    TRANSPOSE_WIDTH);

            /* increment number of stored samples */
            ai85Counter += CHUNK;

            /* if there is silence after at least 1/3 of samples passed, increment number of times back to back silence to find end of keyword */
            if ((avg < thresholdLow) && (ai85Counter >= SAMPLE_SIZE / 3)) {
                avgSilenceCounter ++;
            } else
                avgSilenceCounter = 0;


            /* if this is the last sample and there are not enough samples to
             * feed to CNN, or if it is long silence after keyword,  append with zero (for reading file)
             */
            if (avgSilenceCounter > SILENCE_COUNTER_THRESHOLD)
            {
                memset(pChunkBuff,0,CHUNK);
                PR_DEBUG("%.6d: Word ends, Appends %d zeros", sampleCounter,
                        SAMPLE_SIZE - ai85Counter);
                ret = 0;
                while (!ret) {
                    ret = AddTranspose(pChunkBuff, pAI85Buffer, CHUNK,
                            SAMPLE_SIZE, TRANSPOSE_WIDTH);
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

                /* change state to silence */
                procState = SILENCE;

                /* sanity check, last transpose should have returned 1, as enough samples should have already been added */
                if (ret != 1) {
                    PR_ERROR("ERROR: Transpose incomplete!");
                    fail();
                }

                //----------------------------------  : invoke AI85 CNN
                PR_DEBUG("%.6d: Starts CNN: %d", sampleCounter, wordCounter);

                /* load to CNN */
                if (!cnn_load_data(pAI85Buffer)) {
                    PR_ERROR("ERROR: Loading data to CNN!");
                    fail();
                }

                /* Start timer */
                MXC_TMR_SW_Start(MXC_TMR0);

                /* Start CNN */
                if (!cnn_start()) {
                    PR_ERROR("ERROR: Starting CNN!");
                    fail();
                }

                /* Wait for CNN  to complete */
                cnn_wait();

                /* read data */
                cnn_unload((uint32_t *)ml_data);

                /* Get time */
                MXC_TMR_GetTime(MXC_TMR0, cnn_time, &cnn_time, &units);
                PR_DEBUG("%.6d: Completes CNN: %d", sampleCounter, wordCounter);

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
                PR_DEBUG("CNN Time: %d us", cnn_time);

                /* run softmax */
                softmax_q17p14_q15((const q31_t*) ml_data, NUM_OUTPUTS,
                        ml_softmax);

#ifdef ENABLE_CLASSIFICATION_DISPLAY
                PR_INFO("Classification results:");
                for (int i = 0; i < NUM_OUTPUTS; i++) {
                    int digs = (1000 * ml_softmax[i] + 0x4000) >> 15;
                    int tens = digs % 10;
                    digs = digs/10;

                    printf("[%+.7d] -> Class %.2d %8s: %d.%d%%\n", ml_data[i],
                            i, keywords[i], digs, tens);
                }
#endif
                /* find detected class with max probability */
                ret = check_inference(ml_softmax, ml_data, &out_class, &probability);

                if (!ret)
                    PR_WARN("LOW CONFIDENCE!: ");

                PR_INFO("Detected word: %s (%0.1f%%)", keywords[out_class],
                        probability);

                if (ret) {
                    send_result_to_me14((char *)keywords[out_class], sizeof(keywords[out_class]));
                }

                printf("\n----------------------------------------- \n");

                Max = 0;
                Min = 0;
                //------------------------------------------------------------
            }
        }
    }

    /* Turn off LED2 (Red) */
    PR_INFO("Total Samples:%d, Total Words: %d", sampleCounter, wordCounter);

    while (1);
}

/* **************************************************************************** */

void I2SInit()
{
    mxc_i2s_req_t req;
    int32_t err;

    PR_INFO("*** I2S & Mic Init ***");
    /* Initialize High Pass Filter */
    HPF_init();
    /* Initialize I2S RX buffer */
    memset(i2s_rx_buffer, 0, sizeof(i2s_rx_buffer));
    /* Configure I2S interface parameters */
    req.wordSize    = MXC_I2S_DATASIZE_WORD;
    req.sampleSize  = MXC_I2S_SAMPLESIZE_THIRTYTWO;
    req.justify     = MXC_I2S_MSB_JUSTIFY;
    req.wsPolarity	= MXC_I2S_POL_NORMAL;
    req.channelMode	= MXC_I2S_INTERNAL_SCK_WS_0;
    /* Get only left channel data from on-board microphone. Right channel samples are zeros */
    req.stereoMode	= MXC_I2S_MONO_LEFT_CH;
    req.bitOrder	= MXC_I2S_MSB_FIRST;
    /* I2S clock = PT freq / (2*(req.clkdiv + 1)) */
    /* I2S sample rate = I2S clock/64 = 16kHz */
    req.clkdiv      = 5;
    req.rawData     = NULL;
    req.txData      = NULL;
    req.rxData      = i2s_rx_buffer;
    req.length      = I2S_RX_BUFFER_SIZE;


    if((err = MXC_I2S_Init(&req)) != E_NO_ERROR) {
        PR_ERROR("\nError in I2S_Init: %d", err);
        while (1);
    }

    /* Set I2S RX FIFO threshold to generate interrupt */
    MXC_I2S_SetRXThreshold(4);
    NVIC_SetVector(I2S_IRQn, i2s_isr);
    NVIC_EnableIRQ(I2S_IRQn);
    /* Enable RX FIFO Threshold Interrupt */
    MXC_I2S_EnableInt(MXC_F_I2S_INTEN_RX_THD_CH0);
    MXC_I2S_RXEnable();
    __enable_irq();
}

/* **************************************************************************** */
uint8_t check_inference(q15_t *ml_soft, int32_t *ml_data,
        int16_t *out_class, double *out_prob) {
    int32_t temp[NUM_OUTPUTS];
    q15_t max = 0;    // soft_max output is 0->32767
    int32_t max_ml = 1<<31;  // ml before going to soft_max
    int16_t max_index = -1;

    memcpy(temp, ml_data, sizeof(int32_t) * NUM_OUTPUTS);

    /* find the top 5 classes */
    for (int top = 0; top < 5; top++) {
        /* find the class with highest */
        for (int i = 0; i < NUM_OUTPUTS; i++) {
            if ((int32_t) temp[i] > max_ml) {
                max_ml = (int32_t) temp[i];

                max = ml_soft[i];
                max_index = i;
            }
        }

        /* print top 1 separately */
        if (top == 0) {

            *out_class = max_index;
            *out_prob = 100.0 * max / 32768.0;
            break;
        }
    }

    PR_DEBUG("Min: %d,   Max: %d", Min, Max);

    /* check if probability is low */
    if (*out_prob > INFERENCE_THRESHOLD)
        return 1;
    else
        return 0;
}

/* **************************************************************************** */

uint8_t AddTranspose(uint8_t *pIn, uint8_t *pOut, uint16_t inSize,
        uint16_t outSize, uint16_t width) {
    /* Data order in Ai85 memory (transpose is included):
	input(series of 8 bit samples): (0,0) ...  (0,127)  (1,0) ... (1,127) ...... (127,0)...(127,127)    16384 samples
	output (32bit word): 16K samples in a buffer. Later, each 1K goes to a seperate CNN memory group
	0x0000:
		(0,3)(0,2)(0,1)(0,0)
		(0,67)(0,66)(0,65)(0,64)
		(1,3)(1,2)(1,1)(1,0)
		(1,67)(1,66)(1,65)(1,64)
		....
		(127,67)(127,66)(127,65)(127,64)
	0x0400:
		(0,7)(0,6)(0,5)(0,4)
		(0,71)(0,70)(0,69)(0,68)
		....
		(127,71)(127,70)(127,69)(127,68)
	...
	0x3C00:
		(0,63)(0,62)(0,61)(0,60)
		(0,127)(0,126)(0,125)(0,124)
		....
		(127,127)(127,126)(127,125)(127,124)
     */

    static uint16_t row = 0, col = 0, total = 0;
    uint16_t  secondHalf = 0, wordRow = 0, byteInWord = 0, group = 0, index = 0;
    for (int i = 0; i < inSize; i++) {
        /* is it above 63? */
        if (col >= (width >> 1)) {
            secondHalf = 1; // odd word rows
        } else {
            secondHalf = 0; // even word rows
        }

        /* which group (0-15) it should be */
        group = (col % (width >> 1)) / 4;

        /* which word row (0-255) within the group */
        wordRow = secondHalf + (row << 1);

        /* which byte (0-3) in the word */
        byteInWord = col % 4;

        /* find output index */
        index = 1024 * group + 4 * wordRow + byteInWord;

        /* place sample in correct output location */
        pOut[index] = pIn[i];

        total++;

        /* increment row and col index */
        col++;
        if (col >= width) {
            col = 0;
            row++;
        }
    }

    if (total >= outSize) {
        /* sanity check */
        if (row != width)
            PR_ERROR("ERROR: Rearranging!");

        total = 0;
        row = 0;
        col = 0;
        return 1;
    } else
        return 0;

}
/* **************************************************************************** */
uint8_t MicReadChunk(uint8_t *pBuff, uint16_t * avg)
{
    static uint16_t chunkCount = 0;
    static uint16_t sum = 0;

    static uint32_t index = 0;

    int32_t sample = 0;
    int16_t temp = 0;
    uint32_t rx_size = 0;

    /* sample not ready */
    if (!i2s_flag) {
        *avg = 0;
        return 0;
    }

    /* Clear flag */
    i2s_flag = 0;
    /* Read number of samples in I2S RX FIFO */
    rx_size = MXC_I2S->dmach0 >> MXC_F_I2S_DMACH0_RX_LVL_POS;
    //	printf("%d ", rx_size);

    /* read until fifo is empty or enough samples are collected */
    while ((rx_size--) && (chunkCount < CHUNK)) {
        /* Read microphone sample from I2S FIFO */
        sample = (int32_t)MXC_I2S->fifoch0;
        /* The actual value is 18 MSB of 32-bit word */
        temp = sample >> 14;

        /* Remove DC from microphone signal */
        sample = HPF((int16_t)temp); // filter needs about 1K sample to converge

        /* Discard first 10k samples due to microphone charging cap effect */
        if (index++ < 10000)
            continue;

        /* Turn on LED2 (Red) */
        //		LED_On(LED2);

        /* absolute for averaging */
        if (sample >= 0)
            sum += sample;
        else
            sum -= sample;

        /* Convert to 8 bit unsigned */
        pBuff[chunkCount] = (uint8_t)((sample)*SAMPLE_SCALE_FACTOR/256);

        temp=(int8_t)pBuff[chunkCount];

        chunkCount++;

        /* record max and min */
        if (temp > Max)
            Max = temp;
        if (temp < Min)
            Min = temp;

    }

    /* if not enough samples, return 0 */
    if (chunkCount < CHUNK) {
        *avg = 0;
        return 0;
    }

    /* enough samples are collected, calculate average and return 1 */
    *avg = ((uint16_t)(sum / CHUNK));

    chunkCount = 0;
    sum = 0;
    return 1;
}

static int16_t	x0, x1, Coeff;
static int32_t	y0, y1;

/************************************************************************************/
void HPF_init(void) {
    Coeff = 32604; //0.995
    x0 = 0;
    y0 = 0;
    y1 = y0;
    x1 = x0;
}

/************************************************************************************/
int16_t HPF(int16_t input) {
    int16_t Acc, output;
    int32_t tmp;

    /* a 1st order IIR high pass filter (100 Hz cutoff frequency)  */
    /* y(n)=x(n)-x(n-1)+A*y(n-1) and A =.995*2^15 */

    x0=input;

    tmp = (Coeff * y1);
    Acc = (int16_t)((tmp + (1 << 14)) >> 15);
    y0 = x0 - x1 + Acc;

    /* Clipping */
    if(y0 > 32767)
        y0 = 32767;
    if(y0 < -32768)
        y0 = -32768;

    /* Update filter state */
    y1 = y0;
    x1 = x0;

    output = (int16_t)y0;

    return	(output);
}

/************************************************************************************/
static void send_result_to_me14(char *result, uint32_t len)
{
    qspi_header_t header;
    header.start_symbol = QSPI_START_SYMBOL;
    header.data_len = len;
    header.data_type = QSPI_TYPE_RESPONSE_AUDIO_RESULT;

    PR_INFO("result tx start");

    qspi_dma_slave_tx(QSPI_ID, DMA_CHANNEL_QSPI, (uint8_t*) &header, sizeof(qspi_header_t), &qspi_int);
    qspi_dma_slave_tx_wait();

    qspi_dma_slave_tx(QSPI_ID, DMA_CHANNEL_QSPI, (uint8_t *)result, len, &qspi_int);
    qspi_dma_slave_tx_wait();

    PR_INFO("result tx completed");
}
