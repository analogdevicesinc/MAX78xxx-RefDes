

# ASL Demo on the MAXREFDES178# Cube Camera

## Getting Started with MAXREFDES178, Building Firmware, Loading and Debugging

**NOTE: The accuracy of the existing ASL demo is low due to the inferior quality of images taken with the cube camera as compared with images used in the training dataset. An improved version of this demo using the Domain Adaptation technique is available and will be ported to this demo in the near future.**

Please refer to main MAXREFDES178 documentation for instructions how to build, and load firmware:

[Getting Started with the MAXREFDES178# Cube Camera](./../maxrefdes178_doc/README.md)

Please note that ASL demo does not support Android application.


## Testing ASL Demo

After loading all 3 components of ASL Demo (MAX32666, MAX78000_video and MAX78000_audio), turn on the cube camera:

- Plug a USB-C cable to charge the device.

- Press power button for 1 seconds to turn on the device.

- Power LED will start blinking blue.

- Logo, BLE MAC, serial number and firmware version will appear on the LCD.
  
- Device will start with pressing Start Video

- Keyword Spotting is enabled and the detected words are displayed on the top. However, **voice commands are disabled** in this demo. Following words are detected:

  - ['up', 'down', 'left', 'right', 'stop', 'go', 'yes', 'no', 'on', 'off', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'zero', 'cube'].

- The images are captured at 250msec interval and categorized as a certain letter.
