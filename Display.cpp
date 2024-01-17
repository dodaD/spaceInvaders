#include "Constants.h"
#include "ScreenDriver.h"

void drawFigure(int xCoord, int yCoord, int width, int height, int colour) {
  ER5517.DrawSquare_Fill(xCoord, yCoord, 
                         width + xCoord, 
                         height + yCoord, 
                         colour
                         );
}

int adjustCoordX(float oldXCoord) {
  float newXCoord = (oldXCoord / 70.0 * LCD_XSIZE_TFT);
  return newXCoord;
}

int adjustCoordY(float oldYCoord) {
  float newYCoord = LCD_YSIZE_TFT - (oldYCoord / 100.0 * LCD_YSIZE_TFT);
  return newYCoord;
}

