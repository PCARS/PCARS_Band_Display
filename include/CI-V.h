#ifndef CI_V_H
#define CI_V_H

#include "Setup.h"

#define BUFFER_SIZE 11  // Buffer used to store CI-V packets, Max size for frequency command FE FE 94 00 01 XX XX XX XX XX FE
#define MIN_MSG_SIZE 6  // CI-V messages are at least 6 bytes long, eg. FE,FE,E0,94,XX,FD

#define RADIO_ADDR 0x94  // Change to match Radio's CI-V address
#define RADIO_ADDR_IDX 3  // See ICOM-7300 manual, section 19-2

#define CTRLR_ADDR 0x00  // Transceiver uses controller address 0x00 NOT 0xE0 as per the manual
#define CTRLR_ADDR_IDX 2

#define SEND_FREQ_CMD 0x00  // See ICOM-7300 manual, section 19-2
#define SEND_MODE_CMD 0x01  // See ICOM-7300 manual, section 19-2
#define SEND_TX_CMD 0x1C    // See ICOM-7300 manual, section 19-2 
#define CMD_IDX 4           // See ICOM-7300 manual, section 19-2

#define DATA_AREA_IDX 5  // See ICOM-7300 manual, section 19-2
#define FREQ_DATA_SIZE 4  // Actually is five bytes long, but last byte is for 100's and 1000's of MHz

#define MODE_IDX 0  // See ICOM-7300 manual, section 19-2

#define END_OF_MSG 0xFD  // See ICOM-7300 manual, section 19-2


extern const char* band_1;    // Store current band in use for radio # 1
extern const char* mode_1;    // Store current mode in use for radio # 1
extern const char* band_2;    // Store current band in use for radio # 2
extern const char* mode_2;    // Store current mode in use for radio # 2
extern const char* tx_rx_1;   // Store current tx/rx mode for radio # 1
extern const char* tx_rx_2;   // Store current tx/rx mode for radio # 2
extern uint8_t station;  // Store the station number of that last received packet


// Function prototypes
long decodeFrequency( byte *freqBytes );
const char* determineBand(long frequency);
const char* decodeMode( byte *modeBytes );
const char* decodeTX( byte *txBytes );
void processCIV( HardwareSerial &radio);

#endif