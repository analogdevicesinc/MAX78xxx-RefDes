#ifndef AUDIO_PROCESSOR_H_
#define AUDIO_PROCESSOR_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stddef.h>

#include "gpio.h"
#include "sdhc_lib.h"
#include "ff.h"

#include "maxrefdes178_definitions.h"
#include "max32666_qspi_master.h"
#include "max32666_pmic.h"

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define MAXLEN 			256
#define FORMAT_HEADER 	0x20746D66
#define FORMAT_SIZE		5
#define DATA_HEADER		0x61746164
#define DATA_MAXLEN 	5000
#define DATA_FILL_SZ	(DATA_MAXLEN * sizeof(uint16_t))
#define NUM_FILES		4
#define AUDIO_STATS_SZ	6

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct {
	uint8_t fileNo;
	uint8_t playing;
	uint16_t numChannels;
	uint16_t bitsPerSample;
	uint32_t sampleRate;
	uint32_t dataSize;
	uint16_t* buffer;
	uint16_t bufferSize;
} WavFile_t;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern TCHAR *FF_ERRORS[20];
extern TCHAR* myFiles[NUM_FILES];

//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
/* Initialize SDHC module */
int initSDHC(void);

/* Close open .wav file */
int closeFile(WavFile_t* wavFile);

/* Send audio recording parameters */
int sendAudioStats(WavFile_t* wavFile);

/* Send audio samples */
int sendAudioData(WavFile_t* wavFile);

/* Unmount SD Card */
int umount(void);

#endif /* AUDIO_PROCESSOR_H_ */