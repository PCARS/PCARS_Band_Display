#include "CI-V.h"


String band_1;    // Store current band in use for radio # 1
String mode_1;    // Store current mode in use for radio # 1
String band_2;    // Store current band in use for radio # 2
String mode_2;    // Store current mode in use for radio # 2
uint8_t station;  // Store the station number of that last received packet


// Function to decode and extract frequency from CI-V message
void processCIV( HardwareSerial &radio )
{
  byte buffer[BUFFER_SIZE];  // Used to store received CI-V data

  uint8_t index;  // Used to point to data inside the buffer array


  delayMicroseconds ( 5729 );  // Allow time for UART RX buffer to fill. @19200 baud, one bit = 1/19200 = 52.08 us. 8N1 = 10 bits/byte. So 52.08 x 10 = 521 us/byte
                               // So for a CI-V packet of 11 bytes (BUFFER_SIZE), 11 x 521 us = 5729 us

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
    Serial.print( F( "Raw CI-V: " ) );  // Print RAW CI-V data packet values

    for ( uint8_t i = 0; i <= index; i++ )  // Loop through buffer
    {
        Serial.print( buffer[i], HEX );  // Print buffer contents
        Serial.print( F( " " ) );  // Print a space between buffer data
    }
    
    Serial.println();  // Print a blank line
  

      if( buffer[CMD_IDX] == SEND_FREQ_CMD ||  buffer[CMD_IDX] == QUERY_FREQ_CMD )    // Check if it's a frequency update (0xFE 0xFE 0x94 0x00 0x00 .... 0xFE)
      {
        long frequency = decodeFrequency( buffer + DATA_AREA_IDX );  // Decode frequency data and store
        Serial.print( F( "New Frequency: " ) );  // Print frequency label
        Serial.print( frequency );  // Print frequency
        Serial.println( F( " Hz" ) );  // Print frequency units
      
        Serial.print( F( "Band: " ) );  // Print frequency label
        Serial.println( determineBand(frequency) );  // Determine operating band based on frequency and print band
      }

      if ( buffer[CMD_IDX] == SEND_MODE_CMD ||  buffer[CMD_IDX] == QUERY_MODE_CMD )  // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
      {
        Serial.print( F( "New Mode: " ) );  // Print mode label
        Serial.println( decodeMode( buffer + DATA_AREA_IDX) );  // Decode mode data and print mode
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

  return frequency;
}


String determineBand(long frequency)
{

  String band;  // Used to store band based on frequency

  // Assign band based on frequency. See Hambands4_Color_17x11.pdf in lib/Reference folder 
  if (frequency >= 1800000 && frequency <= 2000000) band = "160M";
  else if ( frequency >= 3500000 && frequency <= 4000000 ) band = "80M";
  else if ( frequency >= 5255000 && frequency <= 5405000 ) band = "60M";
  else if ( frequency >= 7000000 && frequency <= 7300000 ) band = "40M";
  else if ( frequency >= 10100000 && frequency <= 10150000 ) band = "30M";
  else if ( frequency >= 14000000 && frequency <= 14350000 ) band = "20M";
  else if ( frequency >= 18068000 && frequency <= 18168000 ) band = "17M";
  else if ( frequency >= 21000000 && frequency <= 21450000 ) band = "15M";
  else if ( frequency >= 24890000 && frequency <= 24990000 ) band = "12M";
  else if ( frequency >= 28000000 && frequency <= 29700000 ) band = "10M";
  else if ( frequency >= 50000000 && frequency <= 54000000 ) band = "6M";
  else if ( frequency >= 70000000 && frequency <= 70500000 ) band = "4M";
  else band = "???";  // In case frequency is out of bounds

  // Assign updated band based on which radio station transmitted the update
  if ( station == 1 )
  band_1 = band;

  else if ( station == 2 )
    band_2 = band;

  return band; // Return current band

}


String decodeMode( byte *modeBytes )
{

  String mode;  // Used to store mode based on received BCD code

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
  if ( station == 1 )
    mode_1 = mode;
  
  else if ( station == 2 )
    mode_2 = mode;

  return mode;  // Return current mode

}


bool band_Conflict_Check()
{

  static uint32_t conflict_detected_timer;  // Store how much time band conflict has been active
  uint32_t time_now;  // Store current time
  static bool conflict_active;  // Store state of conflict

  if( band_1 == band_2 )  // Check to see if bands are the same for both radios
  {
    time_now = millis();  // Get current time
    
    if( conflict_detected_timer == 0 )  // Check to see if this is the first time that the conflict was detected
      conflict_detected_timer = time_now;  // Recorded time when band conflict was first detected
    
    else if ( time_now - conflict_detected_timer  >= BAND_CONFLICT_HOLD_TIME && conflict_active == false )  // Check if conflict has persisted
    {
      ledcWriteTone( BUZZER_CHANNEL, BUZZER_FREQUENCY );  // Activate buzzer

      conflict_active = true;
      
      return true;  // Return true since conflict has been present for the determined time
    }

  }


  else
  {
    conflict_detected_timer = 0;  // Reset band conflict timer if no conflict exists

    conflict_active = false;

    ledcWriteTone( BUZZER_CHANNEL, 0 ); // 0 Hz stops the buzzer tone
  }

  return false;  // If you made it this far the conflict is no longer present
  
}