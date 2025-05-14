#include "Display.h"  // Default header file

void update_Display( )
{
  // Uses static variables to retain previous settings
  static uint8_t prev_band_1;   // Store previously tuned band
  static String prev_mode_1;   // Store previously set mode. Initialized to an empty string ( "" )

  static uint8_t prev_band_2;   // Store previously tuned band 
  static String prev_mode_2;   // Store previously set mode. Initialized to an empty string ( "" )


  if ( station == 1 )  // Check if station 1 was the latest to send an update
  {
    
    matrix.setTextColor( STATION_1_COLOR );  // Set text color for Station # 1
    
    if ( band_1 != prev_band_1 )  // Only update display if changes occurred
    {
      String band_1_str = String( band_1 ) + 'M';
      
      matrix.fillRect( LEFT_MARGIN, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
      matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );  // Set cursor to appropriate position
      matrix.print( band_1_str );  // Update display with new data
      prev_band_1 = band_1;  // Store current setting
    }

    if ( mode_1 != prev_mode_1 )  // Only update display if changes occurred
    {
      matrix.fillRect( LEFT_MARGIN, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
      matrix.setCursor( 1, FOURTH_ROW_Y );  // Set cursor to appropriate position
      matrix.print( mode_1 );  // Update display with new data
      prev_mode_1 = mode_1;  // Store current setting
    }

  }

  if ( station == 2 )  // Check if station 2 was the latest to send an update
  {
    
    matrix.setTextColor( STATION_2_COLOR );  // Set text color for Station # 2
    
    if ( band_2 != prev_band_2 )  // Only update display if changes occurred
    {
      String band_2_str = String( band_2 ) + 'M';
      
      matrix.fillRect( TEXT_BOX_X, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth( band_2_str ) , THIRD_ROW_Y );  // Set cursor to appropriate position
      matrix.print( band_2 );  // Update display with new data
      prev_band_2 = band_2;  // Store current setting
    }

    if ( mode_2 != prev_mode_2 )  // Only update display if changes occurred
    {
      matrix.fillRect( TEXT_BOX_X, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth( mode_2 ), FOURTH_ROW_Y );  // Set cursor to appropriate position
      matrix.print( mode_2 );  // Update display with new data
      prev_mode_2 = mode_2;  // Store current setting
    }

  }

/******************************* Clear station(s) band and mode if associated radio is non-responsive ********************************/

  if ( radio1_active == false )
  {
    matrix.fillRect( LEFT_MARGIN, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );   // Erase old displayed setting
    matrix.fillRect( LEFT_MARGIN, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
  }

  if ( radio2_active == false )
  {
    matrix.fillRect( TEXT_BOX_X, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );   // Erase old displayed setting
    matrix.fillRect( TEXT_BOX_X, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK );  // Erase old displayed setting
  }
  
}