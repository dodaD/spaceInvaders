#include "Constants.h"
#include "ScreenDriver.h"

void drawSquare(int xCoord, int yCoord, int width, int height, int colour) {
  ER5517.DrawSquare_Fill(xCoord, yCoord, 
                         width + xCoord, 
                         height + yCoord, 
                         colour
                         );
}

void drawFigures(int (*array), int colour, string figure) {
  bool isBullet = figure == "bullet";
  for (auto d1: array) {
    for (auto d2: d1) { // WARNING: check if auto is correct
      drawSquare(
        adjustCoordX(array[d1][d2][xCoord]),
        adjustCoordY(array[d1][d2][yCoord]),
        adjustCoordX(isBullet ? widthOfBullet : sideOfMonster),
        adjustCoordY(isBullet ? heightOfBullet : sideOfMonster),
        colour
      );
    }
  }
}

int adjustCoordX(float oldXCoord) {
  float newXCoord = (oldXCoord / 70.0 * LCD_XSIZE_TFT);
  return newXCoord;
}

int adjustCoordY(float oldYCoord) {
  float newYCoord = LCD_YSIZE_TFT - (oldYCoord / 100.0 * LCD_YSIZE_TFT);
  return newYCoord;
}
