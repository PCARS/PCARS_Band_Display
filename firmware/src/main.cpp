/*
   This program receives CI-V data from two radio stations (Radio 1 & Radio 2)
   using serial communication. The CI-V protocol provides real-time updates on
   each radio's operating band, mode, and TX/RX status.

   The received data is processed to extract relevant information, and the
   64x64 LED matrix display is updated accordingly. Each radio's status is
   visually represented on the matrix, ensuring clear and immediate feedback
   on their operational states.

   - Radio 1 and Radio 2 CI-V data is received via separate serial inputs.
   - The program decodes the band and mode from the incoming CI-V packets.
   - The LED matrix is updated dynamically to reflect changes for each radio.
*/


#include "Setup.h"
#include "CI-V.h"
#include "Display.h"


void setup()
{

  setup_PWM();  // Setup PWM for driving passive buzzer
  
  setup_Serial();  // Setup serial ports for printing and CI-V radio communication
    
  setup_LED_Display();  // Setup LED Matrix display and display default settings

  setup_WIFI();

  setup_NTP();

  Serial.println( F( "Listening to IC-7300 radio(s)..." ) );  // Print that program is ready to receive CI-V data

}


void loop()
{

  radio1_active =  query_Radio( Serial1, 1 );  // Check for CI_V activity from Radio 1

  radio2_active =  query_Radio( Serial2, 2 );  // Check for CI_V activity from Radio 2

  update_Display();  // Update display if changes occurred

  band_Conflict_Check();  // Check for band conflicts and alert if needed

  delay( LOOP_DELAY );  // Delay to prevent excessive traffic on the CI-V bus
  
}