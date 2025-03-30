#include "Band_Display.h"
#include "setup.h"


String band_1;
String mode_1;
String band_2;
String mode_2;
String tx_rx_1;
String tx_rx_2;
uint8_t station;


// Decode frequency from BCD-encoded CI-V data
long decodeFrequency( byte *freqBytes )
{
    
  long freq = 0;
  
  for( uint8_t i = 0; i < FREQ_DATA_SIZE; i++ )
  {

    freq += ( freqBytes[i] & 0x0F ) * pow( 10, 2 * i );
    freq += ( freqBytes[i] >> 4 )   * pow( 10, 2 * i + 1 );

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

  String band;

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
  else band = "???";

  if (station ==1)
  band_1 = band;

  else if (station ==2)
    band_2 = band;

  return band;

}


String decodeMode( byte *modeBytes )
{

  String mode;
  
  switch ( modeBytes[MODE_IDX] )
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
      mode = "???";
    
  }

  if (station ==1)
    mode_1 = mode;
  
  else if (station ==2)
    mode_2 = mode;

  return mode;

}

String decodeTX( byte *txBytes )
{

  String tx_rx;
  
 if ( txBytes[MODE_IDX] == 0 )
  tx_rx = "RX";

 if ( txBytes[MODE_IDX] == 1 )
  tx_rx = "TX";
  
if (station ==1)
  tx_rx_1 = tx_rx;
  
else if (station ==2)
  tx_rx_2 = tx_rx;

return tx_rx;

}


// Function to decode and extract frequency from CI-V message
void processCIV( HardwareSerial &radio)
{
  byte buffer[BUFFER_SIZE];

  uint8_t index;

  long frequency;
  
  delay( 10 );  // Allow time for UART RX buffer to fill

  for ( index = 0; index < BUFFER_SIZE; index++ )
  {
  
    if ( radio.available() )
      buffer[index] = radio.read();
  
    if ( buffer[index] == END_OF_MSG )  // End of CI-V message
      break;  

  }

  
  while( radio.available() )  // Clear UART RX buffer
    radio.read();

  if ( index >= MIN_MSG_SIZE )
  {
    Serial.print( F( "Raw CI-V: " ) );

    for ( uint8_t i = 0; i <= index; i++ )
    {
        Serial.print( buffer[i], HEX );
        Serial.print( F( " " ) );
    }
    Serial.println();
  

    // Check if it's a frequency update (0xFE 0xFE 0x94 0x00 0x00 ...)
    if (buffer[RADIO_ADDR_IDX] == RADIO_ADDR && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[CMD_IDX] == SEND_FREQ_CMD)
    {
        frequency = decodeFrequency( buffer + DATA_AREA_IDX );
        Serial.print( F( "New Frequency: " ) );
        Serial.print( frequency );
        Serial.println( F( " Hz" ) );
        determineBand(frequency);
    }

    // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
    if (buffer[RADIO_ADDR_IDX] == RADIO_ADDR && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[CMD_IDX] == SEND_MODE_CMD)
    {
        String mode = decodeMode( buffer + DATA_AREA_IDX);
        Serial.print( F( "New Mode: " ) );
        Serial.println( mode );
    }

    // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
    if (buffer[RADIO_ADDR_IDX] == RADIO_ADDR && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[CMD_IDX] == SEND_TX_CMD)
    {
        String tx_rx = decodeTX( buffer + DATA_AREA_IDX + 1);
        Serial.print( F( "TX/RX: " ) );
        Serial.println( tx_rx );
    }

  }

}


void update_Display( uint8_t station )
{

  static String prev_band_1;
  static String prev_mode_1;

  static String prev_band_2;
  static String prev_mode_2;

  static String prev_tx_rx_1;
  static String prev_tx_rx_2;


  if (station == 1)
  {
    
    matrix.setTextColor(STATION_1_COLOR);  // Set text color (magenta)
    
    if (band_1 != prev_band_1)
    {
      matrix.drawRect(LEFT_MARGIN, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);
      matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );
      matrix.print( band_1);
      prev_band_1 = band_1;  
    }

    if (mode_1 != prev_mode_1)
    {
      matrix.drawRect(LEFT_MARGIN, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);
      matrix.setCursor( 1, FOURTH_ROW_Y );
      matrix.print( mode_1 );
      prev_mode_1 = mode_1;
    }

    if ( tx_rx_1 != prev_tx_rx_1)
    {
      matrix.drawRect(LEFT_MARGIN, SIXTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);
      matrix.setCursor( 1, SIXTH_ROW_Y );
      
      if(tx_rx_1 == "TX")
        matrix.setTextColor(RED);  // Set text color (magenta)
      else if (tx_rx_1 == "RX")
        matrix.setTextColor(GREEN);  // Set text color (magenta)
      
      matrix.print( tx_rx_1 );
      
        prev_tx_rx_1 = tx_rx_1;
    }
  }

  if (station == 2)
  {
    
    matrix.setTextColor(STATION_2_COLOR);  // Set text color (magenta)
    
    if (band_2 != prev_band_2)
    {
      matrix.drawRect(28, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(band_2) , THIRD_ROW_Y );
      matrix.print( band_2);
      prev_band_2 = band_2;
    }

    if (mode_2 != prev_mode_2)
    {
      matrix.drawRect(28, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(mode_2), FOURTH_ROW_Y );
      matrix.print( mode_2 );
      prev_mode_2 = mode_2;
    }

    if ( tx_rx_2 != prev_tx_rx_2)
    {
      matrix.drawRect(28, SIXTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(tx_rx_2), SIXTH_ROW_Y );
      
      if(tx_rx_2 == "TX")
      matrix.setTextColor(RED);  // Set text color (magenta)
      else if (tx_rx_2 == "RX")
        matrix.setTextColor(GREEN);  // Set text color (magenta)
      
      matrix.print( tx_rx_2 );
      
      prev_tx_rx_2 = tx_rx_2;
    }
  }
  
}