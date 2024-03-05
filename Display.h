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
                         width + x - 1, 
                         y + height - 1, 
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

void drawGrid() {
  for(int w = 0; w <= gridXLimit * multiplier; w += multiplier) {
    drawSquare(w, 0, 1, gridYLimit * multiplier, White);
  }
  for(int h = 0; h <= gridYLimit * multiplier; h += multiplier) {
    drawSquare(0, h, gridXLimit * multiplier, 1, White);
  }
  drawSquare(0, LCD_YSIZE_TFT-1, gridXLimit * multiplier, 1, White);
}

#endif
