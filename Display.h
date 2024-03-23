#ifndef Display_h
#define Display_h

#include "Constants.h"
#include "ScreenDriver.h"

//#define LCD_XSIZE_TFT    800
// #define LCD_YSIZE_TFT    480

int multiplier = 6;
int previousAmountOfLives = 0; //To optimize re-drawing of ships that symbolize lifes left

int adjustCoordX(float oldXCoord) {
  float newXCoord = oldXCoord * multiplier;

  return newXCoord;
}

int adjustCoordY(float oldYCoord) {
  float newYCoord = LCD_YSIZE_TFT - oldYCoord * multiplier;

  return newYCoord;
}

void drawFigure(int x, int y, int width, int height, int colour) {
  if (x > gridXLimit
    || y > gridYLimit
    || x <  0 - width
    || y < height
  ) {
    return;
  }

  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);
  int corectWidth = width * multiplier + newX - 1;
  int correctHeight = height * multiplier + newY - 1;

  corectWidth = corectWidth > gridXLimit * multiplier 
    ? gridXLimit * multiplier
    : width * multiplier + newX - 1;

  correctHeight = correctHeight < 0  
    ? 0
    : height * multiplier + newY - 1;

  if(x < 0) {
    corectWidth = (width + x) * multiplier;
    newX = 0;
  }

  ER5517.DrawSquare_Fill(
    newX,
    newY,
    corectWidth,
    correctHeight,
    colour
  );
}

void drawGrid() {
  for(int w = 0; w <= gridXLimit * multiplier; w += multiplier) {
    ER5517.DrawSquare_Fill(w, 0, w, gridYLimit * multiplier - 1, White);
  }
  for(int h = 0; h <= gridYLimit * multiplier; h += multiplier) {
    ER5517.DrawSquare_Fill(0, h, gridXLimit * multiplier - 1, h, White);
  }
  ER5517.DrawSquare_Fill(0, 
             LCD_YSIZE_TFT - 1, 
             gridXLimit * multiplier - 1, 
             LCD_YSIZE_TFT - 1, 
             White);
}

drawStats(int score, int lifes) {
  ER5517.Foreground_color_65k(Green);
  ER5517.Font_Select_12x24_24x24();
  ER5517.Goto_Text_XY(500, 57);
  char newScore[8];

  String tempScore = String(score);
  tempScore.toCharArray(newScore, 8); 
  char* pointerToScore = newScore;

  ER5517.Show_String("Score: "); 
  ER5517.Show_String(pointerToScore); // SEE MAX VALUE OF INT!!!

  if(previousAmountOfLives == lifes) {
    return;
  }
  int width = 39;
  int height = 15;
  int startPointX = 500;
  int startPointY = 30;
  int gap = 5;

  for(int l = 0; l < previousAmountOfLives; l++) {
    ER5517.DrawSquare_Fill(
      startPointX + (width + gap) * l,
      startPointY,
      startPointX + width + (width + gap) * l,
      startPointY + height,
      Black
    ); //Draw mini-ships to represent lives left
  }

  for(int l = 0; l < lifes; l++) {
    ER5517.DrawSquare_Fill(
      startPointX + (width + gap) * l,
      startPointY,
      startPointX + width + (width + gap) * l,
      startPointY + height,
      Green
    ); //Draw mini-ships to represent lives left
  }
  previousAmountOfLives = lifes;
}

void drawGameOverText() {
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit * multiplier) / 2 - 72, 
                      (gridYLimit * multiplier) / 2 - 30);//Half of the text width
  ER5517.Foreground_color_65k(Green);
  ER5517.Show_String("GAME OVER!"); 

  ER5517.Font_Select_12x24_24x24();
  ER5517.Goto_Text_XY((gridXLimit / 2 - 19) * multiplier, 
                      (gridYLimit / 2) * multiplier);
  ER5517.Show_String("Restart?"); 
}

void drawLoserText() {
  drawGameOverText();
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit * multiplier) / 2 - 72, 
                      (gridYLimit * multiplier) / 2 - 66);
  ER5517.Show_String("You have lost!"); 
}

void drawWinningText() {
  drawGameOverText();
  ER5517.Foreground_color_65k(Yellow);
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit * multiplier) / 2 - 72, 
                      (gridYLimit * multiplier) / 2 - 66);
  ER5517.Show_String("You won!");
}

#endif
