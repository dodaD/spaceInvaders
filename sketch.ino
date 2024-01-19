#include "BulletClass.h"
#include "MonsterClass.h"
#include "Constants.h"
#include "ScreenDriver.h"
#include "Display.cpp"

Monster allMonsters[rows][columns];
Bullet allBullets[columns][bulletsPerColumn];
char monstersMovingDirection = 'L'; 
//TODO: google about enum
int monstersColumns[columns];
int allMonstersCoords[rows][columns][2];
int allBulletsCoords[columns][bulletsPerColumn][2];
int spaceShipBulletCoord[2] = {35, 0};
Bullet spaceShipBullet = Bullet;
unsigned long previousMillis;

void setup() {
  allBullets[numberOfBullets-1] = Bullet(35, 0, true); // TODO: rename
  Serial.begin(9600);
  pinMode(5,   OUTPUT);
  digitalWrite(5, HIGH);//Disable  SD 
  pinMode(2,   OUTPUT);
  digitalWrite(2, HIGH);//Disable  RTP   
  
  // WARNING: ya hz
  ER5517.Parallel_Init();
  ER5517.HW_Reset();
  ER5517.System_Check_Temp();
  delay(100);
  while(ER5517.LCD_StatusRead()&0x02);
  ER5517.initial();
  ER5517.Display_ON();

  ER5517.Select_Main_Window_16bpp();
  ER5517.Main_Image_Start_Address(layer1_start_addr);        
  ER5517.Main_Image_Width(LCD_XSIZE_TFT);
  ER5517.Main_Window_Start_XY(0,0);
  ER5517.Canvas_Image_Start_address(0);
  ER5517.Canvas_image_width(LCD_XSIZE_TFT);
  ER5517.Active_Window_XY(0,0);
  ER5517.Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT); 
  ER5517.DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Black);
  // end of ya hz
  
  createMonstersAndBullets(); 
}

void loop() { // TODO: make it work mb idk
  drawFigures(&allBulletsCoords, Black, "bullet");
  //drawBullets(Black); //erase Monsters ^^^^^^^^^
  //drawMonsters(Black); //erase old one
  bulletsMove();
  //drawBullets(White); //draw new one

  checkDirectionOfMonsters();
  monstersReachedScreenEnd();
  
  for(int r = 0; r < rows; r++) {
    for(int c = 0; c < columns; c++) {
      moveMonster(r, c); // TODO: be continued...
      bool haveCrushed = checkCollision(allMonstriks[r][c]);
      if(haveCrushed) {
          allMonstriks[r][c].deleteMonstrik();
          allBullets[r*c+1].isDeleted = true;
          allBullets[numberOfBullets-1].returnBack(35, 0);
          monstersColumns[c]--;
        }
    }
  }

  //drawMonsters(White); //draw new one
}

void bulletsMove() {
  for(int c = 0; c < columns; c++) {
    for(int b = 0; b < bulletsPerColumn; b++) {
      if(allBullets[c][b].isDeleted || allBullets[c][b].stopMove) {
        continue;
      }
      if(allBulletsCoords[c][b][yCoord] <= 0) {
        allBulletsCoords[c][b][yCoord] = 20; //TODO: do it okay or smth :/
        continue;
      }
      allBulletsCoords[c][b][yCoord] -= 1;
    }
  }

  if(spaceShipBullet.isDeleted || spaceShipBullet.stopMove) {
    return;
  }
  
  if(spaceShipBulletCoord[yCoord] >= 70) {
    spaceShipBullet.stopMove = true;
    spaceShipBulletCoord[yCoord] = 0;
    return;
  }
  spaceShipBullet[yCoord] += 1;
}

void moveMonster(int column, int row) {
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    for (int horz=0; horz<sideOfMonster; horz++) {
      for (int ver=0; ver<sideOfMonster; ver++) {
        if(allMonsters[row][column].isDeleted) {
          return;
        }
        if (monstersMovingDirection == 'L') {
          allMonstersCoords[row][column][xCoord] -= moveDistance;
        }else if(monstersMovingDirection == 'D') {
          allMonstersCoords[row][column][yCoord] -= moveDistance;
        } else {
          allMonstersCoords[row][column][xCoord] += moveDistance;
        }
      }
    }
    previousMillis = currentMillis;
  }
}

void lowerMonsters() {}
  for(int r=0; r<rows; r++) {
    for(int c=0; c<columns; c++) {
      if(allMonsters[r][c].isDeleted) {
        continue;
      }
      allMonstersCoords[r][c][yCoord] -= 1;
    }
  }
}

void checkDirectionOfMonsters() {
  for(int c=0; c<columns; c++) {
    if(monstersColumns[c] != 0) { 
      for(int r=0; r<rows; r++) {
        if(allMonsters[r][i].isDeleted) {
          continue;
        }
        if(allMonstersCoords[r][c][xCoord] <= 0
          && allMonsters[r][c][yCoord] != 0
          ) {          
          monstersMovingDirection = 'R';
          lowerMonsters();
          break;
        }
      }
    }
  }

  for(int c=columns; c>0; c--) {
    if(monstersColumns[c] != 0) { 
      for(int r=0; r<rows; r++) {
        if(allMonsters[r][c].isDeleted) {
          continue;
        }
        if(allMonstersCoords[r][c][xCoord] >= 70
          && allMonsters[r][c][yCoord] != 0
          ) {
          monstersMovingDirection = 'L';
          lowerMonsters();
          break;
        }
      }
    }
  }
}

void createMonstersAndBullets() {
  for(int c = 0; c < columns; c++) {
    for(int b = 0; b < bulletsPerColumn; b++) {
      allBulletsCoords[c][b][xCoord] = 
        startPositionX + c * widthOfBullet + 1 * c;
      allBulletsCoords[c][b][yCoord] = 
        startPositionY + b * heightOfBullet + 2 * b;
      allBullets[c][b] = Bullet;
    }
  }

  for(int r = 0; r < rows; r++) {
    monstersColumns[c] = rows; //TODO: ask about it
    for(int c = 0; c < columns; c++) {
      allMonstersCoords[r][c] = 
      startPositionX + c * sideOfMonster + 1 * c,
      startPositionY + r * sideOfMonster + 2 * r;
      allMonsters[r][c] = Kubik;
    }
  }
}

//BUG:
bool checkCollision(Monster instance) {
  if(allBullets[numberOfBullets-1].stopMove || instance.isDeleted) {
    return false;
  }
  int xOfBullet = allBullets[numberOfBullets-1].allCoords[0][0][xCoord];
  int yOfBullet = allBullets[numberOfBullets-1].allCoords[0][0][yCoord];
  int xOfMonstrikLeft = instance.coordOfTheTopLeftCorner[xCoord];
  int yOfMonstrikLeft = instance.allCoords[yCoord];
  int xOfMonstrikRight = instance.coordOfTheTopLeftCorner[xCoord] 
                         + sideOfMonster;
  int yOfMonstrikRight = instance.coordOfTheTopLeftCorner[yCoord] 
                         + sideOfMonster;

  if(yOfBullet >= yOfMonstrikLeft 
    && yOfBullet <= yOfMonstrikRight+heightOfBullet-1 
    && xOfBullet >= xOfMonstrikLeft 
    && xOfBullet <= xOfMonstrikRight+widthOfBullet-1
    ){
    return true;
  }

  return false;
}
