#ifndef AUDIO_H_
#define AUDIO_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "i2c.h"
#include "i2s.h"
#include "mxc_delay.h"
#include "nvic_table.h"

#include "maxrefdes178_definitions.h"
#include "max78000_qspi_slave.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define EXT_CLK_FREQ				12288000
#define NUM_DATA_BUF				2
#define I2S_CLKDIV(fsamp, width)   	(EXT_CLK_FREQ / (4 * fsamp * width))
#define I2S_BUF_DEPTH				8
#define I2S_TX_BUFFER_SIZE			5000

// Audio Codec
#define MAX9867_I2C					MXC_I2C1
#define MAX9867_SLV_ADDR			0x18
#define MAX9867_FREQ				100000
#define MAX9867_REV					0x42
#define MAX9867_REV_REG				0xFF
#define MAX8967_REG_START_ADDR		0x04
#define MAX9867_PWR_REG				0x17
#define MAX9867_SHUTDOWN			0x00
#define MAX9867_DAC_EN				0x8C
#define MAX9867_INT_DIS				0x00
#define MAX9867_MCLK_DIV1			0x10
#define MAX9867_PLL_EN				0x80
#define MAX9867_NI_VAL(fsamp)	((65536 * 96 * fsamp) / EXT_CLK_FREQ)
#define MAX9867_I2S_MASTER			0x10
#define	MAX9867_MONO_DIS			0x00
#define MAX9867_FILT_DIS			0x00
#define MAX9867_ST_DIS				0x00
#define MAX9867_DAC_GAIN_DIS		0x00
#define MAX9867_ADC_LVL0			0x33
#define MAX9867_LI_MUTE				0x4F
#define MAX9867_VOL_LOW				0x19
#define MAX9867_MIC_GAIN_DIS		0x14
#define MAX9867_AUX_DIS				0x00
#define	MAX9867_DIG_MIC_DIS			0x00
#define MAX9867_SE_STEREO			0x06

// Random
#define MSB_MASK					0xFF00
#define LSB_MASK					0xFF
#define BYTE_SHIFT					8

//-----------------------------------------------------------------------------
// Type declarations
//-----------------------------------------------------------------------------
typedef struct {
	uint16_t numChannels;
	uint16_t bitsPerSample;
	uint32_t sampleRate;
} WavFile_t;

typedef struct {
	int numEntries;
	int index;
	int lastTX;
	uint16_t dataBuf[I2S_TX_BUFFER_SIZE];
} I2SBuf_t;

typedef enum {
	NO_CMD,
	GO,
	LEFT,
	RIGHT
} AudioCmd_t;

typedef enum {
    STOP = 0,     /* No processing  */
    SILENCE = 1,  /* Threshold not detected yet  */
    KEYWORD = 2   /* Threshold has been detected, gathering keyword samples */
} mic_processing_state_t;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern volatile uint8_t i2sRXFlag;
extern I2SBuf_t i2sTXBuf[NUM_DATA_BUF];
extern int bufSelect;
extern int refill;
extern volatile uint8_t playing;

//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
/* Initialize MAX9867 Audio Codec */
int max9867_init(WavFile_t* fileInfo);

/* Shutdown MAX9867 Audio Codec */						
int max9867_shutdown(void);

/* I2S Interrupt Service Routine */
void i2s_isr(void);

/* Get parameters for playback of audio data */
int getAudioFileStats(WavFile_t* fileInfo, classification_result_t classification_result);

/* Initialize I2S for audio playback through headphone amplifiers */
int codec_i2sInit(WavFile_t* fileInfo);

/* Fill I2S data buffer with Audio data from SD Card */ 
int fillDataBuf(WavFile_t* fileInfo, int bufIndex);

/* Get audio data from MAX32666 */
int getAudioData(uint16_t* startAddr, int* numRead, WavFile_t* fileInfo);

/* Transmit available audio samples to audio codec */
int playAudio(int bufIndex);

/* Stop requesting audio samples from MAX32665 */
void stopAudio(void);

#endif /* AUDIO_H_ */