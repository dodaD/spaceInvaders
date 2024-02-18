#include "ScreenDriver.h"
#include "Display.h"
#include "Constants.h"

unsigned long previousMillisForMoving = 0UL;
unsigned long previousMillisForInvulnerability = 0UL;
unsigned long previousMillisForSpeedingUp = 0UL;
unsigned long interval = 0UL;

typedef struct {
  int xCoord;
  int yCoord;
  bool isInvulnerable;
}Ship;
Ship spaceShip = {60, 20, false};

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

typedef struct {
  int score;
  int lifes;
}Stats;

Stats gamesStats= {0, 3};

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
  if(gamesStats.lifes == 0) {
    //loser print
    return;
  }
  drawShip(White);
  shootFromShip();
  moveShipBullets();
  //checkCollisionWithMonsters();
  //checkCollisionWithShip();
  moveMonsters();
  monsterShoot();
}

void drawMonster(int r, int c, int colour) {
  drawFigure(
      allMonsters[r][c].xCoord,
      allMonsters[r][c].yCoord,
      sideOfMonster,
      sideOfMonster,
      colour
      );
}

void drawShip(int colour) {
  if(spaceShip.isInvulnerable) {
    drawFigure(spaceShip.xCoord, 
    spaceShip.yCoord,
    shipWidth, 
    shipHeight, 
    Blue
    );
    delay(25);
    drawFigure(spaceShip.xCoord, 
    spaceShip.yCoord,
    shipWidth, 
    shipHeight, 
    Black
    );
    return;
  }
  drawFigure(spaceShip.xCoord, 
      spaceShip.yCoord,
      shipWidth, 
      shipHeight, 
      colour
  );
}

void drawMonstersBullet(int b, int colour) {
  drawFigure(allBullets[b].xCoord,
      allBullets[b].yCoord, 
      widthOfBullet, 
      heightOfBullet, 
      colour
      );
}

void drawShipBullet(int colour) {
  drawFigure(spaceShipBullet.xCoord,
        spaceShipBullet.yCoord, 
        widthOfBullet, 
        heightOfBullet, 
        colour
   );
}

void shootFromShip() {
  if(!spaceShipBullet.isReadyToShoot) {
    return;
  }
  drawShipBullet(Black);
  spaceShipBullet.isReadyToShoot = false;
  spaceShipBullet.xCoord = spaceShip.xCoord + shipWidth / 2;
  spaceShipBullet.yCoord = spaceShip.yCoord + 2; // TODO:
}

void moveShipBullets() {
    if(!spaceShipBullet.isReadyToShoot) {
      drawShipBullet(Black);
      if(spaceShipBullet.yCoord >= gridYLimit) {
        spaceShipBullet.isReadyToShoot = true;
        return;
      }       
      spaceShipBullet.yCoord += 1;
      drawShipBullet(Red);
    }
}

void checkCollisionWithMonsters() {
  if (spaceShipBullet.isReadyToShoot) {
    return false;
  } else if (spaceShipBullet.yCoord >= gridYLimit){
    return;
  }
  for (int r = 0; r<rows; r++) {
    for (int c = 0; c<columns; c++) {
      if (allMonsters[r][c].isDeleted) {
        continue;
      }
      if (allMonsters[r][c].yCoord >= spaceShipBullet.yCoord - heightOfBullet
          && allMonsters[r][c].yCoord-sideOfMonster <= spaceShipBullet.yCoord
          && allMonsters[r][c].xCoord <= spaceShipBullet.xCoord+widthOfBullet
          && allMonsters[r][c].xCoord+sideOfMonster >= spaceShipBullet.xCoord
         ){
        spaceShipBullet.isReadyToShoot = true;
        drawMonster(r, c, Black);
        allMonsters[r][c].isDeleted = true;
        return;
      }
    }
  }
}

void moveShip(char direction) {
  drawShip(Black);
  if (direction == 'R' && spaceShip.xCoord < gridXLimit - shipWidth) {
    spaceShip.xCoord += moveDistanceForShip;
  } else if(spaceShip.xCoord > moveDistanceForShip && direction == 'L') {
    spaceShip.xCoord -= moveDistanceForShip;
  }
  drawShip(White);
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
  if(intervalForSpeedingUp <
    currentMillis - previousMillisForSpeedingUp
    && interval > 10UL
    ) {
    interval -= 10UL;
    previousMillisForSpeedingUp = currentMillis;
  }
  monstersChangeDirection();

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      if(allMonsters[r][c].isDeleted) {
      return;
      }
      drawMonster(r, c, Black);
      if (monstersMovingDirection == 'L') {
        allMonsters[r][c].xCoord -= moveDistanceForMonsters;
        previousMillisForMoving = currentMillis;
      } else if(monstersMovingDirection == 'R') {
        allMonsters[r][c].xCoord += moveDistanceForMonsters;
        previousMillisForMoving = currentMillis;
      }
      drawMonster(r, c, White);
    }
  } 
}

void createBulletsForMonsters() {
  for (int i = 0; i < maxBullets; i++) {
    allBullets[i].isReadyToShoot = true;
  }
}
 
void monsterShoot() {
  delay(25);//just for demonstrationdrawMonstersBullets
  for(int b = 0; b < maxBullets; b++) {
    if(allBullets[b].isReadyToShoot && rand() % 100 > 50 ) {
      allBullets[b].isReadyToShoot = false;
      Monster randomMonster = getRandomMonster();
      allBullets[b].xCoord = randomMonster.xCoord + sideOfMonster / 2;
      allBullets[b].yCoord = randomMonster.yCoord - sideOfMonster;
      continue;
    }
    drawMonstersBullet(b, Black);
    if(allBullets[b].isReadyToShoot) {
      continue;
    }
    if(allBullets[b].yCoord < 0 + heightOfBullet) {
      allBullets[b].isReadyToShoot = true;
      return;
    }
    allBullets[b].yCoord -= 1;
    drawMonstersBullet(b, Green);
  }
}

Monster getRandomMonster() {
  int column = rand() % columns;
  int row = rand() % rows;
  if(allMonsters[row][column].isDeleted
    ) {
    return getRandomMonster();
  }
  return allMonsters[row][column];
}

void checkCollisionWithShip() {
  unsigned long currentMillis = millis();
  if (spaceShip.isInvulnerable &&
      currentMillis - previousMillisForInvulnerability 
      > intervalForInvulnerability) {
      spaceShip.isInvulnerable = false;
  } else if (spaceShip.isInvulnerable){
    return;
  }

  for (int b = 0; b < maxBullets; b++) {
    if (allBullets[b].isReadyToShoot) {
      continue;
    }

    if (allBullets[b].xCoord + widthOfBullet >= spaceShip.xCoord &&
        allBullets[b].xCoord <= spaceShip.xCoord + shipWidth &&
        allBullets[b].yCoord >= spaceShip.yCoord - shipHeight &&
        allBullets[b].yCoord - heightOfBullet <= spaceShip.yCoord
      ){
      drawMonstersBullet(b, Black);
      allBullets[b].isReadyToShoot = true;
      gamesStats.lifes -= 1;
      spaceShip.isInvulnerable = true;
      previousMillisForInvulnerability = currentMillis;
      return;
    }
  }
  return;
}

void monstersChangeDirection() { 
  for(int c = columns - 1; c > 0 && monstersMovingDirection == 'R'; c--) {
    if(monstersColumns[c] == 0) {
      continue;
    }
    for(int r = 0; r < rows; r++) {
      if(allMonsters[r][c].isDeleted == true ) {
        continue;
      }
      if(allMonsters[r][c].xCoord >= gridXLimit - sideOfMonster) {
        monstersMovingDirection = 'L';
        lowerMonsters();
        return;
      }
    }
  }

  for(int c = 0; c < columns && monstersMovingDirection == 'L'; c++) {
    if(monstersColumns[c] == 0) {
      continue;
    }
    for(int r = 0; r < rows; r++) {
      if(allMonsters[r][c].isDeleted == true ) {
        continue;
      }
      if(allMonsters[r][c].xCoord <= 0 ) {
        monstersMovingDirection = 'R';
        lowerMonsters(); 
        return;
      }
    }
  }
}

void lowerMonsters() { 
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      if(allMonsters[r][c].isDeleted) {
        continue;
      }

      if(allMonsters[r][c].yCoord - sideOfMonster <= spaceShip.yCoord + 1) {
        gamesStats.lifes = 0;
        return;
      }
      drawMonster(r, c, Black);
      allMonsters[r][c].yCoord -= 1;
    }
  }
}
