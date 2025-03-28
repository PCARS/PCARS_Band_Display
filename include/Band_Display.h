#ifndef BAND_DISPLAY_H
#define BAND_DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

// Define the panel configuration to override defaults in library
#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 64
#define MATRIX_CHAIN 1 // Number of panels chained together


#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


// ESP32 DevKit Pin Definitions (Avoiding Serial1 and Serial2 conflicts)
#define P_R1  25
#define P_G1  26
#define P_B1  27
#define P_R2  14
#define P_G2  12
#define P_B2  13

#define P_A   33
#define P_B   23
#define P_C   5
#define P_D   19
#define P_E   32

#define P_LAT 21
#define P_OE  22
#define P_CLK 18


// Declare matrix as an external variable (no memory allocated here)
extern MatrixPanel_I2S_DMA matrix;

#endif



#define SERIAL_BAUD 115200

#define CI_V_BAUD 19200  // Change if needed to match Radios baud date

#define BUFFER_SIZE 16
#define MIN_MSG_SIZE 6  // CI-V messages are at least 6 bytes long, eg. FE,FE,E0,94,XX,FD

#define RADIO_ADDR 0x94  // Change to match Radio's CI-V address
#define RADIO_ADDR_IDX 3

#define CTRLR_ADDR 0x00  // Transceiver uses controller address 0x00 NOT 0xE0 as per the manual
#define CTRLR_ADDR_IDX 2

#define SEND_FREQ_CMD 0x00
#define SEND_MODE_CMD 0x01
#define CMD_IDX 4

#define DATA_AREA_IDX 5
#define FREQ_DATA_SIZE 4  // Actually is five bytes long, but last byte is for 100's and 1000's of MHz

#define MODE_IDX 0

#define END_OF_MSG 0xFD



// Function prototypes
void setup_Serial();
void setup_LED_Display();
long decodeFrequency( byte *freqBytes );
String decodeMode( byte *modeBytes );
void processCIV( HardwareSerial &radio, uint8_t station );
void update_Display( long frequency, String mode, uint8_t station );
void setup();
void loop();