#include "setup.h"


MatrixPanel_I2S_DMA matrix;


void setup_Serial()
{

  Serial.begin( SERIAL_BAUD );  // Main Serial for debugging
  Serial1.begin( CI_V_BAUD, SERIAL_8N1, RX_1, TX_1);     // RX, TX (Safe GPIOs)
  Serial2.begin( CI_V_BAUD, SERIAL_8N1, RX_2, TX_2  );  //  RX, TX (Safe GPIOs)

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
  if (matrix.begin(mx_config) == false)
  {
    Serial.println("Matrix initialization failed!");
    while (1);
  }


  matrix.setBrightness(90);
  matrix.setTextSize(1);

  matrix.clearScreen();  // Clear the screen



  matrix.setTextColor(YELLOW);  // Set text color (yellow)
  
  matrix.setCursor( ( MATRIX_WIDTH - calculateTextWidth("Station") ) / 2, FIRST_ROW_Y);
  matrix.print("Station");
      

  matrix.setCursor( LEFT_MARGIN, SECOND_ROW_Y);
  matrix.print("1");
      
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("2"), SECOND_ROW_Y);
  matrix.print("2");



  matrix.setTextColor(STATION_1_COLOR);  // Set text color (magenta)
  
  matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );
  matrix.print( "20M");

  matrix.setCursor( LEFT_MARGIN, FOURTH_ROW_Y );
  matrix.print( "USB");

  matrix.setCursor( LEFT_MARGIN, FIFTH_ROW_Y );
  matrix.print( F( "TA33" ) );

  matrix.setCursor( LEFT_MARGIN, SIXTH_ROW_Y );
  matrix.print( F( "800W" ) );

  matrix.setTextColor(GREEN);  // Set text color
  matrix.setCursor( LEFT_MARGIN, SEVENTH_ROW_Y );
  matrix.print( F( "RX" ) );



  matrix.setTextColor(STATION_2_COLOR);  // Set text color (cyan)

  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("160M") , THIRD_ROW_Y );
  matrix.print( "160M");
  
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("RTTY"), FOURTH_ROW_Y );
  matrix.print( "RTTY");

  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("PRO"), FIFTH_ROW_Y );
  matrix.print( "PRO" );

  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("1000W"), SIXTH_ROW_Y );
  matrix.print( "1000W" );

  matrix.setTextColor(RED);  // Set text color
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("TX"), SEVENTH_ROW_Y );
  matrix.print( F( "TX" ) );

}

// Function to calculate the pixel width of the text
uint8_t calculateTextWidth(String text)
{
  uint8_t length = text.length();  // Get the length of the text
  
  return ( CHAR_WIDTH * length ) + length - 1;   // Calculate total width of the string in pixels   

}