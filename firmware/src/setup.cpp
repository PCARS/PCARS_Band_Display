#include "setup.h"  // Default header file
#include "CI-V.h"  // Required for radio queries


MatrixPanel_I2S_DMA matrix;  // Instantiate LED matrix object
struct tm timeinfo;
const char* ssid = "TMOBILE-6A99";
const char* password = "2da6e267ca";
const char* ntpServer = "pool.ntp.org";

void setup_PWM()
{

  ledcSetup( BUZZER_CHANNEL, BUZZER_FREQUENCY, 8 );  // PWM Channel, Buzzer tone frequency, 8-bit resolution

  ledcAttachPin( BUZZER_PIN, BUZZER_CHANNEL ); // Attach pin to PWM channel

}


void setup_Serial()
{

  Serial.begin( SERIAL_BAUD );  // Main Serial for debugging

  Serial1.begin( CI_V_BAUD, SERIAL_8N1, RX_1, TX_1);     // RX, TX (Safe GPIOs)
  
  Serial2.begin( CI_V_BAUD, SERIAL_8N1, RX_2, TX_2  );  //  RX, TX (Safe GPIOs)

}


void setup_LED_Display()
{
  
  const String station_Label = "Station";
  const String station_Num_1 = "#1";
  const String station_Num_2 = "#2";

  
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
  mx_config.driver = LED_MATRIX_DRIVER;  // Specific driver IC for LED matrix
  
  
  // Initialize the matrix
  if ( matrix.begin( mx_config ) == false )  // Test for LED matrix initialization
  {

    Serial.println( F( "Matrix initialization failed!" ) );  // Print error message

    ESP.restart();  // Restart ESP32 if LED matrix will not initialize

  }


  matrix.setBrightness( BRIGHTNESS );  // 0 - 255

  matrix.clearScreen();  // Clear the screen


  matrix.setTextColor( STATION_LABEL_COLOR );  // Set text color for Station labels
  
  matrix.setCursor( ( MATRIX_WIDTH - calculateTextWidth( station_Label ) ) / 2, FIRST_ROW_Y);  // Center top row
  matrix.print( station_Label );  // Display Station label
      
  matrix.setTextColor( STATION_LABEL_COLOR );  // Set text color for Station labels

  matrix.setCursor( LEFT_MARGIN, SECOND_ROW_Y);  // Left justified
  matrix.print( station_Num_1 );  // Display station # 1 label
      
  matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth( station_Num_2 ), SECOND_ROW_Y);  // Right justified
  matrix.print( station_Num_2 );  // Display station # 2 label

  matrix.drawLine( LEFT_MARGIN, THIRD_ROW_Y - 2, MATRIX_WIDTH - 2, THIRD_ROW_Y - 2, DARKGREY );

  // radio1_active =  query_Radio( Serial1, 1 );

  // if ( radio1_active )  // Get current band and mode from radio. Only update display if radio is active
  // {

  //   matrix.setTextColor( STATION_1_COLOR );  // Set text color for Station 1

  //   String band_1_str = String( band_1 ) + "m";
    
  //   matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );  // Left justified
  //   matrix.print( band_1_str );  // Display band

  //   matrix.setCursor( LEFT_MARGIN, FOURTH_ROW_Y );  // Left justified
  //   matrix.print( mode_1 );  // Display mode

    // matrix.setCursor( LEFT_MARGIN, FIFTH_ROW_Y );  // Left justified
    // matrix.print( F( "TA33" ) );  // Display antenna in use

    // matrix.setCursor( LEFT_MARGIN, SIXTH_ROW_Y );  // Left justified
    // matrix.print( F( "800W" ) );  // Display amplifier power

    // matrix.setTextColor(GREEN);  // Set text color
    // matrix.setCursor( LEFT_MARGIN, SEVENTH_ROW_Y );  // Left justified
    // matrix.print( F( "RX" ) );  // Display RX/TX status

  // }

  // radio2_active =  query_Radio( Serial2, 2 );

  // if( radio2_active )  // Get current band and mode from radio. Only update display if radio is active
  // {

  //   matrix.setTextColor(STATION_2_COLOR);  // Set text color for Station 2

  //   String band_2_str = String( band_2 ) + "m";

  //   matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(band_2_str) , THIRD_ROW_Y );  // Right justified
  //   matrix.print( band_2_str );  // Display band
    
  //   matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(mode_2), FOURTH_ROW_Y );  // Right justified
  //   matrix.print( mode_2 );  // Display mode

    // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("PRO"), FIFTH_ROW_Y );  // Right justified
    // matrix.print( "PRO" );  // Display antenna in use

    // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("1000W"), SIXTH_ROW_Y );  // Right justified
    // matrix.print( "1000W" );  // Display amplifier power

    // matrix.setTextColor(RED);  // Set text color
    // matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth("TX"), SEVENTH_ROW_Y );  // Right justified
    // matrix.print( F( "TX" ) );  // Display RX/TX status

  // }

}


// Function to calculate the pixel width of a text string
uint8_t calculateTextWidth( String text )
{

  uint8_t length = text.length();  // Get the length of the text
  
  return ( CHAR_WIDTH * length ) + length - 1;   // Calculate total width of the string in pixels

}


void  setup_WIFI()
{

  Serial.printf( "Connecting to %s", ssid );
  WiFi.begin( ssid, password );

  while ( WiFi.status() != WL_CONNECTED )
  {
    delay( 500 );
    Serial.print( "." );
  }

  printWifiStatus();

}


/*=============================================================== */
/* This prints out Wi-Fi settings                                 */
/*=============================================================== */
void printWifiStatus() {

  /* print the SSID of the network you're attached to */
  Serial.println("\r\n----WiFi connected----");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  /* print your assigned IP address */
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  /* print the WiFi's received signal strength */
  Serial.print("RSSI:");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm\r\n");
}

void setup_NTP()
{
  uint8_t timeout_NTP = 0;
  
  configTzTime( "EST5EDT,M3.2.0/2,M11.1.0/2", ntpServer );

  while( getLocalTime( &timeinfo ) == false && timeout_NTP < NTP_TIMEOUT )
  {
    Serial.println( F( "Waiting for time update...") );
    delay( 1000 );
    timeout_NTP++;
  }

}
