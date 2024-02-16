#ifndef Display_h
#define Display_h

#include "Constants.h"
#include "ScreenDriver.h"

void drawSquare(int x, int y, int width, int height, int colour) {
  ER5517.DrawSquare_Fill(x, y, 
                         width + x, 
                         y + height, 
                         colour
                         );
}

int adjustCoordX(float oldXCoord) {
  float newXCoord = (oldXCoord * LCD_XSIZE_TFT / gridXLimit);
  return newXCoord;
}

int adjustCoordY(float oldYCoord) {
  float newYCoord = LCD_YSIZE_TFT - (oldYCoord * LCD_YSIZE_TFT / gridYLimit);
  return newYCoord;
}

void drawFigure(int x, int y, int width, int height, int colour) {
  drawSquare(
    adjustCoordX(x),
    adjustCoordY(y),
    width * LCD_XSIZE_TFT / gridXLimit,
    height * LCD_XSIZE_TFT / gridYLimit,
    colour
  );
}

#endif
