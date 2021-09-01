#include "audio_processor.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
FATFS *fs;		//FFat Filesystem Object
FATFS fs_obj;
FRESULT err;	//FFat Result (Struct)
DIR dir;		//FFat Directory Object
FILINFO fno;	//FFat File Information Object
TCHAR *FF_ERRORS[20];
TCHAR cwd[MAXLEN];
FIL file;		//FFat File Object

UINT bytes_written = 0;
UINT bytes_read = 0;
UINT mounted = 0;
uint16_t dataBuf[DATA_MAXLEN];
TCHAR* myFiles[NUM_FILES];

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
static int mount() {
    fs = &fs_obj;
    if((err = f_mount(fs, "", 1)) != FR_OK) {			//Mount the default drive to fs now
        printf("Error opening SD card: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
    }
    else {
        printf("SD card mounted.\n");
        mounted = 1;
    }
    
    f_getcwd(cwd, sizeof(cwd));							//Set the Current working directory
    
    return err;
}

static int getAudioInfo(WavFile_t* wavFile) {
	uint32_t fileData[FORMAT_SIZE];

	// Look for format section
	do {
		if((err = f_read(&file, fileData, 4, &bytes_read)) != FR_OK){
			printf("Error reading file: %s\n", FF_ERRORS[err]);
			f_mount(NULL, "", 0);
			return err;
		}
	} while(fileData[0] != FORMAT_HEADER);

	//parse format section
	if((err = f_read(&file, fileData, FORMAT_SIZE*4, &bytes_read)) != FR_OK){
		printf("Error reading file: %s\n", FF_ERRORS[err]);
		f_mount(NULL, "", 0);
		return err;
	}
	wavFile->numChannels = ((fileData[1] & 0xFFFF0000) >> 16);
	wavFile->sampleRate = fileData[2];
	wavFile->bitsPerSample = ((fileData[4] & 0xFFFF0000) >> 16);

	// Look for data section
	do {
		if((err = f_read(&file, fileData, 4, &bytes_read)) != FR_OK){
			printf("Error reading file: %s\n", FF_ERRORS[err]);
			f_mount(NULL, "", 0);
			return err;
		}
	} while(fileData[0] != DATA_HEADER);

	// Parse data section
	if((err = f_read(&file, fileData, 4, &bytes_read)) != FR_OK){
		printf("Error reading file: %s\n", FF_ERRORS[err]);
		f_mount(NULL, "", 0);
		return err;
	}
	wavFile->dataSize = fileData[0];

	return E_NO_ERROR;
}

int initSDHC(void) {
    mxc_sdhc_cfg_t cfg;

    // Enable Power To Card
    mxc_gpio_cfg_t SDPowerEnablePin = {MXC_GPIO1, MXC_GPIO_PIN_11, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO};
    MXC_GPIO_Config(&SDPowerEnablePin);
    MXC_GPIO_OutClr(MXC_GPIO1, SDPowerEnablePin.mask);

    // Initialize SDHC peripheral
    cfg.bus_voltage = MXC_SDHC_Bus_Voltage_3_3;
    cfg.block_gap = 0;
    cfg.clk_div = 0x0b0; // Maximum divide ratio, frequency must be >= 400 kHz during Card Identification phase
    if(MXC_SDHC_Init(&cfg) != E_NO_ERROR) {
        printf("Unable to initialize SDHC driver.\n");
        return 1;
    }

    // wait for card to be inserted
    while (!MXC_SDHC_Card_Inserted());
    printf("Card inserted.\n");
    
    // set up card to get it ready for a transaction
    if (MXC_SDHC_Lib_InitCard(10) == E_NO_ERROR) {
      printf("Card Initialized.\n");
    } else {
      printf("No card response! Remove card, reset EvKit, and try again.\n");

      return -1;
    }
    
    if (MXC_SDHC_Lib_Get_Card_Type() == CARD_SDHC) {
      printf("Card type: SDHC\n");
    } else {
      printf("Card type: MMC/eMMC\n");
    }
    
    /* Configure for fastest possible clock, must not exceed 52 MHz for eMMC */
    if (SystemCoreClock >= 96000000)  {
      printf("SD clock ratio (at card) 4:1\n");
      MXC_SDHC_Set_Clock_Config(1);
    } else {
      printf("SD clock ratio (at card) 2:1\n");
      MXC_SDHC_Set_Clock_Config(0);
    }

    return E_NO_ERROR;
}

int closeFile(WavFile_t* wavFile) {
	pmic_led_green(0);
	pmic_led_red(1);

    if((err = f_close(&file)) != FR_OK){
        printf("Error closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    wavFile->playing = 0;

    return err;
}

int sendAudioStats(WavFile_t* wavFile)
{
    if(!mounted) {
        mount();
    }

    // Open .wav file
    if((err = f_open(&file, myFiles[wavFile->fileNo], FA_READ)) != FR_OK){
        printf("Error opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

	pmic_led_green(1);
	pmic_led_red(0);

    // Get Audio Format Information
    getAudioInfo(wavFile);
    wavFile->buffer = dataBuf;
    wavFile->bufferSize = 0;
    wavFile->playing = 1;

    wavFile->buffer[0] = wavFile->numChannels;
    wavFile->buffer[1] = wavFile->bitsPerSample;
    wavFile->buffer[2] = wavFile->sampleRate;

    if((err = qspi_master_send_audio((uint8_t*) wavFile->buffer, AUDIO_STATS_SZ, QSPI_PACKET_TYPE_AUDIO_STATS)) != E_NO_ERROR) {
    	printf("Error sending audio file information.\n");
    	closeFile(wavFile);
    	return err;
    }

    return err;
}

int sendAudioData(WavFile_t* wavFile) {
	int err;
	if(!wavFile->playing) {
		return E_BAD_STATE;
	}
	else if(wavFile->buffer == NULL) {
		return E_NULL_PTR;
	}

	// Read audio samples from SD card
	wavFile->bufferSize = 0;
	if(wavFile->dataSize > DATA_FILL_SZ) {
		err = f_read(&file, wavFile->buffer, DATA_FILL_SZ, (UINT*) &wavFile->bufferSize);
	}
	else {
		err = f_read(&file, wavFile->buffer, wavFile->dataSize, (UINT*) &wavFile->bufferSize);
	}

	// Check read was successful
	if(err != FR_OK){
		printf("Error reading file: %s\n", FF_ERRORS[err]);
		closeFile(wavFile);
	}
	else {
		// Update remaining bytes in audio file
		wavFile->dataSize -= wavFile->bufferSize;
		if(wavFile->dataSize <= 0) {
			closeFile(wavFile);
		}
	}

	// Send bytes read to audio AI85
    if((err = qspi_master_send_audio((uint8_t*) wavFile->buffer, wavFile->bufferSize, QSPI_PACKET_TYPE_AUDIO_DATA)) != E_NO_ERROR) {
    	printf("Error sending audio samples.\n");
    	closeFile(wavFile);
    	return err;
    }

    return err;
}

int umount() {
    if((err = f_mount(NULL, "", 0)) != FR_OK){			//Unmount the default drive from its mount point
        printf("Error unmounting volume: %s\n", FF_ERRORS[err]);
    }
    else {
        printf("SD card unmounted.\n");
        mounted = 0;
    }
    
    return err;
}