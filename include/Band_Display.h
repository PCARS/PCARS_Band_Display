#ifndef BAND_DISPLAY_H
#define BAND_DISPLAY_H

#include <Arduino.h>

#define BUFFER_SIZE 16
#define MIN_MSG_SIZE 6  // CI-V messages are at least 6 bytes long, eg. FE,FE,E0,94,XX,FD

#define RADIO_ADDR 0x94  // Change to match Radio's CI-V address
#define RADIO_ADDR_IDX 3

#define CTRLR_ADDR 0x00  // Transceiver uses controller address 0x00 NOT 0xE0 as per the manual
#define CTRLR_ADDR_IDX 2

#define SEND_FREQ_CMD 0x00
#define SEND_MODE_CMD 0x01
#define SEND_TX_CMD 0x1C
#define CMD_IDX 4

#define DATA_AREA_IDX 5
#define FREQ_DATA_SIZE 4  // Actually is five bytes long, but last byte is for 100's and 1000's of MHz

#define MODE_IDX 0

#define END_OF_MSG 0xFD

extern String band_1;
extern String band_2;
extern String mode_1;
extern String mode_2;
extern String tx_rx_1;
extern String tx_rx_2;
extern uint8_t station;

// Function prototypes
long decodeFrequency( byte *freqBytes );
String determineBand(long frequency);
String decodeMode( byte *modeBytes );
String decodeTX( byte *modeBytes );
void processCIV( HardwareSerial &radio);
void update_Display( uint8_t station );

#endif