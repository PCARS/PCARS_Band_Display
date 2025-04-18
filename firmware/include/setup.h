#ifndef SETUP_H
#define SETUP_H

#include <Arduino.h>       // Required libarary for Arduino programming
#include <Adafruit_GFX.h>  // LED matrix library dependancy


// Define the panel configuration to override defaults in library
#define MATRIX_WIDTH 64   // Matrix panel width in pixels
#define MATRIX_HEIGHT 64  // Matrix panel height in pixels
#define MATRIX_CHAIN 1    // Number of panels chained together


#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>  // LED matrix library

#define BUZZER_PIN 15  // Assign pin to drive passive buzzer with PWM
#define BUZZER_FREQUENCY 2500  // 2.5kHz typically close to buzzer resonant frequency
#define BUZZER_CHANNEL 0


#define SERIAL_BAUD 115200  // Baud rate for printing

#define CI_V_BAUD 19200  // Change if needed to match radio's baud date
#define RX_1 35  // Radio Station 1 UART RX pin
#define TX_1 23  // Radio Station 1 UART TX pin
#define RX_2 34  // Radio Station 2 UART RX pin
#define TX_2 33  // Radio Station 2 UART TX pin



// ESP32 DevKit Pin Definitions (Avoiding Serial1 and Serial2 conflicts)
// See 64x64 LED Matrix Connector Pinout.PNG in lib/Reference Docs folder

// Right side of ribbon cable with connector facing towards you, red stripe up
#define P_R1  21
#define P_B1  19
#define P_R2  18
#define P_B2   5
#define P_A   17
#define P_C   16
#define P_CLK  4
#define P_OE  15

// Left side of ribbon cable with connector facing towards you, red stripe up
#define P_G1  25
// GND PIN
#define P_G2  26
#define P_E   27 // Required for 64x64 LED Matrix
#define P_B   14
#define P_D   12
#define P_LAT 13  // A.K.A. Strobe
// GND PIN


// Define color codes, RGB565 format, eg 0xFFFF = 11111 111111 11111 = white
//                                                RRRRR GGGGGG BBBBB
#define BLACK       0x0000  // 0
#define NAVY        0x000F  // 15
#define DARKGREEN   0x03E0  // 992
#define DARKCYAN    0x03EF  // 1007
#define MAROON      0x7800  // 30720
#define PURPLE      0x780F  // 30735
#define OLIVE       0x7BC0  // 31712
#define LIGHTGREY   0xC618  // 50712
#define DARKGREY    0x7BEF  // 31727
#define BLUE        0x001F  // 31
#define GREEN       0x07E0  // 2016
#define CYAN        0x07FF  // 2047
#define RED         0xF800  // 63488
#define MAGENTA     0xF81F  // 63519
#define YELLOW      0xFFE0  // 65504
#define WHITE       0xFFFF  // 65535
#define ORANGE      0xFD20  // 64800
#define GREENYELLOW 0xB7E5  // 45029
#define PINK        0xFC9C  // 64536


#define BRIGHTNESS 90  // LED Matrix pixel brightness 0 - 255

#define STATION_LABEL_COLOR YELLOW  // Determine color for station labels
#define STATION_1_COLOR MAGENTA  // Determine color for station # 1
#define STATION_2_COLOR CYAN     // Determine color for station # 2


#define CHAR_HEIGHT   7  // Height of displayed characters in pixels
#define CHAR_WIDTH    5  // Width of displayed characters in pixels
#define LEFT_MARGIN   1  // Margin from left side of display in pixels
#define RIGHT_MARGIN  1  // Margin from right side of display in pixels
#define MAX_NUM_CHARS 5  // Max num of chars = 5, eg. 80/40, 1000W
#define TEXT_BOX_WIDTH ( MAX_NUM_CHARS * CHAR_WIDTH + MAX_NUM_CHARS - 1 ) // 5*5 + (5-1) = 25 + 4 = 29
#define TEXT_BOX_X ( MATRIX_WIDTH - RIGHT_MARGIN - TEXT_BOX_WIDTH )
#define ROW_SEPARATION 10  // Space between displayed rows
#define FIRST_ROW_Y 1  // Y-value of the first displayed row, 0,0 is top left of display
#define SECOND_ROW_Y FIRST_ROW_Y  +  5  // Y-value of the 2nd displayed row. Offset approximately 2/3 down from top of first row
#define THIRD_ROW_Y  SECOND_ROW_Y +  ROW_SEPARATION  // Y-value of the 2nd displayed row
#define FOURTH_ROW_Y THIRD_ROW_Y  +  ROW_SEPARATION  // Y-value of the 3rd displayed row
#define FIFTH_ROW_Y FOURTH_ROW_Y  +  ROW_SEPARATION  // Y-value of the 4th displayed row
#define SIXTH_ROW_Y FIFTH_ROW_Y   +  ROW_SEPARATION  // Y-value of the 5th displayed row
#define SEVENTH_ROW_Y SIXTH_ROW_Y +  ROW_SEPARATION  // Y-value of the 6th displayed row


extern MatrixPanel_I2S_DMA matrix;  // Matrix instantiation


// Function prototypes
void setup_Serial();
void setup_LED_Display();
uint8_t calculateTextWidth(String text);
void query_Radio(HardwareSerial &radio, uint8_t station_num);
void setup_Digital_Pins();
void setup_PWM();


#endif