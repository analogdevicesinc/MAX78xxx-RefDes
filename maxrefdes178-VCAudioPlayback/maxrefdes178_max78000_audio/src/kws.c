#include "kws.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static int32_t i2sRXBuf[I2S_RX_BUFFER_SIZE];
static int16_t  x0, x1, Coeff;
static int32_t  y0, y1;
int16_t Max, Min;

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void KWS_i2sInit(void)
{
    mxc_i2s_req_t req;
    int32_t err;

    /* Initialize High Pass Filter */
    HPF_init();
    /* Initialize I2S RX buffer */
    memset(i2sRXBuf, 0, sizeof(i2sRXBuf));
    /* Configure I2S interface parameters */
    req.wordSize    = MXC_I2S_DATASIZE_WORD;
    req.sampleSize  = MXC_I2S_SAMPLESIZE_THIRTYTWO;
    req.justify     = MXC_I2S_MSB_JUSTIFY;
    req.wsPolarity  = MXC_I2S_POL_NORMAL;
    req.channelMode = MXC_I2S_INTERNAL_SCK_WS_0;
    /* Get only left channel data from on-board microphone. Right channel samples are zeros */
    req.stereoMode  = MXC_I2S_MONO_LEFT_CH;
    req.bitOrder    = MXC_I2S_MSB_FIRST;
    /* I2S clock = PT freq / (2*(req.clkdiv + 1)) */
    /* I2S sample rate = I2S clock/64 = 16kHz */
    req.clkdiv      = 5;
    req.rawData     = NULL;
    req.txData      = NULL;
    req.rxData      = i2sRXBuf;
    req.length      = I2S_RX_BUFFER_SIZE;


    if((err = MXC_I2S_Init(&req)) != E_NO_ERROR) {
        fail();
    }

    /* Set I2S RX FIFO threshold to generate interrupt */
    MXC_I2S_SetRXThreshold(4);
    MXC_NVIC_SetVector(I2S_IRQn, i2s_isr);
    NVIC_EnableIRQ(I2S_IRQn);
    /* Enable RX FIFO Threshold Interrupt */
    MXC_I2S_EnableInt(MXC_F_I2S_INTEN_RX_THD_CH0);
    MXC_I2S_RXEnable();
    __enable_irq();
}

uint8_t check_inference(q15_t *ml_soft, int32_t *ml_data, int16_t *out_class, double *out_prob) {
    int32_t temp[NUM_OUTPUTS];
    q15_t max = 0;    // soft_max output is 0->32767
    int32_t max_ml = 1 << 31;  // ml before going to soft_max
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

    /* check if probability is low */
    if (*out_prob > INFERENCE_THRESHOLD) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t cnn_load_data(uint8_t* pIn)
{
    uint32_t mem;
    uint16_t index = 0;

    /* data should already be formated correctly */
    /* pIn is 16KB, each 1KB belongs to a memory group */
    for (mem = 0x50400000; mem <= 0x50418000; mem += 0x8000) {
        memcpy((uint8_t*)mem, &pIn[index], 1024);
        //printf("%.10X \n",(uint8_t *)mem);
        index += 1024;
    }

    for (mem = 0x50800000; mem <= 0x50818000; mem += 0x8000) {
        memcpy((uint8_t*)mem, &pIn[index], 1024);
        index += 1024;
    }

    for (mem = 0x50C00000; mem <= 0x50C18000; mem += 0x8000) {
        memcpy((uint8_t*)mem, &pIn[index], 1024);
        index += 1024;
    }

    for (mem = 0x51000000; mem <= 0x51018000; mem += 0x8000) {
        memcpy((uint8_t*)mem, &pIn[index], 1024);
        index += 1024;
    }

    return CNN_OK;
}

uint8_t AddTranspose(uint8_t *pIn, uint8_t *pOut, uint16_t inSize, uint16_t outSize, uint16_t width) {
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
        total = 0;
        row = 0;
        col = 0;
        return 1;
    } else {
        return 0;
    }
}

uint8_t MicReadChunk(uint8_t *pBuff, uint16_t * avg)
{
    static uint16_t chunkCount = 0;
    static uint16_t sum = 0;

    static uint32_t index = 0;

    int32_t sample = 0;
    int16_t temp = 0;
    uint32_t rx_size = 0;

    /* sample not ready */
    if (!i2sRXFlag) {
        *avg = 0;
        return 0;
    }

    /* Clear flag */
    i2sRXFlag = 0;
    /* Read number of samples in I2S RX FIFO */
    rx_size = MXC_I2S->dmach0 >> MXC_F_I2S_DMACH0_RX_LVL_POS;
    //  printf("%d ", rx_size);

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
        if (temp > Max) {
            Max = temp;
        }

        if (temp < Min) {
            Min = temp;
        }

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

void HPF_init(void) {
    Coeff = 32604; //0.995
    x0 = 0;
    y0 = 0;
    y1 = y0;
    x1 = x0;
}

int16_t HPF(int16_t input) {
    int16_t Acc, output;
    int32_t tmp;

    /* a 1st order IIR high pass filter (100 Hz cutoff frequency)  */
    /* y(n)=x(n)-x(n-1)+A*y(n-1) and A =.995*2^15 */

    x0 = input;

    tmp = (Coeff * y1);
    Acc = (int16_t)((tmp + (1 << 14)) >> 15);
    y0 = x0 - x1 + Acc;

    /* Clipping */
    if (y0 > 32767) {
        y0 = 32767;
    }

    if (y0 < -32768) {
        y0 = -32768;
    }

    /* Update filter state */
    y1 = y0;
    x1 = x0;

    output = (int16_t)y0;

    return (output);
}

void fail(void)
{
    LED_On(0);
    LED_On(2);
    LED_On(1);
    while(1);
}
