#include "ScreenDriver.h"
#include "Display.h"
#include "Constants.h"

unsigned long previousMillisForMoving = 0UL;

typedef struct {
  int xCoord;
  int yCoord;
  bool isInvulnerable;
}Ship;
Ship spaceShip = {35, 20};

int monstersColumns[columns];
char monstersMovingDirection = 'R';
typedef struct {
  int xCoord;
  int yCoord;
  bool isDeleted;
} Monster;
Monster allMonsters[rows][columns];

typedef struct {
  int xCoord;
  int yCoord;
  bool isReadyToShoot;
}Bullet;

Bullet spaceShipBullet = {0, 0, true}; 
const int maxBullets = 7 > columns * rows ? columns * rows : 7;
Bullet allBullets[maxBullets];

void setup() {
  Serial.begin(9600);
  pinMode(5,   OUTPUT);
  digitalWrite(5, HIGH);//Disable  SD 
  pinMode(2,   OUTPUT);
  digitalWrite(2, HIGH);//Disable  RTP   
  
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
  createMonsters();
  createBulletsForMonsters();
}

void loop() {
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {

      moveMonsters();
      drawFigure(allMonsters[r][c].xCoord, 
        allMonsters[r][c].yCoord,
        sideOfMonster, 
        sideOfMonster, 
        White
      );
    }
  }
}

void shootFromShip() {
  if(!spaceShipBullet.isReadyToShoot) {
    return;
  }
  spaceShipBullet.isReadyToShoot = false;
  spaceShipBullet.xCoord = spaceShip.xCoord + shipWidth/2;
  spaceShipBullet.yCoord = spaceShip.yCoord;
}

void moveShip(char direction) {
  if (direction == 'R' && spaceShip.xCoord < gridXLimit - shipWidth) {
    drawFigure(
      spaceShip.xCoord, 
      spaceShip.yCoord, 
      shipWidth, 
      shipHeight,
      Black
    );
    spaceShip.xCoord += moveDistanceForShip;
    drawFigure(
      spaceShip.xCoord, 
      spaceShip.yCoord,
      shipWidth,
      shipHeight,
      White
    );
  } else if(spaceShip.xCoord > moveDistanceForShip && direction == 'L'){
    drawFigure(
      spaceShip.xCoord, 
      spaceShip.yCoord, 
      shipWidth, 
      shipHeight,
      Black
    );
    spaceShip.xCoord -= moveDistanceForShip;
    drawFigure(
      spaceShip.xCoord, 
      spaceShip.yCoord,
      shipWidth,
      shipHeight,
      White
    );
  }
}

void spaceShipShoot() {
    if(!spaceShipBullet.isReadyToShoot) {
      drawFigure(spaceShipBullet.xCoord, 
        spaceShipBullet.yCoord, 
        widthOfBullet, 
        heightOfBullet, 
        Black
      );
      spaceShipBullet.yCoord += 1;
        drawFigure(spaceShipBullet.xCoord, 
        spaceShipBullet.yCoord, 
        widthOfBullet,
        heightOfBullet,
        Green
       );
    }
}

void createMonsters() {
  int columnGap = 1;
  int rowGap = 2;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      monstersColumns[c] = rows; //TODO: ask if I should keep it here
      allMonsters[r][c].xCoord = startPositionX +
                                        c * sideOfMonster + 
                                        columnGap * c;
      Serial.println(allMonsters[r][c].xCoord);
      allMonsters[r][c].yCoord = startPositionY +
                                        r * sideOfMonster + 
                                        rowGap * r;
      allMonsters[r][c].isDeleted = false;
    }
  }
}

void moveMonsters() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForMoving < interval) {
    return;
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      if(allMonsters[r][c].isDeleted) {
      return;
      }
      Serial.print("I exist-: ");
      Serial.println(allMonsters[r][c].xCoord);
      drawFigure(allMonsters[r][c].xCoord,
          allMonsters[r][c].yCoord, 
          sideOfMonster, 
          sideOfMonster, 
          Black
          );
      if (monstersMovingDirection == 'L') {
        Serial.println("WTF");
        allMonsters[r][c].xCoord -= moveDistanceForMonsters;
        previousMillisForMoving = currentMillis;
      } else if(monstersMovingDirection == 'R') {
        allMonsters[r][c].xCoord += moveDistanceForMonsters;
      }
    }
  } 
  monstersChangeDirection();
  previousMillisForMoving = currentMillis;
}

void createBulletsForMonsters() {
  for (int i = 0; i < maxBullets; i++) {
    allBullets[i].isReadyToShoot = true;
  }
}

void monsterShoot() {
  for(int b = 0; b < maxBullets; b++) {
    if(allBullets[b].isReadyToShoot && rand() % 100 > 50 ) {
      allBullets[b].isReadyToShoot = false;
      Monster randomMonster = getRandomMonster();
      allBullets[b].xCoord = randomMonster.xCoord;
      allBullets[b].yCoord = randomMonster.yCoord;
      continue;
    }
    if(allBullets[b].yCoord < 0 + heightOfBullet) {
        allBullets[b].isReadyToShoot = true;
      }
    allBullets[b].yCoord -= 1;
  }
}

Monster getRandomMonster() {
  int column = rand() % columns;
  int row = rand() % rows;
  if(allMonsters[row][column].isDeleted) {
    return getRandomMonster();
  }
  return allMonsters[row][column];
}

void monstersChangeDirection() { 
  if (monstersMovingDirection == 'R') {
    for (int c = columns; c > 0; c--) { 
      if (monstersColumns[c] != 0) { 
        for (int r = 0; r < rows; r++) {
          if (allMonsters[r][c].isDeleted) {
            continue;
          }
          if (allMonsters[r][c].xCoord >= 70
              && allMonsters[r][c].yCoord != 0
             ) {
            Serial.print("I do not lmao: ");
            Serial.println(allMonsters[r][c].xCoord);
            monstersMovingDirection = 'L';
            lowerMonsters();
            return;
            break;
          }
        }
      }
    }
  }

  for (int c=0; c<columns; c++) {// TODO: <- re-write this loop(c above in func)
    if (monstersColumns[c] != 0) { 
      for (int r=0; r<rows; r++) {
        if (allMonsters[r][c].isDeleted) {
          continue;
        }
        if (allMonsters[r][c].xCoord <= 0
          && allMonsters[r][c].yCoord != 0
          ) {
          monstersMovingDirection = 'R';
          lowerMonsters();
          break;
        }
      }
    }
  }
}

void lowerMonsters() { 
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      allMonsters[r][c].yCoord -= 1;
    }
  }
}
