#ifndef CI_V_H
#define CI_V_H

#include "Setup.h"

#define BUFFER_SIZE 11  // Buffer used to store CI-V packets, Max size for frequency command FE FE 94 00 01 XX XX XX XX XX FE
#define MIN_MSG_SIZE 6  // CI-V messages are at least 6 bytes long, eg. FE,FE,E0,94,XX,FD

#define START_OF_MSG 0xFE  // Start flag for CI-V message start, See ICOM-7300 manual, section 19-2

#define RADIO_ADDR 0x94  // Change to match Radio's CI-V address
#define RADIO_ADDR_IDX 3  // See ICOM-7300 manual, section 19-2

#define CTRLR_ADDR 0x00  // Transceiver uses controller address 0x00 NOT 0xE0 as per the manual
#define CTRLR_ADDR_IDX 2

#define SEND_FREQ_CMD 0x00   // See ICOM-7300 manual, section 19-2
#define SEND_MODE_CMD 0x01   // See ICOM-7300 manual, section 19-2
#define QUERY_FREQ_CMD 0x03  // See ICOM-7300 manual, section 19-2
#define QUERY_MODE_CMD 0x04  // See ICOM-7300 manual, section 19-2
#define QUERY_CMD_SIZE 6     // See ICOM-7300 manual, section 19-2

#define CMD_IDX 4            // See ICOM-7300 manual, section 19-2

#define DATA_AREA_IDX 5   // See ICOM-7300 manual, section 19-2
#define FREQ_DATA_SIZE 4  // Actually is five bytes long, but last byte is for 100's and 1000's of MHz

#define MODE_IDX 0  // See ICOM-7300 manual, section 19-2

#define END_OF_MSG 0xFD  // See ICOM-7300 manual, section 19-2


#define BAND_CONFLICT_HOLD_TIME 3000  // Time required before alerting band conflict [ms]

//#define TWO_TONE_SIREN  // Comment out to disable two-tone WEE-OOO and enable single tone alert

#ifdef TWO_TONE_SIREN

#define TONE_PERIOD 700   // Time period for each tone in milliseconds
#define TONE_1_FREQ 1000  // Frequency of the first tone (WEE)
#define TONE_2_FREQ 500   // Frequency of the second tone (OOO)

#else

#define TONE_1_FREQ 2400  // Frequency of the single tone, near resonant frequency of 2400 Hz 

#endif

#define QUIET_BUS_MIN_TIME 100  // Time [ms] CI-V bus must be inactive before sending query transmissions from ESP32


extern uint8_t band_1;      // Store current band in use for radio # 1
extern String mode_1;       // Store current mode in use for radio # 1
extern bool radio1_active;  // Store ON/OFFstatus of radio # 1

extern uint8_t band_2;      // Store current band in use for radio # 2
extern String mode_2;       // Store current mode in use for radio # 2
extern bool radio2_active;  // Store ON/OFFstatus of radio # 1

extern uint8_t station;     // Store the station number of that last received packet


// Function prototypes
bool process_CIV( HardwareSerial &radio);  // Process CI-V packets. Return true of good packet received, false otherwise
long decodeFrequency( byte *freqBytes );  // Decode the radio operating frequency from the received CI-V packet
uint8_t determineBand( long frequency );  // Determine the US Amateur Radio HF Band based on operating frequency
String decodeMode( byte *modeBytes );     // Decode the radio operating mode from the received CI-V packet
bool band_Conflict_Check(); // Check if both radio's are active and trigger alarm if band conflict is present for a set amount of time
bool query_Radio(HardwareSerial &radio, uint8_t station_num );  // Query radio for operating frequency and mode. Return true if query successful

#endif