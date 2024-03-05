#include "ScreenDriver.h"
#include "Display.h"
#include "Constants.h"

unsigned long previousMillisForMoving = 0UL;
unsigned long previousMillisForInvulnerability = 0UL;
unsigned long previousMillisForSpeedingUp = 0UL;
unsigned long previousMillisForShooting = 0UL;
unsigned long previousMillisForMovingBullets = 0UL;
unsigned long previousMillisForMovingShipBullet = 0UL;
unsigned long interval = 200UL;
unsigned long intervalForShooting = random(500UL, 1000UL);
int columnsDestroyed = 0;

typedef struct {
  int xCoord;
  int yCoord;
  bool isInvulnerable;
}Ship;
Ship spaceShip = {0, spaceShipY, false};

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
  //drawGrid();
  //Serial.println(startPositionX);
}

void loop() {
  drawGrid();
  if(gamesStats.lifes == 0) {
    //loser print
    return;
  }
  if(columnsDestroyed == columns) {
    //winner print
    Serial.println("no way");
    return;
  }
  shootFromShip();
  //moveShipBullets();
  //checkCollisionWithMonsters();
  //checkCollisionWithShip();
  //moveMonsters();
  monstersBulletsMove();
  monstersShoot();
  drawShip(White);
  //moveShip('L');
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
      spaceShip.yCoord - shipHeight,
      shipWidth, 
      shipHeight, 
      colour == White ? Blue : Black 
    );
    return;
  }
  drawFigure(spaceShip.xCoord, 
      spaceShip.yCoord - shipHeight,
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
  spaceShipBullet.xCoord = spaceShip.xCoord + 1 + shipWidth / 2;
  spaceShipBullet.yCoord = spaceShip.yCoord + 1; // TODO:
  drawShipBullet(Red);
}

void moveShipBullets() {
  if(spaceShipBullet.isReadyToShoot) {
    return;
  }
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForMovingShipBullet
    < intervalForMovingBullets) {
    return;
  }
  previousMillisForMovingShipBullet = currentMillis;

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
          && allMonsters[r][c].yCoord - sideOfMonster 
          <= spaceShipBullet.yCoord
          && allMonsters[r][c].xCoord 
          <= spaceShipBullet.xCoord + widthOfBullet - 1
          && allMonsters[r][c].xCoord + sideOfMonster - 1 
          >= spaceShipBullet.xCoord
         ){
        spaceShipBullet.isReadyToShoot = true;
        drawMonster(r, c, Black);
        allMonsters[r][c].isDeleted = true;
        monstersColumns[c] -= 1;
        if (monstersColumns[c] == 0) {
          columnsDestroyed += 1;
        }
        return;
      }
    }
  }
}

void moveShip(char direction) {
  if(spaceShip.xCoord > gridXLimit - shipWidth && direction == 'R'
      || spaceShip.xCoord <= 0 && direction == 'L') {
    return;
  }
  delay(10);
  drawShip(Black);
  if (direction == 'R') {
    spaceShip.xCoord += moveDistanceForShip;
  } else if(direction == 'L') {
    spaceShip.xCoord -= moveDistanceForShip;
  }
  drawShip(White);
}

void createMonsters() {
  for (int c = 0; c < columns; c++) {
    monstersColumns[c] = rows;
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      int positionX = startPositionX + c * sideOfMonster + columnGap * c;
      int positionY = startPositionY - r * sideOfMonster - rowGap * r;

      allMonsters[r][c].xCoord = positionX;
      allMonsters[r][c].yCoord = positionY;
      allMonsters[r][c].isDeleted = false;
      drawMonster(r, c, White);
    }
  }
}

void moveMonsters() {
  monstersChangeDirection();
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForMoving < interval) {
    return;
  }
  if(intervalForSpeedingUp <
    currentMillis - previousMillisForSpeedingUp
    && interval >= 10UL
    ) {
    interval -= 10UL;
    previousMillisForSpeedingUp = currentMillis;
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      if(allMonsters[r][c].isDeleted) {
        continue;
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
 
void monstersShoot() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForShooting < intervalForShooting) {
    return;
  }
  intervalForShooting = random(500UL, 1000UL);
  previousMillisForShooting = currentMillis;
  for(int b = 0; b < maxBullets; b++) {
    if(allBullets[b].isReadyToShoot && rand() % 100 > 50 ) {
      allBullets[b].isReadyToShoot = false;
      shootRandomly(b);
      continue;
    }
  }
}

void shootRandomly(int b) {
  int column = rand() % columns;
  Serial.println("**********************");
  Serial.println(column);
  int row = 0;
  for ( int r = rows - 1; r >= 0; r-- ) {
    if(allMonsters[r][column].isDeleted == false) {
      row = r;
      break;
    }
  }
  if(allMonsters[row][column].isDeleted) {
    return shootRandomly(b);
  }

  allBullets[b].xCoord = allMonsters[row][column].xCoord + sideOfMonster / 2;
  allBullets[b].yCoord =  allMonsters[row][column].yCoord - sideOfMonster;
}

void monstersBulletsMove() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForMovingBullets
    < intervalForMovingBullets) {
    return;
  }
  previousMillisForMovingBullets = currentMillis;

  for(int b = 0; b < maxBullets; b++) {  
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

void checkCollisionWithShip() {
  unsigned long currentMillis = millis();
  if (spaceShip.isInvulnerable &&
      currentMillis - previousMillisForInvulnerability 
      > intervalForInvulnerability) {
      spaceShip.isInvulnerable = false;
  }

  for (int b = 0; b < maxBullets; b++) {
    if (allBullets[b].isReadyToShoot) {
      continue;
    }
    if (allBullets[b].xCoord + widthOfBullet - 1 >= spaceShip.xCoord &&
        allBullets[b].xCoord <= spaceShip.xCoord + shipWidth - 1 &&
        allBullets[b].yCoord >= spaceShip.yCoord - shipHeight &&
        allBullets[b].yCoord - heightOfBullet <= spaceShip.yCoord
       ){
      if (spaceShip.isInvulnerable) {
        drawMonstersBullet(b, Black);
        allBullets[b].isReadyToShoot = true;
        return;
      }
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
      if(allMonsters[r][c].xCoord >= gridXLimit - sideOfMonster + 1) {
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
      if(allMonsters[r][c].xCoord < moveDistanceForMonsters ) {
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
      drawFigure(
          allMonsters[r][c].xCoord,
          allMonsters[r][c].yCoord,
          sideOfMonster,
          1,
          Black
          );
      allMonsters[r][c].yCoord -= 1;
    }
  }
}
