#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

// Define the panel configuration to override defaults in library
#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 64
#define MATRIX_CHAIN 1 // Number of panels chained together


#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


// ESP32 DevKit Pin Definitions (Avoiding Serial1 and Serial2 conflicts)
#define P_R1  25
#define P_G1  26
#define P_B1  27
#define P_R2  14
#define P_G2  12
#define P_B2  13

#define P_A   33
#define P_B   23
#define P_C   5
#define P_D   19
#define P_E   32

#define P_LAT 21
#define P_OE  22
#define P_CLK 18


#define SERIAL_BAUD 115200

#define CI_V_BAUD 19200  // Change if needed to match Radios baud date



// define color codes
#define BLACK 0
#define NAVY 15
#define DARKGREEN 992
#define DARKCYAN 1007
#define MAROON 30720
#define PURPLE 30735
#define OLIVE 31712
#define LIGHTGREY 50712
#define DARKGREY 31727
#define BLUE 31
#define GREEN  2016
#define CYAN 2047
#define RED 63488
#define MAGENTA 63519
#define YELLOW 65504
#define WHITE 65535
#define ORANGE 64800
#define GREENYELLOW 45029
#define PINK 64536

#define STATION_1_COLOR MAGENTA
#define STATION_2_COLOR CYAN

#define TEXT_BOX_WIDTH 27 // 6*4 + (4-1)*1 = 24 + 3 = 27
#define CHAR_HEIGHT 7
#define CHAR_WIDTH 5
#define LEFT_MARGIN 1
#define RIGHT_MARGIN 1
#define ROW_SEPARATION 10
#define FIRST_ROW_Y 1
#define SECOND_ROW_Y FIRST_ROW_Y  +  5
#define THIRD_ROW_Y  SECOND_ROW_Y +  ROW_SEPARATION
#define FOURTH_ROW_Y THIRD_ROW_Y  +  ROW_SEPARATION
#define FIFTH_ROW_Y FOURTH_ROW_Y  +  ROW_SEPARATION
#define SIXTH_ROW_Y FIFTH_ROW_Y   +  ROW_SEPARATION
#define SEVENTH_ROW_Y SIXTH_ROW_Y +  ROW_SEPARATION


extern MatrixPanel_I2S_DMA matrix;

void setup_Serial();
void setup_LED_Display();
uint8_t calculateTextWidth(String text);


#endif