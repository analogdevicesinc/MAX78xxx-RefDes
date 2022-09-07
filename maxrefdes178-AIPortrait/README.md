

# AIPortrait Demo on the MAXREFDES178# Cube Camera

## Getting Started with MAXREFDES178, Building Firmware, Loading and Debugging

Please refer to main MAXREFDES178 documentation for instructions how to build, and load firmware:

[Getting Started with the MAXREFDES178# Cube Camera](./../maxrefdes178_doc/README.md)

NOTE: This demo does not support Android application.


## Testing AIPortrait Demo

This demo shows a hi-resolution U-Net network, trained to segment images into two categories and color them as follows:

- Portrait: Original color
- Background: Blue

After loading all 3 components of AIPortrait Demo (MAX32666, MAX78000_video and MAX78000_audio), turn on the cube camera:

- Plug a USB-C cable to charge the device.

- Press power button for 1 seconds to turn on the device.

- Power LED will start blinking blue.

- Analog Devices logo, BLE MAC, serial number and firmware version will appear on the LCD.
  
  ![](../maxrefdes178_doc/aiportrait_intro.jpg)

- Device will start with pressing "Start Video"

- Keyword Spotting is enabled and the detected words are displayed on the top. However, **voice commands are disabled** in this demo. Following words are detected:

  - ['up', 'down', 'left', 'right', 'stop', 'go', 'yes', 'no', 'on', 'off', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'zero', 'cube'].

- The images are captured at 1 sec interval and the alternates with the result of segmentations

  ![](../maxrefdes178_doc/aiportrait_camera.jpg)

  ![](../maxrefdes178_doc/aiportrait_mask.jpg)
