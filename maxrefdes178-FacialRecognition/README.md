

# Facial Recognition Demo on the MAXREFDES178# Cube Camera

## Getting Started with MAXREFDES178, Building Firmware, Loading and Debugging

Please refer to main MAXREFDES178 documentation for instructions how to build, and load firmware:

[Getting Started with the MAXREFDES178# Cube Camera](./../maxrefdes178_doc/README.md)

NOTE: This demo does not support Android application.

# Overview

This demo includes FaceDetection and FaceID CNN models and runs them sequentially. It has also a record feature where the user can add the people they want to the database.

The FaceID CNN model weight binary file (weigths_2.bin), DotProduct model weight binary file ( weights_3.bin), and database file (database.bin) need to be programmed into the SD card. The FaceDetection CNN model detects a face and the application draws a box around the face.

The FaceID CNN model demonstrates the identification of several persons from their facial images.

For this purpose, the FaceID CNN model generates a 512-length embedding for a given image, whose distance to whole embeddings stored for each subject is calculated. . The image is identified as either one of these subjects or 'Unknown' depending on the embedding distances.

## Testing Facial Recognition Demo

The SD Card should contain three binaries: "weights_2.bin", "weights_3.bin" and "database.bin". "weights_2.bin" is used to load weights of the FaceID model, "weights_3.bin" is used to load weights of the DotProducts, and "database.bin" is used to keep track of the recorded people. These binaries can be found inside the SDCardBinaries folder.

After loading all 3 components of DigitDetect Demo (MAX32666, MAX78000_video and MAX78000_audio), turn on the cube camera:

- Insert the programmed SD Card into the CubeCamera.

- Plug a USB-C cable to charge the device.

- Press the power button for 1 second to turn on the device.

- Power LED will start blinking blue.

- Analog Devices logo, serial number, and firmware version will appear on the LCD.

- The device will start by pressing "Start Video"

- The application detects faces in captured images and draws a box around them. If the application recognizes the faces, FaceID classification results appear at the bottom of the LCD.


### Record Mode

- Plug a USB-C cable to charge the device.

- Press the power button for 1 second to turn on the device.

- Power LED will start blinking blue.

- Analog Devices logo, serial number, and firmware version will appear on the LCD.

- The device will start by pressing "Start Video"

- Record mode is enabled by pressing the "X" button. The user can type a name with the help of the keyboard. The maximum name size is six characters.

- The device will capture the face by pressing "Capture"

- The embeddings will record by pressing "Ok". The capture will be discarded by pressing "Retry".

- Record mode is disabled by pressing the "X" button again. Now the device will recognize the recorded face.
