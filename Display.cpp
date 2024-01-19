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
  int dimension1 = rows;
  int dimension2 = columns;
  int width = sideOfMonster;
  int height = sideOfMonster;

  if (figure == "bullet") {
    dimension1 = columns;
    dimension2 = bulletsPerColumn;
    width = widthOfBullet;
    height = heightOfBullet;
  }

  for(int d1 = 0; d1 < dimension1; d1++) {
    for(int d2 = 0; d2 < dimension2; d2++) {
      drawSquare(
        adjustCoordX(array[d1][d2][xCoord]),
        adjustCoordY(array[d1][d2][yCoord]),
        adjustCoordX(width),
        adjustCoordY(height),
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
