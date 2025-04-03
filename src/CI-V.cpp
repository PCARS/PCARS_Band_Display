#include "CI-V.h"


const char* band_1;    // Store current band in use for radio # 1
const char* mode_1;    // Store current mode in use for radio # 1
const char* band_2;    // Store current band in use for radio # 2
const char* mode_2;    // Store current mode in use for radio # 2
const char* tx_rx_1;   // Store current tx/rx mode for radio # 1
const char* tx_rx_2;   // Store current tx/rx mode for radio # 2
uint8_t station;  // Store the station number of that last received packet


// Function to decode and extract frequency from CI-V message
void processCIV( HardwareSerial &radio)
{
  byte buffer[BUFFER_SIZE];  // Used to store received CI-V data

  uint8_t index;  // Used to point to data inside the buffer array


  ets_delay_us ( 5729 );  // Allow time for UART RX buffer to fill. @19200 baud, one bit = 1/19200 = 52.08 us. 8N1 = 10 bits/byte. So 52.08 x 10 = 521 us/byte
               // So far a CI-V packet of 11 bytes (BUFFER_SIZE), 11 x 521 us = 5729 us

  for ( index = 0; index < BUFFER_SIZE; index++ )  // Loop through data in the UART RX buffer
  {
  
    if ( radio.available() )  // Check if data still available in the UART RX buffer
      buffer[index] = radio.read();  // Read data from UART RX buffer and store in buffer array
  
    if ( buffer[index] == END_OF_MSG )  // End of CI-V message, break out of data capture loop
      break;

  }

  
  while( radio.available() )  // Clear UART RX buffer
    radio.read();

  if ( index >= MIN_MSG_SIZE && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[index] == END_OF_MSG )  // Check for appropriately formatted data capture
  {
    Serial.print( F( "Raw CI-V: " ) );  // Annunciate RAW CI-V data packet values

    for ( uint8_t i = 0; i <= index; i++ )  // Loop through buffer
    {
        Serial.print( buffer[i], HEX );  // Print buffer contents
        Serial.print( F( " " ) );  // Print a space between buffer data
    }
    
    Serial.println();  // Print a blank line
  

    switch ( buffer[CMD_IDX] )
    {

      case SEND_FREQ_CMD:     // Check if it's a frequency update (0xFE 0xFE 0x94 0x00 0x00 .... 0xFE)
      {
        long frequency = decodeFrequency( buffer + DATA_AREA_IDX );  // Decode frequency data and store
        Serial.print( F( "New Frequency: " ) );  // Print frequency label
        Serial.print( frequency );  // Print frequency
        Serial.println( F( " Hz" ) );  // Print frequency units
        determineBand(frequency);  // Determine operating band based on frequency

        break;
      }

      case SEND_MODE_CMD:  // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
      {
        const char* mode = decodeMode( buffer + DATA_AREA_IDX);  // Decode mode data and store
        Serial.print( F( "New Mode: " ) );  // Print mode label
        Serial.println( mode );  // Print mode

        break;
      }

      case SEND_TX_CMD:  // Check if it's TX/RX status update (0xFE 0xFE 0x94 0x00 0x1C ...)
      {
        const char* tx_rx = decodeTX( buffer + DATA_AREA_IDX + 1);  // Decode TX/RX status data and store
        Serial.print( F( "TX/RX: " ) );  // Print TX/RX status label
        Serial.println( tx_rx );  // Print TX/RX status

        break;
      }

    }

  }

}


// Decode frequency from BCD-encoded CI-V data
long decodeFrequency( byte *freqBytes )
{
    
  long frequency = 0;  // Initialize frequency at 0
  
  for( uint8_t i = 0; i < FREQ_DATA_SIZE; i++ )  // Cycle through the first 4 frequency bytes. Fifth byte is 100's & 1000's of MHz, so not needed.
  {

    frequency += ( freqBytes[i] & 0x0F ) * pow( 10, 2 * i );      // Bottom half of byte for the 1's, see ICOM-7300 manual 19-9
    frequency += ( freqBytes[i] >> 4 )   * pow( 10, 2 * i + 1 );  // Top half of byte for the 10's, see ICOM-7300 manual 19-9

  }

  // freq += ((freqBytes[0] >> 4) & 0x0F) * 10;
  // freq += (freqBytes[0] & 0x0F) * 1;
  
  // freq += ((freqBytes[1] >> 4) & 0x0F) * 1E3;
  // freq += (freqBytes[1] & 0x0F) * 1E2;
  
  // freq += ((freqBytes[2] >> 4) & 0x0F) * 1E5;
  // freq += (freqBytes[2] & 0x0F) * 1E4;
  
  // freq += ((freqBytes[3] >> 4) & 0x0F) * 1E7;
  // freq += (freqBytes[3] & 0x0F) * 1E6;

  return frequency;
}


const char* determineBand(long frequency)
{

  const char* band;  // Used to store band based on frequency

  // Assign band based on frequency. See Hambands4_Color_17x11.pdf in lib/Reference folder 
  if (frequency >= 1800000 && frequency <= 2000000) band = "160M";
  else if (frequency >= 3500000 && frequency <= 4000000) band = "80M";
  else if (frequency >= 5255000 && frequency <= 5405000) band = "60M";
  else if (frequency >= 7000000 && frequency <= 7300000) band = "40M";
  else if (frequency >= 10100000 && frequency <= 10150000) band = "30M";
  else if (frequency >= 14000000 && frequency <= 14350000) band = "20M";
  else if (frequency >= 18068000 && frequency <= 18168000) band = "17M";
  else if (frequency >= 21000000 && frequency <= 21450000) band = "15M";
  else if (frequency >= 24890000 && frequency <= 24990000) band = "12M";
  else if (frequency >= 28000000 && frequency <= 29700000) band = "10M";
  else if (frequency >= 50000000 && frequency <= 54000000) band = "6M";
  else if (frequency >= 70000000 && frequency <= 70500000) band = "4M";
  else band = "???";  // In case frequency is out of bounds

  // Assign updated band based on which radio station transmitted the update
  if ( station == 1 )
  band_1 = band;

  else if ( station == 2 )
    band_2 = band;

  return band; // Return current band

}


const char* decodeMode( byte *modeBytes )
{

  const char* mode;  // Used to store mode based on received BCD code

  switch ( modeBytes[MODE_IDX] )  // See ICOM-7300 manual section 19-9
  {
    case 0:
      mode = "LSB";
      break;
    
    case 1:
      mode = "USB";
      break;

    case 2:
      mode = "AM";
      break;
    
    case 3:
      mode = "CW";
      break;
    
    case 4:
      mode = "RTTY";
      break;
    
    case 5:
      mode = "FM";
      break;

    case 7:
      mode = "CW-R";
      break;

    case 8:
      mode = "RTTYR";
      break;

    default:
      mode = "???";  // In case of unrecognized operating mode
    
  }

  // Assign updated mode based on which radio station transmitted the update
  if (station == 1)
    mode_1 = mode;
  
  else if (station == 2)
    mode_2 = mode;

  return mode;  // Return current mode

}


const char* decodeTX( byte *txBytes )
{

 const char* tx_rx;  // Used to store TX/RX status based on received BCD code
  
 if ( txBytes[MODE_IDX] == 0 )  // See ICOM-7300 manual section 19-7
  tx_rx = "RX";

 if ( txBytes[MODE_IDX] == 1 )
  tx_rx = "TX";
  
// Assign updated TX/RX status based on which radio station transmitted the update  
if ( station == 1 )
  tx_rx_1 = tx_rx;
  
else if ( station == 2 )
  tx_rx_2 = tx_rx;

return tx_rx;  // Return current TX/RX status

}