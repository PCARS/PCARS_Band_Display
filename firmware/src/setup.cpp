#include "setup.h"
#include "CI-V.h"


MatrixPanel_I2S_DMA matrix;  // Instantiate LED matrix object


void setup_PWM()
{

  ledcSetup( BUZZER_CHANNEL, BUZZER_FREQUENCY, 8 );  // Channel 0, 2kHz tone, 8-bit resolution

  ledcAttachPin( BUZZER_PIN, BUZZER_CHANNEL ); // Attach pin to channel 0

}


void setup_Serial()
{

  Serial.begin( SERIAL_BAUD );  // Main Serial for debugging
  Serial1.begin( CI_V_BAUD, SERIAL_8N1, RX_1, TX_1);     // RX, TX (Safe GPIOs)
  Serial2.begin( CI_V_BAUD, SERIAL_8N1, RX_2, TX_2  );  //  RX, TX (Safe GPIOs)

}


void setup_LED_Display()
{
  
  HUB75_I2S_CFG mx_config( MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_CHAIN );  // Configuration structure for matrix object instance initialization

  // Configure pins
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
  mx_config.driver = HUB75_I2S_CFG::FM6124;  // Specific driver IC for LED matrix
  
  
  // Initialize the matrix
  if ( matrix.begin( mx_config ) == false )  // Test for LED matrix initialization
  {
    Serial.println( F( "Matrix initialization failed!" ) );  // Print error message
    while ( true );  // Loop forever, do not proceed
  }


  matrix.setBrightness( BRIGHTNESS );  // 0 - 255, 90 is a good mid-range level

  matrix.clearScreen();  // Clear the screen


  matrix.setTextColor( STATION_LABEL_COLOR );  // Set text color for Station labels
  
  matrix.setCursor( ( MATRIX_WIDTH - calculateTextWidth( F( "Station" ) ) ) / 2, FIRST_ROW_Y);  // Center top row
  matrix.print(F ( "Station" ) );  // Display Station label
      

  matrix.setCursor( LEFT_MARGIN, SECOND_ROW_Y);  // Left justified
  matrix.print( F( "#1" ) );  // Display station # 1 label
      
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth( F( "#2" ) ), SECOND_ROW_Y);  // Right justified
  matrix.print( F( "#2" ) );  // Display station # 2 label


  query_Radio( Serial1, 1 );  // Get current band and mode from radio

  matrix.setTextColor(STATION_1_COLOR);  // Set text color for Station 1
  
  matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );  // Left justified
  matrix.print( band_1 );  // Display band

  matrix.setCursor( LEFT_MARGIN, FOURTH_ROW_Y );  // Left justified
  matrix.print( mode_1 );  // Display mode

  // matrix.setCursor( LEFT_MARGIN, FIFTH_ROW_Y );  // Left justified
  // matrix.print( F( "TA33" ) );  // Display antenna in use

  // matrix.setCursor( LEFT_MARGIN, SIXTH_ROW_Y );  // Left justified
  // matrix.print( F( "800W" ) );  // Display amplifier power

  // matrix.setTextColor(GREEN);  // Set text color
  // matrix.setCursor( LEFT_MARGIN, SEVENTH_ROW_Y );  // Left justified
  // matrix.print( F( "RX" ) );  // Display RX/TX status


  query_Radio( Serial2, 2 );  // Get current band and mode from radio

  matrix.setTextColor(STATION_2_COLOR);  // Set text color for Station 2

  String band_2_str = String( band_2 ) + 'M';

  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(band_2_str) , THIRD_ROW_Y );  // Right justified
  matrix.print( band_2_str );  // Display band
  
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(mode_2), FOURTH_ROW_Y );  // Right justified
  matrix.print( mode_2 );  // Display mode

  // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("PRO"), FIFTH_ROW_Y );  // Right justified
  // matrix.print( "PRO" );  // Display antenna in use

  // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("1000W"), SIXTH_ROW_Y );  // Right justified
  // matrix.print( "1000W" );  // Display amplifier power

  // matrix.setTextColor(RED);  // Set text color
  // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("TX"), SEVENTH_ROW_Y );  // Right justified
  // matrix.print( F( "TX" ) );  // Display RX/TX status

}

// Function to calculate the pixel width of a text string
uint8_t calculateTextWidth( String text )
{
  uint8_t length = text.length();  // Get the length of the text
  
  return ( CHAR_WIDTH * length ) + length - 1;   // Calculate total width of the string in pixels

}


bool query_Radio(HardwareSerial &radio, uint8_t station_num)
{

  const byte query[2][6] = {

    {START_OF_MSG, START_OF_MSG, RADIO_ADDR, CTRLR_ADDR, QUERY_FREQ_CMD, END_OF_MSG},  // Define frequency query command
    {START_OF_MSG, START_OF_MSG, RADIO_ADDR, CTRLR_ADDR, QUERY_MODE_CMD, END_OF_MSG}   // Define mode query command

  };

  bool query_response = false;  // Store if radio responded to query

  station = station_num;  // Set radio station number

  for ( uint8_t i = 0; i < 2; i++ )  // Loop through the queries
  {

    while( radio.available() )  // Flush UART RX buffer
      radio.read();

    radio.write( query[i], 6 );  // Send query command

    delay(50);  // Sufficient delay for response from radio

    for( uint8_t i = 0; i < 6; i++)  // Loop through and discard command echo response
      radio.read();

    query_response = processCIV( radio );  // Process CI-V data packet and store query result

  }

  return query_response;  // Return query response status

}