#ifndef Display_h
#define Display_h

#include "Constants.h"
#include "ScreenDriver.h"

//#define LCD_XSIZE_TFT    800
// #define LCD_YSIZE_TFT    480

int screenHeight = LCD_YSIZE_TFT; 
int screenWidth = LCD_XSIZE_TFT;
int multiplier = 6;

void drawSquare(int x, int y, int width, int height, int colour) {
  ER5517.DrawSquare_Fill(x, y, 
                         width + x, 
                         y + height, 
                         colour
                         );
}

int adjustCoordX(float oldXCoord) {
  float newXCoord = oldXCoord * multiplier;
  return newXCoord;
}

int adjustCoordY(float oldYCoord) {
  float newYCoord = screenHeight - oldYCoord * multiplier;
  return newYCoord;
}

void drawFigure(int x, int y, int width, int height, int colour) {
  drawSquare(
    adjustCoordX(x),
    adjustCoordY(y),
    width * multiplier,
    height * multiplier,
    colour
  );
}

#endif
