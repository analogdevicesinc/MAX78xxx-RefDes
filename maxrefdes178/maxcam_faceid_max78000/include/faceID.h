#ifndef FACEID_H_
#define FACEID_H_

#define DATA_SIZE_IN (160*120*3)

// Data input: HWC (little data): 3x160x120

void load_input(const uint8_t *buffer);

void cnn_start(void);

int cnn_load(void);

void cnn_wait(void);

int cnn_check(void);

// Custom unload for this network:
// 8-bit data, shape: (512, 1, 1)
void cnn_unload(uint8_t *out_buf);

// Initialize CNN accelerator
int initCNN(void);


#endif
