Upated PCARS Operating Band Display.

Firmware for the ESP32 DevKit V1 board that is used to drive the 64x64 HUB75 LED Matrix display.
The firmware also interfaces with the radio's CI-V busses by utilizing the HW UARTS. The firware listens for changes to frequency and operating mode.
It can also query the radios on starup to initialize the LED display with the proper status information.
The firmware will also activate a warning buzzer if both stations are on the same operating bamd.
