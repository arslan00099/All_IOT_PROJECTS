// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607
// 32x32 MATRICES DO NOT WORK WITH ARDUINO UNO or METRO 328.

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).


#define CLK  15   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   33
#define LAT 32
#define A   12
#define B   16
#define C   17
#define D   18

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);


void setupPanel() {

  matrix.begin();

  // // draw a pixel in solid white
  // matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  // delay(500);

  // // fix the screen with green
  // matrix.fillRect(0, 0, 32, 32, matrix.Color333(0, 7, 0));
  // delay(500);

  // // draw a box in yellow
  // matrix.drawRect(0, 0, 32, 32, matrix.Color333(7, 7, 0));
  // delay(500);

  // // draw an 'X' in red
  // matrix.drawLine(0, 0, 31, 31, matrix.Color333(7, 0, 0));
  // matrix.drawLine(31, 0, 0, 31, matrix.Color333(7, 0, 0));
  // delay(500);

  // // draw a blue circle
  // matrix.drawCircle(10, 10, 10, matrix.Color333(0, 0, 7));
  // delay(500);

  // // fill a violet circle
  // matrix.fillCircle(21, 21, 10, matrix.Color333(7, 0, 7));
  // delay(500);

  // // fill the screen with 'black'
  // matrix.fillScreen(matrix.Color333(0, 0, 0));

  // // draw some text!
  // matrix.setCursor(1, 0);    // start at top left, with one pixel of spacing
  // matrix.setTextSize(1);     // size 1 == 8 pixels high
  // matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  // matrix.setTextColor(matrix.Color333(7,7,7));
  // matrix.println(" Ada");
  // matrix.println("fruit");

  // // print each letter with a rainbow color
  // matrix.setTextColor(matrix.Color333(7,0,0));
  // matrix.print('3');
  // matrix.setTextColor(matrix.Color333(7,4,0));
  // matrix.print('2');
  // matrix.setTextColor(matrix.Color333(7,7,0));
  // matrix.print('x');
  // matrix.setTextColor(matrix.Color333(4,7,0));
  // matrix.print('3');
  // matrix.setTextColor(matrix.Color333(0,7,0));
  // matrix.println('2');

  // matrix.setTextColor(matrix.Color333(0,7,7));
  // matrix.print('*');
  // matrix.setTextColor(matrix.Color333(0,4,7));
  // matrix.print('R');
  // matrix.setTextColor(matrix.Color333(0,0,7));
  // matrix.print('G');
  // matrix.setTextColor(matrix.Color333(4,0,7));
  // matrix.print('B');
  // matrix.setTextColor(matrix.Color333(7,0,4));
  // matrix.print('*');

  // // whew!
}

void displayMatrix(int x,int y,int fontSize, uint16_t color, String text)
{

   // // draw some text!
   matrix.setCursor(x, y);    // start at top left, with one pixel of spacing
   matrix.setTextSize(fontSize);     // size 1 == 8 pixels high
   matrix.setTextColor(color);
   matrix.print(text);
}

void clearMatrix()
{
   matrix.fillRect(0,0,64,17,matrix.Color333(0, 0, 0));

}

