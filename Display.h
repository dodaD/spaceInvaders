#ifndef Display_h
#define Display_h

#include "Constants.h"
#include "ScreenDriver.h"

//#define LCD_XSIZE_TFT    800
// #define LCD_YSIZE_TFT    480

int multiplier = 6;

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

void drawScore(int score) {
  ER5517.DrawSquare_Fill(gridXLimit * multiplier + 1, 
                         0, 
                         gridXLimit * multiplier + 2, 
                         LCD_YSIZE_TFT - 1, 
                         Green);
  char scoreStr = score;
  ER5517.Goto_Text_XY(700, 100);
  ER5517.Foreground_color_65k(Green);
  //ER5517.Show_String("S"); 
  //ER5517.Show_String(scoreStr);
}

void drawGameOverText() {
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit * multiplier) / 2 - 72, 
                      (gridYLimit * multiplier) / 2 - 30);//Half of the text width
  ER5517.Foreground_color_65k(Green);
  ER5517.Show_String("G"); 
  ER5517.Show_String("A"); 
  ER5517.Show_String("M"); 
  ER5517.Show_String("E"); 
  ER5517.Show_String(" "); 
  ER5517.Show_String("O"); 
  ER5517.Show_String("V"); 
  ER5517.Show_String("E"); 
  ER5517.Show_String("R");

  ER5517.Font_Select_12x24_24x24();
  ER5517.Goto_Text_XY((gridXLimit / 2 - 19) * multiplier, 
                      (gridYLimit / 2) * multiplier);
  ER5517.Show_String("R"); 
  ER5517.Show_String("e"); 
  ER5517.Show_String("s"); 
  ER5517.Show_String("t"); 
  ER5517.Show_String("a"); 
  ER5517.Show_String("r"); 
  ER5517.Show_String("t"); 
  ER5517.Show_String(" "); 
  ER5517.Show_String("i"); 
  ER5517.Show_String("n"); 
  ER5517.Show_String(" "); 
  ER5517.Show_String("3"); 
  ER5517.Show_String("."); 
  ER5517.Show_String("."); 
  ER5517.Show_String("."); 
  ER5517.Show_String("2"); 
  ER5517.Show_String("."); 
  ER5517.Show_String("."); 
  ER5517.Show_String("."); 
  ER5517.Show_String("1"); 
}

void drawLoserText() {
  drawGameOverText();
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit * multiplier) / 2 - 72, 
                      (gridYLimit * multiplier) / 2 - 66);
  ER5517.Show_String("Y"); 
  ER5517.Show_String("o"); 
  ER5517.Show_String("u"); 
  ER5517.Show_String(" "); 
  ER5517.Show_String("l"); 
  ER5517.Show_String("o"); 
  ER5517.Show_String("s"); 
  ER5517.Show_String("t"); 
  ER5517.Show_String("!");
  int exampleValue = 13;
  char newChar[2];

  String temp_str = String(exampleValue);
  temp_str.toCharArray(newChar,2); 
  char (*p)[2]; 
  p = &newChar;
  ER5517.Show_String(p[0]);
}

void drawWinningText() {
  drawGameOverText();
  ER5517.Foreground_color_65k(Yellow);
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit * multiplier) / 2 - 72, 
                      (gridYLimit * multiplier) / 2 - 66);
  ER5517.Show_String("~");
  ER5517.Show_String("Y"); 
  ER5517.Show_String("o"); 
  ER5517.Show_String("u"); 
  ER5517.Show_String(" "); 
  ER5517.Show_String("w"); 
  ER5517.Show_String("o"); 
  ER5517.Show_String("n"); 
  ER5517.Show_String("!");
}

#endif
