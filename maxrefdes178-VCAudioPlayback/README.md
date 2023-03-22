

# Voice Controlled Audio Playback Demo on the MAXREFDES178# Cube Camera

## Getting Started with MAXREFDES178, Building Firmware, Loading and Debugging

Please refer to main MAXREFDES178 documentation for instructions how to build, and load firmware:

[Getting Started with the MAXREFDES178# Cube Camera](../maxrefdes178_doc/README.md)

NOTE: Voice Controlled Audio Playback demo does not support Android application.


## Testing Voice Controlled Audio Playback Demo

For reference:

  * MAXDAP-TYPE-C Pico adaptor oriented toward the screen.

    ![](../maxrefdes178_doc/mrd178_buttons.jpg)

##### Setting up the Device

Ensure the MicroSD card is set up properly. First check that the SD card is formatted as a FAT filesystem. If it is not, it can be formatted using your host machine or by running the SDHC\_FAT example on a MAX32665/66 EV Kit. Next load the WAV files onto the SD card in the root directory. If you have your own WAV files you wish to use verify that the sampling rate is less than 48kHz and the sample width is less than 16 bits. Additionally, if you use your own WAV files you'll have to change the filenames on lines 143-146 of *maxrefdes178_max32666/max32666_main.c* and the value of "NUM\_FILES" on line 27 of *maxrefdes178_max32666/include/audio_processor.h*. However if you do not have your own WAV files, the example has been set up to use the WAV files provided in the *WAV-Files* folder.

Next, insert the MicroSD card into the MicroSD card slot located next to the Power Button with the notch on the MicroSD card pointing towards the top of the Cube Camera. Additionally, insert headphones into the audio jack closest to Button B.

##### Running the Example

For practical purposes, the device operates in two states: keyword spotting and audio playback. These states are indicated through Button B's and the Power Button's status LEDs. When in the keyword spotting state, Button B's LED will appear magenta and the Power Button's LED will appear red. When in the audio playback state, both status LEDs will appear green. Note that when the device is in audio playback state it is unable to process audio commands.

Begin by waiting for the device to enter the keyword spotting state, this should happen almost immediately. Once the device is in this state use the commands "GO", "LEFT" and "RIGHT" to control which audio track will be played back. "GO" plays the current track, "LEFT" plays the previous track, and "RIGHT" plays the next track.

Once a command is received, the device will enter the audio playback state as indicated by the status LEDs and the audio output through the headphones. The device will stay in the audio playback state until either Button B is pressed or the audio track has finished, at which point the device returns to the keyword spotting state.
