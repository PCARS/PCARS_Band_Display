#include "CI-V.h"


enum Band {
  UNKNOWN,
  BAND_160M = 160,
  BAND_80M = 80,
  BAND_60M = 60,
  BAND_40M = 40,
  BAND_30M = 30,
  BAND_20M = 20,
  BAND_17M = 17,
  BAND_15M = 15,
  BAND_12M = 12,
  BAND_10M = 10,
  BAND_6M = 6
};


uint8_t band_1;   // Store current band in use for radio # 1
String mode_1;    // Store current mode in use for radio # 1
uint8_t band_2;   // Store current band in use for radio # 2
String mode_2;    // Store current mode in use for radio # 2
uint8_t station;  // Store the station number of that last received packet


// Function to decode and extract frequency from CI-V message. Return true if received packet is well formed
bool processCIV( HardwareSerial &radio )
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
      Serial.print( determineBand( frequency ) );  // Determine operating band based on frequency and print band
      Serial.println( F( 'M' ) );  // Print band unit, i.e. meters - M
    }

    if ( buffer[CMD_IDX] == SEND_MODE_CMD ||  buffer[CMD_IDX] == QUERY_MODE_CMD )  // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
    {
      Serial.print( F( "New Mode: " ) );  // Print mode label
      Serial.println( decodeMode( buffer + DATA_AREA_IDX ) );  // Decode mode data and print mode
    }

    return true;  // If received CI-V packet is well formed, return true 

  }

  return false;  // Otherwise return false

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


uint8_t determineBand( long frequency )
{

  uint8_t band;  // Used to store band based on frequency

  // Assign band based on frequency. See Hambands4_Color_17x11.pdf in Reference Docs folder 
  if (frequency >= 1800000 && frequency <= 2000000) band = BAND_160M;
  else if ( frequency >= 3500000 && frequency <= 4000000 ) band = BAND_80M;
  else if ( frequency >= 5255000 && frequency <= 5405000 ) band = BAND_60M;
  else if ( frequency >= 7000000 && frequency <= 7300000 ) band = BAND_40M;
  else if ( frequency >= 10100000 && frequency <= 10150000 ) band = BAND_30M;
  else if ( frequency >= 14000000 && frequency <= 14350000 ) band = BAND_20M;
  else if ( frequency >= 18068000 && frequency <= 18168000 ) band = BAND_17M;
  else if ( frequency >= 21000000 && frequency <= 21450000 ) band = BAND_15M;
  else if ( frequency >= 24890000 && frequency <= 24990000 ) band = BAND_12M;
  else if ( frequency >= 28000000 && frequency <= 29700000 ) band = BAND_10M;
  else if ( frequency >= 50000000 && frequency <= 54000000 ) band = BAND_6M;
  else band = UNKNOWN;  // In case frequency is out of bounds

  // Assign updated band based on which radio station transmitted the update
  if ( station == 1 )
    band_1 = band;

  else if ( station == 2 )
    band_2 = band;

  return band; // Return current band

}


String decodeMode( byte *modeBytes )
{

  String mode;  // Used to store mode based on received BCD

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
  static uint32_t conflict_active_timer = 0;  // Store how long band conflict has been active. Initialize to 0.

  static bool conflict_confirmed = false;  // Band conflict flag

  bool radio1_active;  // Store ON/OFFstatus of radio # 1

  bool radio2_active;  // Store ON/OFF status of radio # 2

  uint16_t tone_freq;  // Store tone frequency use to drive alarm buzzer

  static bool buzzer_on = false;  // Alarm buzzer on flag
  
  uint32_t time_now = millis();  // Capture current timestamp


#ifdef TWO_TONE_SIREN

  static uint32_t last_siren_time = 0;  // Keep track of time for alternating tones

#endif

/***************************** Check that both radio's are still active  **************************************/ 

  radio1_active = query_Radio( Serial1, 1 );  // Check if radio # 1 is still active

  radio2_active = query_Radio( Serial2, 2 );  // Check if radio # 2 is still active


/****************************** Determine if band conflict exists  ****************************************/ 

  if ( radio1_active && radio2_active && band_1 == band_2 )    // For alarm to trigger both radios must be on AND tuned to the same band 
  {
    if ( conflict_active_timer == 0 )  // First time detecting conflict
      conflict_active_timer = time_now;  // Store current millis() timestamp
    
    else if ( time_now - conflict_active_timer >= BAND_CONFLICT_HOLD_TIME )  // If conflict persists for sufficient time
      conflict_confirmed = true;  // Set band conflict flag
  }

  else  // If the bands are no longer the same, or one radio goes offline reset timer and conflict flag
  {
    conflict_active_timer = 0;  // Reset conflict timer

    conflict_confirmed = false;  // Reset conflict flag

  }
  
/****************************** Trigger/clear band conflict alarm as needed  ****************************************/  

  if ( conflict_confirmed )  // If conflict is active, play alternating tones for the siren
  {
      
#ifdef TWO_TONE_SIREN  

  // Alternate between two tones every `TONE_PERIOD` milliseconds
    if ( time_now - last_siren_time >= TONE_PERIOD )
    {
      last_siren_time = time_now;  // Update the time for the next tone

      // Alternate between the two tones
      if ( ( time_now / TONE_PERIOD ) % 2 == 0 )
        tone_freq = TONE_1_FREQ;  // WEE
          
      else
        tone_freq = TONE_2_FREQ;  // OOO

      ledcWriteTone( BUZZER_CHANNEL, tone_freq );  // Active buzzer with given tone frequency

    }

#else
    if ( buzzer_on == false )  // Only turn on alarm buzzer if it was previously off. This avoids repeated calls to the PWM function below
      ledcWriteTone( BUZZER_CHANNEL, BUZZER_FREQUENCY );  // Single-tone 

#endif  

    buzzer_on = true;  // Set alarm buzzer on flag
     
  }

  else  // Turn alarm buzzer off
  {

    if ( buzzer_on == true )  // Only turn off buzzer if it was previously on. This avoids repeated calls to the PWM function below
    {
      ledcWriteTone( BUZZER_CHANNEL, 0 );  // Zero PWM frequency turns off alarm buzzer

      buzzer_on = false;  // Clear buzzer on flag
    }

  }

 
  return conflict_confirmed;  // Return status of band conflict check

}