#include "Band_Display.h"
#include "setup.h"


String band_1;    // Store current band in use for radio # 1
String mode_1;    // Store current mode in use for radio # 1
String band_2;    // Store current band in use for radio # 2
String mode_2;    // Store current mode in use for radio # 2
String tx_rx_1;   // Store current tx/rx mode for radio # 1
String tx_rx_2;   // Store current tx/rx mode for radio # 2
uint8_t station;  // Store the station number of that last received packet


// Decode frequency from BCD-encoded CI-V data
long decodeFrequency( byte *freqBytes )
{
    
  long freq = 0;  // Initialize frequency at 0
  
  for( uint8_t i = 0; i < FREQ_DATA_SIZE; i++ )  // Cycle through the first 4 frequency bytes. Fifth byte is 100's & 1000's of MHz, so not needed.
  {

    freq += ( freqBytes[i] & 0x0F ) * pow( 10, 2 * i );      // Bottom half of byte for the 1's, see ICOM-7300 manual 19-9
    freq += ( freqBytes[i] >> 4 )   * pow( 10, 2 * i + 1 );  // Top half of byte for the 10's, see ICOM-7300 manual 19-9

  }

  // freq += ((freqBytes[0] >> 4) & 0x0F) * 10;
  // freq += (freqBytes[0] & 0x0F) * 1;
  
  // freq += ((freqBytes[1] >> 4) & 0x0F) * 1E3;
  // freq += (freqBytes[1] & 0x0F) * 1E2;
  
  // freq += ((freqBytes[2] >> 4) & 0x0F) * 1E5;
  // freq += (freqBytes[2] & 0x0F) * 1E4;
  
  // freq += ((freqBytes[3] >> 4) & 0x0F) * 1E7;
  // freq += (freqBytes[3] & 0x0F) * 1E6;

  return freq;
}


String determineBand(long frequency)
{

  String band;  // Used to store band based on frequency

  // Assign band based on frequency. See Hambands4_Color_17x11.pdf in lib/Reference Docs folder 
  if (frequency >= 1800000 && frequency <= 2000000) band = "160M";
  else if (frequency >= 3500000 && frequency <= 4000000) band = "80M";
  else if (frequency >= 5330000 && frequency <= 5405000) band = "60M";
  else if (frequency >= 7000000 && frequency <= 7300000) band = "40M";
  else if (frequency >= 10100000 && frequency <= 10150000) band = "30M";
  else if (frequency >= 14000000 && frequency <= 14350000) band = "20M";
  else if (frequency >= 18068000 && frequency <= 18168000) band = "17M";
  else if (frequency >= 21000000 && frequency <= 21450000) band = "15M";
  else if (frequency >= 24890000 && frequency <= 24990000) band = "12M";
  else if (frequency >= 28000000 && frequency <= 29700000) band = "10M";
  else if (frequency >= 50000000 && frequency <= 54000000) band = "6M";
  else band = "???";  // In case frequency is out of bounds

  // Assign updated band based on which radio station transmitted the update
  if (station ==1)
  band_1 = band;

  else if (station ==2)
    band_2 = band;

  return band; // Return current band

}


String decodeMode( byte *modeBytes )
{

  String mode;   // Used to store mode based on received BCD code
  
  switch ( modeBytes[MODE_IDX] )  // See ICOM-7300 manual section 19-9
  {
    case 0:
      mode = "LSB";
    
    case 1:
      mode = "USB";

    case 2:
      mode = "AM";
    
    case 3:
      mode = "CW";
    
    case 4:
      mode = "RTTY";
    
    case 5:
      mode = "FM";

    case 7:
      mode = "CW-R";

    case 8:
      mode = "RTTY-R";

    default:
      mode = "???";  // In case of unrecognized operating mode
    
  }

  // Assign updated mode based on which radio station transmitted the update
  if (station ==1)
    mode_1 = mode;
  
  else if (station ==2)
    mode_2 = mode;

  return mode;  // Return current mode

}

String decodeTX( byte *txBytes )
{

  String tx_rx;  // Used to store TX/RX status based on received BCD code
  
 if ( txBytes[MODE_IDX] == 0 )  // See ICOM-7300 manual section 19-7
  tx_rx = "RX";

 if ( txBytes[MODE_IDX] == 1 )
  tx_rx = "TX";
  
// Assign updated TX/RX status based on which radio station transmitted the update  
if (station ==1)
  tx_rx_1 = tx_rx;
  
else if (station ==2)
  tx_rx_2 = tx_rx;

return tx_rx;  // Return current TX/RX status

}


// Function to decode and extract frequency from CI-V message
void processCIV( HardwareSerial &radio)
{
  byte buffer[BUFFER_SIZE];  // Used to store received CI-V data

  uint8_t index;  // Used to point to data inside the buffer array

  long frequency;  // Used to store operating frequency
  
  delay( 10 );  // Allow time for UART RX buffer to fill

  for ( index = 0; index < BUFFER_SIZE; index++ )  // Loop through data in the UART RX buffer
  {
  
    if ( radio.available() )  // Check if data still available in the UART RX buffer
      buffer[index] = radio.read();  // Read data from UART RX buffer and store in buffer array
  
    if ( buffer[index] == END_OF_MSG )  // End of CI-V message, break out of data capture loop
      break;  

  }

  
  while( radio.available() )  // Clear UART RX buffer
    radio.read();

  if ( index >= MIN_MSG_SIZE )  // Check for appropriately sized data capture
  {
    Serial.print( F( "Raw CI-V: " ) );  // Annunciate RAW CI-V data packet values

    for ( uint8_t i = 0; i <= index; i++ )  // Loop through buffer
    {
        Serial.print( buffer[i], HEX );  // Print buffer contents
        Serial.print( F( " " ) );  // Print a space between buffer data
    }
    Serial.println();  // Print a blank line
  

    // Check if it's a frequency update (0xFE 0xFE 0x94 0x00 0x00 ...)
    if (buffer[RADIO_ADDR_IDX] == RADIO_ADDR && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[CMD_IDX] == SEND_FREQ_CMD)
    {
        frequency = decodeFrequency( buffer + DATA_AREA_IDX );  // Decode frequency data and store
        Serial.print( F( "New Frequency: " ) );  // Print frequency label
        Serial.print( frequency );  // Print frequency 
        Serial.println( F( " Hz" ) );  // Print frequency units
        determineBand(frequency);  // Determine operating band based on frequency
    }

    // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
    if (buffer[RADIO_ADDR_IDX] == RADIO_ADDR && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[CMD_IDX] == SEND_MODE_CMD)
    {
        String mode = decodeMode( buffer + DATA_AREA_IDX);  // Decode mode data and store
        Serial.print( F( "New Mode: " ) );  // Print mode label
        Serial.println( mode );  // Print mode
    }

    // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
    if (buffer[RADIO_ADDR_IDX] == RADIO_ADDR && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[CMD_IDX] == SEND_TX_CMD)
    {
        String tx_rx = decodeTX( buffer + DATA_AREA_IDX + 1);  // Decode TX/RX status data and store
        Serial.print( F( "TX/RX: " ) );  // Print TX/RX status label
        Serial.println( tx_rx );  // Print TX/RX status
    }

  }

}


void update_Display( )
{
  // Uses static variables to retain previous settings
  static String prev_band_1;   // Initialized to an empty string ( "" )
  static String prev_mode_1;   // Initialized to an empty string ( "" )
  static String prev_tx_rx_1;  // Initialized to an empty string ( "" )

  static String prev_band_2;   // Initialized to an empty string ( "" )
  static String prev_mode_2;   // Initialized to an empty string ( "" )
  static String prev_tx_rx_2;  // Initialized to an empty string ( "" )


  if (station == 1)  // Check if station 1 was the latest to send an update
  {
    
    matrix.setTextColor(STATION_1_COLOR);  // Set text color (magenta)
    
    if (band_1 != prev_band_1)  // Only update display if changes occurred
    {
      matrix.drawRect(LEFT_MARGIN, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );  // Set cursor to appropriate position
      matrix.print( band_1);  // Update display with new data
      prev_band_1 = band_1;  // Store current setting   
    }

    if (mode_1 != prev_mode_1)  // Only update display if changes occurred
    {
      matrix.drawRect(LEFT_MARGIN, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( 1, FOURTH_ROW_Y );  // Set cursor to appropriate position
      matrix.print( mode_1 );  // Update display with new data
      prev_mode_1 = mode_1;  // Store current setting   
    }

    if ( tx_rx_1 != prev_tx_rx_1)  // Only update display if changes occurred
    {
      matrix.drawRect(LEFT_MARGIN, SIXTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( 1, SIXTH_ROW_Y );  // Set cursor to appropriate position
      
      if(tx_rx_1 == "TX")
        matrix.setTextColor(RED);  // Set text color red for TX
      
      else if (tx_rx_1 == "RX")
        matrix.setTextColor(GREEN);  // Set text color green for RX
      
      matrix.print( tx_rx_1 );  // Update display with new data
      
      prev_tx_rx_1 = tx_rx_1;  // Store current setting   
    }

  }

  if (station == 2)  // Check if station 2 was the latest to send an update
  {
    
    matrix.setTextColor(STATION_2_COLOR);  // Set text color (magenta)
    
    if (band_2 != prev_band_2)  // Only update display if changes occurred
    {
      matrix.drawRect(TEXT_BOX_X, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(band_2) , THIRD_ROW_Y );  // Set cursor to appropriate position
      matrix.print( band_2);  // Update display with new data
      prev_band_2 = band_2;  // Store current setting  
    }

    if (mode_2 != prev_mode_2)  // Only update display if changes occurred
    {
      matrix.drawRect(TEXT_BOX_X, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(mode_2), FOURTH_ROW_Y );  // Set cursor to appropriate position
      matrix.print( mode_2 );  // Update display with new data
      prev_mode_2 = mode_2;  // Store current setting  
    }

    if ( tx_rx_2 != prev_tx_rx_2)  // Only update display if changes occurred
    {
      matrix.drawRect(TEXT_BOX_X, SIXTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(tx_rx_2), SIXTH_ROW_Y );  // Set cursor to appropriate position
      
      if(tx_rx_2 == "TX")
        matrix.setTextColor(RED);  // Set text color red for TX
      
      else if (tx_rx_2 == "RX")
        matrix.setTextColor(GREEN);  // Set text color green for RX
      
      matrix.print( tx_rx_2 );  // Update display with new data
      
      prev_tx_rx_2 = tx_rx_2;  // Store current setting  
    }

  }
  
}