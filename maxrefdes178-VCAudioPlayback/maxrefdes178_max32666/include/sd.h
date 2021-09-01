#ifndef SD_H_
#define SD_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


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

#endif /* SD_H_ */