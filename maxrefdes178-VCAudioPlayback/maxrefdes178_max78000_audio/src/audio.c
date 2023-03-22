#include "audio.h"

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
volatile uint8_t i2sRXFlag = 0;
I2SBuf_t i2sTXBuf[NUM_DATA_BUF];
int bufSelect = 0;
int refill = 0;
volatile uint8_t playing = 0;

//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
int max9867_init(WavFile_t* fileInfo) {
	uint8_t txBuf[21];
	uint8_t rxBuf[20];
	mxc_i2c_req_t i2cReq;
	uint16_t ni;

	// Init I2C
	MXC_I2C_Init(MAX9867_I2C, 1, 0);
	MXC_I2C_SetFrequency(MAX9867_I2C, 100000);

	// 1: Check revision number
	txBuf[0] = MAX9867_REV_REG;
	i2cReq.i2c = MAX9867_I2C;
	i2cReq.addr = MAX9867_SLV_ADDR;
	i2cReq.tx_buf = txBuf;
	i2cReq.tx_len = 1;
	i2cReq.rx_buf = rxBuf;
	i2cReq.rx_len = 1;
	i2cReq.restart = 0;
	MXC_I2C_MasterTransaction(&i2cReq);

	if(rxBuf[0] != MAX9867_REV) {
		return E_BAD_STATE;
	}

	/* Configure Codec registers */
	ni = MAX9867_NI_VAL(fileInfo->sampleRate);
	txBuf[0] = MAX8967_REG_START_ADDR;	// Start configuring registers from register 0x04
	txBuf[1] = MAX9867_INT_DIS;			// 0x04
	txBuf[2] = MAX9867_MCLK_DIV1;		// 0x05
	txBuf[3] = MAX9867_PLL_EN | ((ni & MSB_MASK) >> BYTE_SHIFT);
	txBuf[4] = ni & LSB_MASK;			// 0x07
	txBuf[5] = MAX9867_I2S_MASTER;		// 0x08
	txBuf[6] = MAX9867_MONO_DIS;		// 0x09
	txBuf[7] = MAX9867_FILT_DIS;		// 0x0A
	txBuf[8] = MAX9867_ST_DIS;			// 0x0B
	txBuf[9] = MAX9867_DAC_GAIN_DIS;	// 0x0C
	txBuf[10] = MAX9867_ADC_LVL0;		// 0x0D
	txBuf[11] = MAX9867_LI_MUTE;		// 0x0E
	txBuf[12] = MAX9867_LI_MUTE;		// 0x0F
	txBuf[13] = MAX9867_VOL_LOW;		// 0x10
	txBuf[14] = MAX9867_VOL_LOW;		// 0x11
	txBuf[15] = MAX9867_MIC_GAIN_DIS;	// 0x12
	txBuf[16] = MAX9867_MIC_GAIN_DIS;	// 0x13
	txBuf[17] = MAX9867_AUX_DIS;		// 0x14
	txBuf[18] = MAX9867_DIG_MIC_DIS;	// 0x15
	txBuf[19] = MAX9867_SE_STEREO;		// 0x16
	txBuf[20] = MAX9867_DAC_EN;			// 0x17

	i2cReq.i2c = MAX9867_I2C;
	i2cReq.addr = MAX9867_SLV_ADDR;
	i2cReq.tx_buf = txBuf;
	i2cReq.tx_len = sizeof(txBuf);
	i2cReq.rx_buf = NULL;
	i2cReq.rx_len = 0;
	i2cReq.restart = 0;
	MXC_I2C_MasterTransaction(&i2cReq);
	MXC_Delay(MXC_DELAY_SEC(1));

	/* Verify write successful */
	txBuf[0] = MAX8967_REG_START_ADDR;
	i2cReq.i2c = MAX9867_I2C;
	i2cReq.addr = MAX9867_SLV_ADDR;
	i2cReq.tx_buf = txBuf;
	i2cReq.tx_len = 1;
	i2cReq.rx_buf = rxBuf;
	i2cReq.rx_len = sizeof(rxBuf);
	i2cReq.restart = 0;
	MXC_I2C_MasterTransaction(&i2cReq);

	if(memcmp(&rxBuf[0], &txBuf[1], sizeof(rxBuf)) != 0) {
		printf("Audio CODEC init failed!\n");
		return E_UNINITIALIZED;
	}

	return E_NO_ERROR;
}

int max9867_shutdown(void) {
	uint8_t txBuf[] = {MAX9867_PWR_REG, MAX9867_SHUTDOWN};
	mxc_i2c_req_t i2cReq;

	i2cReq.i2c = MAX9867_I2C;
	i2cReq.addr = MAX9867_SLV_ADDR;
	i2cReq.tx_buf = txBuf;
	i2cReq.tx_len = sizeof(txBuf);
	i2cReq.rx_buf = NULL;
	i2cReq.rx_len = 0;
	i2cReq.restart = 0;

	return MXC_I2C_MasterTransaction(&i2cReq);
}

void i2s_isr(void)
{
	int err;
	int flags = MXC_I2S_GetFlags();
    MXC_I2S_ClearFlags(flags);

    if(flags & MXC_F_I2S_INTFL_TX_HE_CH0) {
		//Play next audio samples
		err = playAudio(bufSelect);
		if(err == E_NONE_AVAIL) {
			if(!i2sTXBuf[bufSelect].lastTX) {
				bufSelect = (bufSelect + 1) % NUM_DATA_BUF;
				playAudio(bufSelect);
				refill = 1;
			}
			else {
				playing = 0;
			}
		}
    }
    else if(flags & MXC_F_I2S_INTFL_RX_THD_CH0) {
    	i2sRXFlag = 1;
    }
}

int getAudioFileStats(WavFile_t* fileInfo, classification_result_t classification_result) {
	qspi_packet_header_t pktHead;
	uint16_t statRecv[3];

	// Request the new file to be played
	qspi_slave_set_rx_state(QSPI_STATE_IDLE);
	qspi_slave_send_packet((uint8_t*) &classification_result, sizeof(classification_result), QSPI_PACKET_TYPE_AUDIO_CLASSIFICATION_RES);

	//Wait for response (put timeout on this?)
	while(qspi_slave_get_rx_state() != QSPI_STATE_CS_DEASSERTED_HEADER);

	// Check correct packet type
	pktHead = qspi_slave_get_rx_header();
	if(pktHead.info.packet_type != QSPI_PACKET_TYPE_AUDIO_STATS) {
		return E_INVALID;
	}

	//Prepare and receive audio statistics
	qspi_slave_set_rx_data((uint8_t*) statRecv, sizeof(statRecv));
	qspi_slave_trigger();
	if(qspi_slave_wait_rx() != E_NO_ERROR) {
		return E_TIME_OUT;
	}

	//set values
	if(qspi_slave_get_rx_state() != QSPI_STATE_COMPLETED) {
		return E_COMM_ERR;
	}

	fileInfo->numChannels = statRecv[0];
	fileInfo->bitsPerSample = statRecv[1];
	fileInfo->sampleRate = statRecv[2];

	return E_NO_ERROR;
}

int codec_i2sInit(WavFile_t* fileInfo) {
	int mono, err;

	//Check Params
	if(fileInfo == NULL) {
		return E_NULL_PTR;
	}

	// Choose between Stereo and mono modes
	if(fileInfo->numChannels == 2) {
		mono = 0;
	}
	else {
		mono = MXC_I2S_MONO_LEFT_CH;
	}

	// Initialize I2S
	mxc_i2s_req_t i2sReq;
	i2sReq.channelMode = MXC_I2S_INTERNAL_SCK_WS_0;
	i2sReq.stereoMode = mono;
	i2sReq.wordSize = MXC_I2S_DATASIZE_HALFWORD;
	i2sReq.justify = MXC_I2S_LSB_JUSTIFY;
	i2sReq.bitOrder = MXC_I2S_MSB_FIRST;
	i2sReq.wsPolarity = MXC_I2S_POL_NORMAL;
	i2sReq.sampleSize = MXC_I2S_SAMPLESIZE_SIXTEEN;
	i2sReq.clkdiv = I2S_CLKDIV(fileInfo->sampleRate, fileInfo->bitsPerSample);
	i2sReq.rawData = i2sTXBuf[bufSelect].dataBuf;
	i2sReq.txData = i2sTXBuf[bufSelect].dataBuf;
	i2sReq.rxData = NULL;
	i2sReq.length = I2S_TX_BUFFER_SIZE;
	if((err = MXC_I2S_Init(&i2sReq)) != E_NO_ERROR) {
		return err;
	}

	// Send initial data to Audio Codec
	if((err = playAudio(0)) != E_NO_ERROR) {
		return err;
	}
	MXC_I2S_TXEnable();

	// Enable I2S Interrupts
    MXC_NVIC_SetVector(I2S_IRQn, i2s_isr);
    NVIC_EnableIRQ(I2S_IRQn);
    __enable_irq();
    MXC_I2S_EnableInt(MXC_F_I2S_INTEN_TX_HE_CH0);

    return E_NO_ERROR;
}

int fillDataBuf(WavFile_t* fileInfo, int bufIndex) {
	int numRead = 0;

	// Fill buffer
	if(getAudioData(&i2sTXBuf[bufIndex].dataBuf[0], &numRead, fileInfo) != E_NO_ERROR) {
		return E_ABORT;
	}

	i2sTXBuf[bufIndex].index = 0;
	i2sTXBuf[bufIndex].numEntries = numRead;

	return numRead;
}

int getAudioData(uint16_t* startAddr, int* numRead, WavFile_t* fileInfo) {

	// Check parameters
	if(startAddr == NULL || numRead == NULL || fileInfo == NULL) {
		return E_NULL_PTR;
	}

	// Send Data Request to ME14 and wait for response
	qspi_slave_set_rx_state(QSPI_STATE_IDLE);
	qspi_slave_send_packet(NULL, 0, QSPI_PACKET_TYPE_AUDIO_DATA_REQ);

	while(qspi_slave_get_rx_state() != QSPI_STATE_CS_DEASSERTED_HEADER);

	// Check packet
	qspi_packet_header_t pktHead = qspi_slave_get_rx_header();
	if(pktHead.info.packet_type != QSPI_PACKET_TYPE_AUDIO_DATA) {
		return E_COMM_ERR;
	}

	// Receive bytes from ME14
	qspi_slave_set_rx_data((uint8_t*) startAddr, pktHead.info.packet_size);
	qspi_slave_trigger();
	if(qspi_slave_wait_rx() != E_NO_ERROR) {
		return E_TIME_OUT;
	}

	// Check packet receive successful
	if(qspi_slave_get_rx_state() != QSPI_STATE_COMPLETED) {
		return E_COMM_ERR;
	}

	// Return number of bytes read
	*numRead = pktHead.info.packet_size / sizeof(uint16_t);

	return E_NO_ERROR;
}

int playAudio(int bufIndex) {
	uint16_t leftSample, rightSample;
	int txBufLen, temp;

	// Get available space in I2S TX Buffer
	txBufLen = I2S_BUF_DEPTH - ((MXC_I2S->dmach0 & MXC_F_I2S_DMACH0_TX_LVL) >> MXC_F_I2S_DMACH0_TX_LVL_POS);

	// Send audio samples until I2S FIFO full or no more audio sample available
	while(txBufLen && i2sTXBuf[bufIndex].index < i2sTXBuf[bufIndex].numEntries) {
		temp = 0;

		// Get samples
		leftSample = i2sTXBuf[bufIndex].dataBuf[i2sTXBuf[bufIndex].index++];
		rightSample = i2sTXBuf[bufIndex].dataBuf[i2sTXBuf[bufIndex].index++];

		// Add samples to the data buffer
		temp = (rightSample << 16) | leftSample;
		MXC_I2S->fifoch0 = temp;

		txBufLen--;
	}

	if(i2sTXBuf[bufIndex].index >= i2sTXBuf[bufIndex].numEntries) {
		return E_NONE_AVAIL;
	}

	return E_NO_ERROR;
}

void stopAudio(void)
{
	classification_result_t stopClass = { .probability = 99.0,  .classification = CLASSIFICATION_DETECTED, .result = "STOP"};
	qspi_slave_set_rx_state(QSPI_STATE_IDLE);
	qspi_slave_send_packet((uint8_t*) &stopClass, sizeof(stopClass), QSPI_PACKET_TYPE_AUDIO_CLASSIFICATION_RES);
}
