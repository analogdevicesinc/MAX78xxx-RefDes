

# UNet Demo on the MAXREFDES178# Cube Camera

## Getting Started with MAXREFDES178, Building Firmware, Loading and Debugging

Please refer to main MAXREFDES178 documentation for instructions how to build, and load firmware:

[Getting Started with the MAXREFDES178# Cube Camera](./../maxrefdes178_doc/README.md)

NOTE: UNet demo does not support Android application.


## Testing UNet Demo

This demo shows a U-Net network, trained to segment images into four categories and color them as follows:

- Building: Red
- Tree: Green
- Sky: Blue
- Unknown: Black

After loading all 3 components of UNet Demo (MAX32666, MAX78000_video and MAX78000_audio), turn on the cube camera:

- Plug a USB-C cable to charge the device.

- Press power button for 1 seconds to turn on the device.

- Power LED will start blinking blue.

- Maxim logo, BLE MAC, serial number and firmware version will appear on the LCD.
  
  ![](../maxrefdes178_doc/unet_intro.jpg)

- Device will start with pressing Start Video

- Keyword Spotting is enabled and the detected words are displayed on the top. However, **voice commands are disabled** in this demo. Following words are detected:

  - ['up', 'down', 'left', 'right', 'stop', 'go', 'yes', 'no', 'on', 'off', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'zero'].

- The images are captured at 250msec interval and the alternates with the result of segmentations

  ![](../maxrefdes178_doc/unet_camera.jpg)

  ![](../maxrefdes178_doc/unet_mask.jpg)
