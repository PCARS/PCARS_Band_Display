This folder contains firmware files for the ESP32 DevKit V1 board that is used to drive a 64x64 HUB75 P3 LED Matrix display.

The firmware also interfaces with the radio's CI-V busses by utilizing the onboard HW UARTS of the ESP32.

The firmware listens for changes to frequency and operating mode.

It can also query the radios on startup to initialize the LED display with the proper status information. (band and operating mode)

The firmware will also activate a warning buzzer if both stations are on the same operating band for more than 3 seconds.

The LED Matrix is driven by ESP32-HUB75-MatrixPanel-I2S-DMA library.

The firmware was developed using the Arduino framework utilizing PlatFormIO running inside MS VS Code. See the platformio.ini file for details.
