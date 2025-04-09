/*
   This program receives CI-V data from two radio stations (Radio 1 & Radio 2) 
   using serial communication. The CI-V protocol provides real-time updates on 
   each radio's operating band, mode, and TX/RX status.

   The received data is processed to extract relevant information, and the 
   64x64 LED matrix display is updated accordingly. Each radio's status is 
   visually represented on the matrix, ensuring clear and immediate feedback 
   on their operational states.

   - Radio 1 and Radio 2 CI-V data is received via separate serial inputs.
   - The program decodes the band, mode, and TX/RX state from the incoming CI-V packets.
   - The LED matrix is updated dynamically to reflect changes for each radio.
*/


#include "Setup.h"
#include "CI-V.h"
#include "Display.h"


void setup()
{

  setup_Serial();  // Setup serial ports for printing and CI-V radio communication
    
  setup_LED_Display();  // Setup LED Matrix display and display default settings

  Serial.println( F( "Listening to IC-7300 radio(s)..." ) );  // Print that program is ready to receive CI-V data

}


void loop()
{

  if ( Serial1.available() )  // Check for activity from Radio 1
  {
    station = 1;  // Set radio station number
    Serial.print( F( "Radio 1: " ) );  // Print that radio 1 has activity
    processCIV( Serial1 );  // Process CI-V data packet
  }

  if ( Serial2.available() )  // Check for activity from Radio 2
  {
    station = 2;  // Set radio station number
    Serial.print( F( "Radio 2: " ) );  // Print that radio 2 has activity
    processCIV( Serial2 );  // Process CI-V data packet
  }

  update_Display();  // Update display if changes occurred

}