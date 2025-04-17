#include "setup.h"
#include "CI-V.h"


MatrixPanel_I2S_DMA matrix;  // Instantiate LED matrix object


void setup_Digital_Pins()
{

  pinMode(LED_BUILTIN, OUTPUT);  // Set LED as output

  digitalWrite(LED_BUILTIN, LOW);  // Off by default

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
  if (matrix.begin(mx_config) == false)  // Test for LED matrix initialization
  {
    Serial.println("Matrix initialization failed!");  // Print error mesage
    while (true);  // Loop forever, do not proceed
  }


  matrix.setBrightness(90);  // 0 - 255, 90 is a good mid-range level
  matrix.setTextSize(1);  // Default text size, 7x5 pixels

  matrix.clearScreen();  // Clear the screen



  matrix.setTextColor(YELLOW);  // Set text color for Station labels
  
  matrix.setCursor( ( MATRIX_WIDTH - calculateTextWidth("Station") ) / 2, FIRST_ROW_Y);  // Center top row
  matrix.print("Station");  // Display Station label
      

  matrix.setCursor( LEFT_MARGIN, SECOND_ROW_Y);  // Left justified
  matrix.print("1");  // Display station # 1 label
      
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("2"), SECOND_ROW_Y);  // Right justified
  matrix.print("2");  // Display station # 2 label


  query_radio(Serial1, 1);  // Get current band and mode from radio

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


  query_radio(Serial2, 2);  // Get current band and mode from radio

  matrix.setTextColor(STATION_2_COLOR);  // Set text color for Station 2

  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(band_2) , THIRD_ROW_Y );  // Right justified
  matrix.print( band_2 );  // Display band
  
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(mode_2), FOURTH_ROW_Y );  // Right justified
  matrix.print( mode_2);  // Display mode

  // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("PRO"), FIFTH_ROW_Y );  // Right justified
  // matrix.print( "PRO" );  // Display antenna in use

  // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("1000W"), SIXTH_ROW_Y );  // Right justified
  // matrix.print( "1000W" );  // Display amplifier power

  // matrix.setTextColor(RED);  // Set text color
  // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("TX"), SEVENTH_ROW_Y );  // Right justified
  // matrix.print( F( "TX" ) );  // Display RX/TX status

}

// Function to calculate the pixel width of the text
uint8_t calculateTextWidth(String text)
{
  uint8_t length = text.length();  // Get the length of the text
  
  return ( CHAR_WIDTH * length ) + length - 1;   // Calculate total width of the string in pixels

}


void query_radio(HardwareSerial &radio, uint8_t station_num)
{

  const byte query_freq[] = {0xFE, 0xFE, RADIO_ADDR, CTRLR_ADDR, QUERY_FREQ_CMD, END_OF_MSG};
  const byte query_mode[] = {0xFE, 0xFE, RADIO_ADDR, CTRLR_ADDR, QUERY_MODE_CMD, END_OF_MSG};

  station = station_num;  // Set radio station number


  while( radio.available() )  // Flush UART RX buffer
    radio.read();

  radio.write( query_freq, sizeof(query_freq) );  // Send query command

  delay(50);
  //while( radio.available() <= 6 );  // Wait for radio to start reply

  Serial.print( F( "Command Echo: " ) );  // Print label

  for( uint8_t i = 0; i < 6; i++)  // Capture command echo
  {
    Serial.print(radio.read(), HEX);  // Read bytes and print
    Serial.print( F(" ") );  // Add space between printed bytes
  }

  Serial.println();  // Go to the next line

  processCIV( radio );  // Process CI-V data packet


  radio.write( query_mode, sizeof(query_mode) );  // Send query command

  delay(50);
  //while( radio.available() <= 6 );  // Wait for radio to start reply

  Serial.print( F( "Command Echo: " ) );
  
  for( uint8_t i = 0; i < 6; i++)
  {
    Serial.print(radio.read(), HEX);
    Serial.print( F(" ") );
  }

  Serial.println();

  processCIV( radio );  // Process CI-V data packet

}