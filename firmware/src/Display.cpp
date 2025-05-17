#include "setup.h"
#include "Display.h"  // Default header file

void update_Display( )
{
  // Uses static variables to retain previous settings
  static uint8_t prev_band_1;   // Store previously tuned band
  static String prev_mode_1;   // Store previously set mode. Initialized to an empty string ( "" )

  static uint8_t prev_band_2;   // Store previously tuned band
  static String prev_mode_2;   // Store previously set mode. Initialized to an empty string ( "" )

  char time_string[32];  // Character strinf to build time display in proper format

  uint8_t prev_minute;


    
  if ( band_1 != prev_band_1 )  // Only update display if changes occurred
  {

    matrix.setTextColor( STATION_1_COLOR );  // Set text color for Station # 1
      
    String band_1_str = String( band_1 ) + "m";
      
    matrix.fillRect( LEFT_MARGIN, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
    matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );  // Set cursor to appropriate position
    matrix.print( band_1_str );  // Update display with new data
    prev_band_1 = band_1;  // Store current setting
    
  }

  if ( mode_1 != prev_mode_1 )  // Only update display if changes occurred
  {

    matrix.setTextColor( STATION_1_COLOR );  // Set text color for Station # 1
      
    matrix.fillRect( LEFT_MARGIN, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
    matrix.setCursor( 1, FOURTH_ROW_Y );  // Set cursor to appropriate position
    matrix.print( mode_1 );  // Update display with new data
    prev_mode_1 = mode_1;  // Store current setting
  }

    
  if ( band_2 != prev_band_2 )  // Only update display if changes occurred
  {

    matrix.setTextColor( STATION_2_COLOR );  // Set text color for Station # 2

    String band_2_str = String( band_2 ) + "m";
      
    matrix.fillRect( TEXT_BOX_X, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
    matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth( band_2_str ) , THIRD_ROW_Y );  // Set cursor to appropriate position
    matrix.print( band_2_str );  // Update display with new data
    prev_band_2 = band_2;  // Store current setting

  }

  if ( mode_2 != prev_mode_2 )  // Only update display if changes occurred
  {

    matrix.setTextColor( STATION_2_COLOR );  // Set text color for Station # 2
      
    matrix.fillRect( TEXT_BOX_X, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
    matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth( mode_2 ), FOURTH_ROW_Y );  // Set cursor to appropriate position
    matrix.print( mode_2 );  // Update display with new data
    prev_mode_2 = mode_2;  // Store current setting

  }


/******************************* Clear station(s) band and mode if associated radio is non-responsive ********************************/

  if ( radio1_active == false )
  {

    matrix.fillRect( LEFT_MARGIN, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );   // Erase old displayed setting

    matrix.fillRect( LEFT_MARGIN, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting

    band_1 = 0;

    mode_1 = "";

  }

  if ( radio2_active == false )
  {

    matrix.fillRect( TEXT_BOX_X, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );   // Erase old displayed setting

    matrix.fillRect( TEXT_BOX_X, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting

    band_2 = 0;

    mode_2 = "";

  }


/******************************* Display current date and time ********************************/
  // Get updated NTP time at the top of each hour
  if ( timeinfo.tm_min == 0 && timeinfo.tm_sec < 3 )
    configTzTime( "EST5EDT,M3.2.0/2,M11.1.0/2", ntpServer );

  if( getLocalTime( &timeinfo ) == false )
    Serial.print( "Failed to obtain time");


  // Build the time string in the desired format for dispating on LED matrix
  snprintf(time_string, sizeof( time_string), "%d/%02d %d:%02d", timeinfo.tm_mon + 1,timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min );

  String display_time = String( time_string );  // Convert char array to String object

  if( timeinfo.tm_min != prev_minute )  // At the top of each minute, i.e when seconds = 0
  {
    matrix.fillRect( 0, SIXTH_ROW_Y, MATRIX_WIDTH - RIGHT_MARGIN, CHAR_HEIGHT, BLACK );   // Erase old displayed setting
    prev_minute = timeinfo.tm_min;
  }


   matrix.setTextColor( GREEN );  // Set text color for date/time display
   matrix.setCursor( ( MATRIX_WIDTH - calculateTextWidth( display_time ) ) / 2, SIXTH_ROW_Y);  // Center bottom row
   matrix.print( display_time );  // Update display with new date/time
  
}