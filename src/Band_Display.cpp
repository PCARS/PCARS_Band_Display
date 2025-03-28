#include "Band_Display.h"


MatrixPanel_I2S_DMA matrix;

void setup_Serial()
{

    Serial.begin( SERIAL_BAUD );  // Main Serial for debugging
    Serial1.begin( CI_V_BAUD, SERIAL_8N1, 4, 15 ); // TX=GPIO4, RX=GPIO5 (Safe GPIOs)
    Serial2.begin( CI_V_BAUD );  // ✅ Serial2 remains on GPIO16/17 (No conflicts with LED matrix)

}


void setup_LED_Display()
{
  
  
  HUB75_I2S_CFG mx_config( MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_CHAIN );


  mx_config.gpio.r1 = P_R1;
  mx_config.gpio.g1 = P_G1;
  mx_config.gpio.b1 = P_B1;
  mx_config.gpio.r2 = P_R2;
  mx_config.gpio.g2 = P_G2;
  mx_config.gpio.b2 = P_B2;
  mx_config.gpio.a = P_A;
  mx_config.gpio.b = P_B;
  mx_config.gpio.c = P_C;
  mx_config.gpio.d = P_D;
  mx_config.gpio.e = P_E;
  mx_config.gpio.clk = P_CLK;
  mx_config.gpio.lat = P_LAT;
  mx_config.gpio.oe = P_OE;
  mx_config.clkphase = false;
  mx_config.driver = HUB75_I2S_CFG::FM6124;
  
  
  
 // Initialize the matrix
 if (!matrix.begin(mx_config))
 {
  Serial.println("Matrix initialization failed!");
  while (1);
}


matrix.setBrightness(90);
matrix.setTextSize(1);

matrix.clearScreen();  // Clear the screen


// Set text color (yellow)
matrix.setTextColor(matrix.color444(15, 15, 0));


matrix.setCursor(8,0);
matrix.print("Stations");
    
matrix.setCursor( 1,7);
matrix.print("1");
    
matrix.setCursor(57,7);
matrix.print("2");



   // Set text color (magenta)
   matrix.setTextColor(matrix.color444(15, 0, 15)); 
  
   matrix.setCursor( 1, 17 );
   matrix.print( "20M");

   matrix.setCursor( 1, 27 );
   matrix.print( "USB");

   matrix.setCursor( 1, 37 );
   matrix.print( F( "TA33" ) );

   matrix.setCursor( 1, 47 );
   matrix.print( F( "800W" ) );


  // Set text color (cyan)
  matrix.setTextColor(matrix.color444(0, 15, 15)); 

  matrix.setCursor( 46, 17 );
  matrix.print( "80M");
  
  matrix.setCursor( 40, 27 );
  matrix.print( "RTTY");

  matrix.setCursor( 46, 37 );
  matrix.print( "PRO" );

  matrix.setCursor( 40, 47 );
  matrix.print( "500W" );

}


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


String decodeMode( byte *modeBytes )
{

  switch ( modeBytes[MODE_IDX] )
  {
    case 0:
      return "LSB";
    
    case 1:
      return "USB";

    case 2:
      return "AM";
    
    case 3:
      return "CW";
    
    case 4:
      return "RTTY";
    
    case 5:
      return "FM";

    case 7:
      return "CW-R";

    case 8:
      return "RTTY-R";

    default:
      return "???";
    
  }

}


// Function to decode and extract frequency from CI-V message
void processCIV( HardwareSerial &radio, uint8_t station )
{
  byte buffer[BUFFER_SIZE];

  uint8_t index;

  long frequency;
  
  String mode;

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
    }

    // Check if it's a mode update (0xFE 0xFE 0x94 0x00 0x01 ...)
    if (buffer[RADIO_ADDR_IDX] == RADIO_ADDR && buffer[CTRLR_ADDR_IDX] == CTRLR_ADDR && buffer[CMD_IDX] == SEND_MODE_CMD)
    {
        mode = decodeMode( buffer + DATA_AREA_IDX );
        Serial.print( F( "New Mode: " ) );
        Serial.println( mode );
    }

    update_Display( frequency, mode, station );

  }

}


void update_Display( long frequency, String mode, uint8_t station )
{



}