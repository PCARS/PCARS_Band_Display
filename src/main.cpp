#include "setup.h"
#include "Band_Display.h"


void setup()
{

  setup_Serial();
    
  setup_LED_Display();

  Serial.println( F( "Listening to IC-7300 radio(s)..." ) );

}


void loop()
{

  if ( Serial1.available() )
  {
    station = 1;
    Serial.print( F( "Radio 1: " ) );
    processCIV( Serial1 );
  }

  if ( Serial2.available() )
  {
    station = 2;
    Serial.print( F( "Radio 2: " ) );
    processCIV( Serial2 );
  }

  update_Display(station);

}