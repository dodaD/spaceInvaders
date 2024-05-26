#ifndef Display_h
#define Display_h

#include "Constants.h"
#include "ScreenDriver.h"

//#define LCD_XSIZE_TFT    800
// #define LCD_YSIZE_TFT    480

int multiplier = 6;
int previousAmountOfLives = 0; //To optimize re-drawing of ships
//that symbolize lifes left
int maxWidth = 10; // Max width when drawing a monster
int onePx = (sideOfMonster * multiplier) / maxWidth; 

int adjustCoordX(float oldXCoord) {
  float newXCoord = oldXCoord * multiplier;

  return newXCoord;
}

int adjustCoordY(float oldYCoord) {
  float newYCoord = LCD_YSIZE_TFT - oldYCoord * multiplier;

  return newYCoord;
}

void drawFigure(int x, int y, int width, int height, int colour) {
  if (x >= gridXLimit
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
                      (gridYLimit * multiplier) / 2 - 66);//Half of the text width
  ER5517.Foreground_color_65k(Green);
  ER5517.Show_String("GAME OVER!"); 

  ER5517.Font_Select_12x24_24x24();
  ER5517.Goto_Text_XY((gridXLimit / 2 - 38) * multiplier, 
                      (gridYLimit / 2) * multiplier);
  ER5517.Show_String("To restart the game, press green button"); 
  ER5517.Goto_Text_XY((gridXLimit / 2 - 30) * multiplier, 
                      (gridYLimit / 2 + 5) * multiplier);
  ER5517.Show_String("To quit, press yellow button"); 
}

void drawLoserText() {
  drawGameOverText();
  ER5517.Foreground_color_65k(Red);
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit / 2 - 15) * multiplier, 
                      (gridYLimit * multiplier) / 2 - 30);
  ER5517.Show_String("You have lost!"); 
}

void drawWinningText(int bonusPointsForSpeed, int bonusPointsForLivesLeft) {
  drawGameOverText();
  ER5517.Foreground_color_65k(Yellow);
  ER5517.Font_Select_16x32_32x32();
  ER5517.Goto_Text_XY((gridXLimit / 2 - 10) * multiplier, 
                      (gridYLimit * multiplier) / 2 - 30);
  ER5517.Show_String("You won!");
  char newBonusPointsForSpeed[8];
  char newBonusPointsForLivesLeft[3];

  String tempBonusPointsForSpeed = String(bonusPointsForSpeed);
  String tempBonusPointsForLivesLeft = String(bonusPointsForLivesLeft);
  tempBonusPointsForSpeed.toCharArray(newBonusPointsForSpeed, 8); 
  tempBonusPointsForLivesLeft.toCharArray(newBonusPointsForLivesLeft, 3); 
  char* pointerToBonusPointsForSpeed = newBonusPointsForSpeed;
  char* pointerToBonusPointsForLivesLeft = newBonusPointsForLivesLeft;

  ER5517.Font_Select_12x24_24x24();
  ER5517.Goto_Text_XY((gridXLimit / 2 - 30) * multiplier, 
                      (gridYLimit / 2 + 10) * multiplier);
  ER5517.Foreground_color_65k(Green);
  ER5517.Show_String("Additional points - speed: +");
  ER5517.Show_String(pointerToBonusPointsForSpeed);
  ER5517.Goto_Text_XY((gridXLimit / 2 - 34) * multiplier, 
                      (gridYLimit / 2 + 15) * multiplier);
  ER5517.Show_String("Additional points - lives left: +");
  ER5517.Show_String(pointerToBonusPointsForLivesLeft);
}

void drawMonster1Base (int x, int y, int color) {
  if (x > gridXLimit
    || y > gridYLimit
    || x <  0 - sideOfMonster
    || y < sideOfMonster
  ) {
    return;
  }

  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);

  ER5517.DrawSquare_Fill(newX + (onePx * 4),
                         newY,
                         (newX + (onePx * 4)) + onePx * 2 - 1,
                         newY + onePx,
                         color);

  ER5517.DrawSquare_Fill(newX + (onePx * 3), //Marging from the widest part
                         newY + onePx ,
                         (newX + (onePx * 3)) + (onePx * 4) - 1, //Start + width
                         newY + onePx * 2, //Start + height
                         color);

  ER5517.DrawSquare_Fill(newX + (onePx * 1),
                         newY + onePx * 2,
                         (newX + (onePx * 1)) + (onePx * 8) - 1,
                         newY + onePx * 3,
                         color);

  ER5517.DrawSquare_Fill(newX,
                         newY + onePx * 3,
                         newX + (onePx * maxWidth) - 1,
                         newY + onePx * 7,
                         color);
}

void reDrawMonster1 (int x, int y, char movementDirection, int colorOfMonster) {
  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);
  int onePxThatNeedsToBeReDrawn = moveDistanceForMonsters * multiplier;
  int startPointX = newX + (onePx * 4);

   if (movementDirection == 'R') {
    ER5517.DrawSquare_Fill(startPointX,
                           newY,
                           startPointX + onePxThatNeedsToBeReDrawn,
                           newY + onePx - 1,
                           Black); 

    startPointX = startPointX + onePx * 2; 
    ER5517.DrawSquare_Fill(startPointX,
                           newY,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx,
                           colorOfMonster); 

    startPointX = newX + (onePx * 3); 
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx,
                           startPointX + onePxThatNeedsToBeReDrawn,
                           newY + onePx * 2 - 1,
                           Black);

    startPointX = startPointX + (onePx * 4); 
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 2,
                           colorOfMonster);

    startPointX = newX + onePx;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 2,
                           startPointX + onePxThatNeedsToBeReDrawn,
                           newY + onePx * 3 - 1,
                           Black);

    startPointX = startPointX + (onePx * 8);
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 2,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 3,
                           colorOfMonster);

    startPointX = newX;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 3,
                           startPointX + onePxThatNeedsToBeReDrawn,
                           newY + onePx * 7,
                           Black);

    startPointX = startPointX + (onePx * maxWidth);
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 3,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 7,
                           colorOfMonster);
    return;   
  }

  if (movementDirection == 'L') {
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY,
                           startPointX,
                           newY + onePx,
                           colorOfMonster); 

    startPointX = startPointX + onePx * 2; 
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY,
                           startPointX,
                           newY + onePx,
                           Black); 

    startPointX = newX + (onePx * 3); 
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY + onePx,
                           startPointX,
                           newY + onePx * 2,
                           colorOfMonster);

    startPointX = startPointX + (onePx * 4); 
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY + onePx,
                           startPointX,
                           newY + onePx * 2,
                           Black);

    startPointX = newX + onePx;
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY + onePx * 2,
                           startPointX,
                           newY + onePx * 3,
                           colorOfMonster);

    startPointX = startPointX + (onePx * 8);
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY + onePx * 2,
                           startPointX,
                           newY + onePx * 3,
                           Black);

    startPointX = newX;
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY + onePx * 3,
                           startPointX,
                           newY + onePx * 7,
                           colorOfMonster);

    startPointX = startPointX + (onePx * maxWidth);
    ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                           newY + onePx * 3,
                           startPointX,
                           newY + onePx * 7,
                           Black);
    return;
  }

}

void drawMonster1Animation (int x, int y, int color, char typeOfAnimation) {
  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y); 

  if (color == Black) { //DRAWING EYES
    ER5517.DrawSquare_Fill(newX + onePx * 1,
                           newY + onePx * 4,
                           (newX + onePx * 1) + onePx * 3,
                           newY + onePx * 5,
                           White);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                           newY + onePx * 4,
                           (newX + onePx * (maxWidth - 4)) + onePx * 3,
                           newY + onePx * 5,
                           White);
  } else {
    ER5517.DrawSquare_Fill(newX + onePx * 1,
                           newY + onePx * 4,
                           (newX + onePx * 1) + onePx * 3,
                           newY + onePx * 5,
                           Black);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                           newY + onePx * 4,
                           (newX + onePx * (maxWidth - 4)) + onePx * 3,
                           newY + onePx * 5,
                           Black);
  } // WARNING: We need to find a better way to do this


  ER5517.DrawSquare_Fill(newX + onePx * 1,
                         newY + onePx * 7,
                         (newX + onePx * 1) + (onePx * 2),
                         newY + onePx * 8,
                         color
                         );

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                         newY + onePx * 7,
                         (newX + onePx * (maxWidth - 3)) + (onePx * 2),
                         newY + onePx * 8,
                         color);

 ER5517.DrawSquare_Fill(newX + onePx * 1,
                         newY + onePx * 9,
                         (newX + onePx * 1) + (onePx * 2),
                         newY + onePx * 10, 
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                         newY + onePx * 9,
                         (newX + onePx * (maxWidth - 3)) + (onePx * 2),
                         newY + onePx * 10,
                         color);

  if(typeOfAnimation == 'C') { 
    ER5517.DrawSquare_Fill(newX,
                           newY + onePx * 8,
                           newX + (onePx * 2),
                           newY + onePx * 9,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 2),
                           newY + onePx * 8,
                           (newX + onePx * (maxWidth - 2)) + (onePx * 2),
                           newY + onePx * 9,
                           color);
    return;
  } 

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx * 8,
                         (newX + onePx * 2) + (onePx * 2),
                         newY + onePx * 9,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                         newY + onePx * 8,
                         (newX + onePx * (maxWidth - 4)) + (onePx * 2),
                         newY + onePx * 9,
                         color);

}

void drawMonster2Base (int x, int y, int color) { 
  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);  

  ER5517.DrawSquare_Fill(newX + onePx,
                         newY,
                         (newX + onePx) + onePx,
                         newY + onePx,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 2),
                         newY,
                         (newX + onePx * (maxWidth - 2)) + onePx,
                         newY + onePx,
                         color); 

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx,
                         (newX + onePx * 2) + onePx,
                         newY + onePx * 2,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                         newY + onePx,
                         (newX + onePx * (maxWidth - 3)) + onePx,
                         newY + onePx * 2,
                         color); 

  ER5517.DrawSquare_Fill(newX + onePx * 3,
                         newY + onePx * 2,
                         (newX + onePx * 3) + onePx,
                         newY + onePx * 3,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                         newY + onePx * 2,
                         (newX + onePx * (maxWidth - 4)) + onePx,
                         newY + onePx * 3,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 3,
                         newY + onePx * 3,
                         (newX + onePx * 3) + onePx * 4,
                         newY + onePx * 4,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx * 4,
                         (newX + onePx * 2) + onePx * 6,
                         newY + onePx * 5,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx,
                         newY + onePx * 5,
                         (newX + onePx) + onePx * 8,
                         newY + onePx * 7,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx * 7,
                         newX + onePx * (maxWidth - 2),
                         newY + onePx * 8,
                         color);

}

void reDrawMonster2 (int x, int y, char movementDirection, int colorOfMonster) {
  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);  
  int onePxThatNeedsToBeReDrawn = moveDistanceForMonsters * multiplier;
  int startPointX = newX + onePx * 3;

  if (movementDirection == 'R') {
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 3,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 4,
                           Black);

    startPointX = startPointX + onePx * 4;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 3,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 4,
                           colorOfMonster);

    startPointX = newX + onePx * 2;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 4 - 1,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 5,
                           Black);

    startPointX = startPointX + onePx * 6;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 4 - 1,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 5,
                           colorOfMonster);

    startPointX = newX + onePx;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 5 - 1,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 7,
                           Black);

    startPointX = startPointX + onePx * 8;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 5 - 1,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 7,
                           colorOfMonster);

    startPointX = newX + onePx * 2;
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 7 - 1,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 8,
                           Black);

    startPointX = newX + onePx * (maxWidth - 2);
    ER5517.DrawSquare_Fill(startPointX,
                           newY + onePx * 7 - 1,
                           startPointX + onePxThatNeedsToBeReDrawn - 1,
                           newY + onePx * 8,
                           colorOfMonster);
    return;
  }

  //****** LEFT MOVEMENT ********
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 3,
                         startPointX,
                         newY + onePx * 4,
                         colorOfMonster);

  startPointX = startPointX + onePx * 4;
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 3,
                         startPointX,
                         newY + onePx * 4,
                         Black);

  startPointX = newX + onePx * 2;
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 4 - 1,
                         startPointX,
                         newY + onePx * 5,
                         colorOfMonster);

  startPointX = startPointX + onePx * 6;
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 4 - 1,
                         startPointX,
                         newY + onePx * 5,
                         Black);

  startPointX = newX + onePx;
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 5 - 1,
                         startPointX,
                         newY + onePx * 7,
                         colorOfMonster);

  startPointX = startPointX + onePx * 8;
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 5 - 1,
                         startPointX,
                         newY + onePx * 7,
                         Black);

  startPointX = newX + onePx * 2;
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 7 - 1,
                         startPointX,
                         newY + onePx * 8 - 1,
                         colorOfMonster);

  startPointX = newX + onePx * (maxWidth - 2);
  ER5517.DrawSquare_Fill(startPointX - onePxThatNeedsToBeReDrawn,
                         newY + onePx * 7 - 1,
                         startPointX,
                         newY + onePx * 8,
                         Black);
 }

void drawMonster2Animation (int x, int y, int color, char typeOfAnimation) { 
  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);

  if (color == Black) { //DRAWING EYES
    ER5517.DrawSquare_Fill(newX + onePx * 3, //EYES
                           newY + onePx * 5,
                           (newX + onePx * 3) + onePx * 1,
                           newY + onePx * 6,
                           White);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                           newY + onePx * 5,
                           (newX + onePx * (maxWidth - 4)) + onePx * 1,
                           newY + onePx * 6,
                           White);
  } else {
    ER5517.DrawSquare_Fill(newX + onePx * 3, //EYES
                           newY + onePx * 5,
                           (newX + onePx * 3) + onePx * 1,
                           newY + onePx * 6,
                           Black);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                           newY + onePx * 5,
                           (newX + onePx * (maxWidth - 4)) + onePx * 1,
                           newY + onePx * 6,
                           Black);
  }

  //Antennas
  ER5517.DrawSquare_Fill(newX + onePx,
                         newY,
                         (newX + onePx) + onePx,
                         newY + onePx,
                         color); //D

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 2),
                         newY,
                         (newX + onePx * (maxWidth - 2)) + onePx,
                         newY + onePx,
                         color); 

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx,
                         (newX + onePx * 2) + onePx,
                         newY + onePx * 2,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                         newY + onePx,
                         (newX + onePx * (maxWidth - 3)) + onePx,
                         newY + onePx * 2,
                         color); 

  ER5517.DrawSquare_Fill(newX + onePx * 3,
                         newY + onePx * 2,
                         (newX + onePx * 3) + onePx,
                         newY + onePx * 3,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                         newY + onePx * 2,
                         (newX + onePx * (maxWidth - 4)) + onePx,
                         newY + onePx * 3,
                         color);

  if (typeOfAnimation == 'C') {
    ER5517.DrawSquare_Fill(newX,
                           newY + onePx * 6,
                           newX + onePx - 1,
                           newY + onePx * 9 - 1,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 1),
                           newY + onePx * 6,
                           (newX + onePx * (maxWidth - 1)) + onePx - 1,
                           newY + onePx * 9 - 1,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * 2,
                           newY + onePx * 8,
                           (newX + onePx * 2) + onePx - 1,
                           newY + onePx * 9 - 1,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                           newY + onePx * 8,
                           (newX + onePx * (maxWidth - 3)) + onePx - 1,
                           newY + onePx * 9 - 1,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * 3,
                           newY + onePx * 9,
                           (newX + onePx * 3) + onePx - 1,
                           newY + onePx * 10 - 1,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                           newY + onePx * 9,
                           (newX + onePx * (maxWidth - 4)) + onePx - 1,
                           newY + onePx * 10 - 1,
                           color);
    return;
  }

  ER5517.DrawSquare_Fill(newX,
                         newY + onePx * 3,
                         newX + onePx - 1,
                         newY + onePx * 6 - 1,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 1),
                         newY + onePx * 3,
                         (newX + onePx * (maxWidth - 1)) + onePx - 1,
                         newY + onePx * 6 - 1,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx * 8,
                         (newX + onePx * 2) + onePx - 1,
                         newY + onePx * 9 - 1,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                         newY + onePx * 8,
                         (newX + onePx * (maxWidth - 3)) + onePx - 1,
                         newY + onePx * 9 - 1,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx,
                         newY + onePx * 9,
                         (newX + onePx) + onePx - 1,
                         newY + onePx * 10 - 1,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 2),
                         newY + onePx * 9,
                         (newX + onePx * (maxWidth - 2)) + onePx - 1,
                         newY + onePx * 10 - 1,
                         color);
}

void drawMonster3Base (int x, int y, int color) {
  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);

  ER5517.DrawSquare_Fill(newX + onePx * 5,
                         newY,
                         (newX + onePx * 5) + onePx * 1,
                         newY + onePx,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 4,
                         newY + onePx,
                         (newX + onePx * 4) + onePx * 2,
                         newY + onePx * 2,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx * 2,
                         (newX + onePx * 2) + onePx * 6,
                         newY + onePx * 3,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx,
                         newY + onePx * 3,
                         (newX + onePx) + onePx * 8,
                         newY + onePx * 4,
                         color);

  ER5517.DrawSquare_Fill(newX,
                         newY + onePx * 4,
                         newX + onePx * 10,
                         newY + onePx * 7,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx * 5,
                         (newX + onePx * 2) + onePx * 2,
                         newY + onePx * 6,
                         Black);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                         newY + onePx * 5,
                         (newX + onePx * (maxWidth - 4)) + onePx * 2,
                         newY + onePx * 6,
                         Black);
}

void drawMonster3Animation (int x, int y, int color, char typeOfAnimation) { 
  int newX = adjustCoordX(x);
  int newY = adjustCoordY(y);
  if (typeOfAnimation == 'C') {
    if (color != Black) {
      drawMonster3Animation(x, y, Black, 'O');
    }
    ER5517.DrawSquare_Fill(newX + onePx * 2,
                           newY + onePx * 7,
                           (newX + onePx * 2) + onePx,
                           newY + onePx * 8,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                           newY + onePx * 7,
                           (newX + onePx * (maxWidth - 3)) + onePx,
                           newY + onePx * 8,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * 4,
                           newY + onePx * 7,
                           (newX + onePx * 4) + onePx * 2,
                           newY + onePx * 8,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx,
                           newY + onePx * 8,
                           (newX + onePx) + onePx,
                           newY + onePx * 9,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 2),
                           newY + onePx * 8,
                           (newX + onePx * (maxWidth - 2)) + onePx,
                           newY + onePx * 9,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * 2,
                           newY + onePx * 9,
                           (newX + onePx * 2) + onePx,
                           newY + onePx * 10,
                           color);

    ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                           newY + onePx * 9,
                           (newX + onePx * (maxWidth - 3)) + onePx,
                           newY + onePx * 10,
                           color);
    return;
  }
  if (color != Black) {
    drawMonster3Animation(x, y, Black, 'C');
  }
  ER5517.DrawSquare_Fill(newX + onePx * 3,
                         newY + onePx * 7,
                         (newX + onePx * 3) + onePx,
                         newY + onePx * 8,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                         newY + onePx * 7,
                         (newX + onePx * (maxWidth - 4)) + onePx,
                         newY + onePx * 8,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 4,
                         newY + onePx * 8,
                         (newX + onePx * 4) + onePx * 2,
                         newY + onePx * 9,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 2,
                         newY + onePx * 8,
                         (newX + onePx * 2) + onePx,
                         newY + onePx * 9,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 3),
                         newY + onePx * 8,
                         (newX + onePx * (maxWidth - 3)) + onePx,
                         newY + onePx * 9,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * 3,
                         newY + onePx * 9,
                         (newX + onePx * 3) + onePx,
                         newY + onePx * 10,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 4),
                         newY + onePx * 9,
                         (newX + onePx * (maxWidth - 4)) + onePx,
                         newY + onePx * 10,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx,
                         newY + onePx * 9,
                         (newX + onePx) + onePx,
                         newY + onePx * 10,
                         color);

  ER5517.DrawSquare_Fill(newX + onePx * (maxWidth - 2),
                         newY + onePx * 9,
                         (newX + onePx * (maxWidth - 2)) + onePx,
                         newY + onePx * 10,
                         color);

}


#endif
