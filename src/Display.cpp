#include "Display.h"

void update_Display( )
{
  // Uses static variables to retain previous settings
  static const char* prev_band_1;   // Initialized to an empty string ( "" )
  static const char* prev_mode_1;   // Initialized to an empty string ( "" )
  static const char* prev_tx_rx_1;  // Initialized to an empty string ( "" )

  static const char* prev_band_2;   // Initialized to an empty string ( "" )
  static const char* prev_mode_2;   // Initialized to an empty string ( "" )
  static const char* prev_tx_rx_2;  // Initialized to an empty string ( "" )


  if (station == 1)  // Check if station 1 was the latest to send an update
  {
    
    matrix.setTextColor(STATION_1_COLOR);  // Set text color (magenta)
    
    if (strcmp(band_1, prev_band_1) != 0)  // Only update display if changes occurred
    {
      matrix.fillRect(LEFT_MARGIN, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( LEFT_MARGIN, THIRD_ROW_Y );  // Set cursor to appropriate position
      matrix.print( band_1);  // Update display with new data
      prev_band_1 = band_1;  // Store current setting
    }

    if (strcmp(mode_1, prev_mode_1) != 0)  // Only update display if changes occurred
    {
      matrix.fillRect(LEFT_MARGIN, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( 1, FOURTH_ROW_Y );  // Set cursor to appropriate position
      matrix.print( mode_1 );  // Update display with new data
      prev_mode_1 = mode_1;  // Store current setting
    }

    if (strcmp(tx_rx_1, prev_tx_rx_1) != 0)  // Only update display if changes occurred
    {
      matrix.fillRect(LEFT_MARGIN, SIXTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( 1, SIXTH_ROW_Y );  // Set cursor to appropriate position
      
      if(strcmp(tx_rx_1, "TX") == 0)
        matrix.setTextColor(RED);  // Set text color red for TX
      
      else if (strcmp(tx_rx_1, "RX") == 0)
        matrix.setTextColor(GREEN);  // Set text color green for RX
      
      matrix.print( tx_rx_1 );  // Update display with new data
      
      prev_tx_rx_1 = tx_rx_1;  // Store current setting
    }

  }

  if (station == 2)  // Check if station 2 was the latest to send an update
  {
    
    matrix.setTextColor(STATION_2_COLOR);  // Set text color (magenta)
    
    if (strcmp(band_2, prev_band_2) != 0)  // Only update display if changes occurred
    {
      matrix.fillRect(TEXT_BOX_X, THIRD_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(band_2) , THIRD_ROW_Y );  // Set cursor to appropriate position
      matrix.print( band_2);  // Update display with new data
      prev_band_2 = band_2;  // Store current setting
    }

    if (strcmp(mode_2, prev_mode_2) != 0)  // Only update display if changes occurred
    {
      matrix.fillRect(TEXT_BOX_X, FOURTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(mode_2), FOURTH_ROW_Y );  // Set cursor to appropriate position
      matrix.print( mode_2 );  // Update display with new data
      prev_mode_2 = mode_2;  // Store current setting
    }

    if (strcmp(tx_rx_2, prev_tx_rx_2) != 0)  // Only update display if changes occurred
    {
      matrix.fillRect(TEXT_BOX_X, SIXTH_ROW_Y, TEXT_BOX_WIDTH, CHAR_HEIGHT, BLACK);  // Erase old displayed setting
      matrix.setCursor( MATRIX_WIDTH - RIGHT_MARGIN - calculateTextWidth(tx_rx_2), SIXTH_ROW_Y );  // Set cursor to appropriate position
      
      if(strcmp(tx_rx_2, "TX") == 0)
        matrix.setTextColor(RED);  // Set text color red for TX
      
      else if (strcmp(tx_rx_2, "RX") == 0)
        matrix.setTextColor(GREEN);  // Set text color green for RX
      
      matrix.print( tx_rx_2 );  // Update display with new data
      
      prev_tx_rx_2 = tx_rx_2;  // Store current setting
    }

  }
  
}