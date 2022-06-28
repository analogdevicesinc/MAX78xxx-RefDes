#ifndef KWS_H
#define KWS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "max78000_audio_cnn.h"
#include "max78000_tornadocnn.h"
#include "audio.h"
#include "i2s.h"
#include "led.h"
#include "nvic_table.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
/*-----------------------------*/
/* keep following unchanged */
#define SAMPLE_SIZE         16384   // size of input vector for CNN, keep it multiple of 128
#define CHUNK               128     // number of data points to read at a time and average for threshold, keep multiple of 128
#define TRANSPOSE_WIDTH     128     // width of 2d data model to be used for transpose
#define NUM_OUTPUTS         21      // number of classes
#define I2S_RX_BUFFER_SIZE  64      // I2S buffer size
#define TFT_BUFF_SIZE       50      // TFT buffer size
/*-----------------------------*/

/* Adjustables */
#define SAMPLE_SCALE_FACTOR         4       // multiplies 16-bit samples by this scale factor before converting to 8-bit
#define THRESHOLD_HIGH              350     // voice detection threshold to find beginning of a keyword
#define THRESHOLD_LOW               100     // voice detection threshold to find end of a keyword
#define SILENCE_COUNTER_THRESHOLD   20      // [>20] number of back to back CHUNK periods with avg < THRESHOLD_LOW to declare the end of a word
#define PREAMBLE_SIZE               30*CHUNK// how many samples before beginning of a keyword to include
#define INFERENCE_THRESHOLD         90      // min probability (0-100) to accept an inference

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern int16_t Max, Min;

//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
/* Enter infinite loop if any step in the intialization or computation fails */
void fail(void);

/* Load data into the CNN for calculation */
uint8_t cnn_load_data(uint8_t* pIn);

/* Read CHUNK number of samples from microphone */
uint8_t MicReadChunk(uint8_t* pBuff, uint16_t* avg);

/* Re-order Mic samples */
uint8_t AddTranspose(uint8_t* pIn, uint8_t* pOut, uint16_t inSize, uint16_t outSize, uint16_t width);

/* Check probability of CNN result being one of the keywords */
uint8_t check_inference(q15_t* ml_soft, int32_t* ml_data, int16_t* out_class, double* out_prob);

/* Initialize I2S to read samples from the Microphone */
void KWS_i2sInit();

/* Initialize high pass filter */
void HPF_init(void);

/* High pass filter */
int16_t HPF(int16_t input);

#endif /* HWS_H */