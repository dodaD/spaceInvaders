int buttonPinBlack = A0;
int buttonPinBlue = A1;
int buttonPinRed = A2;
int buttonPinGreen = A4;
int buttonPinYellow = A3;
bool powerOn = true;

#include "ScreenDriver.h"
#include "Display.h"
#include "Constants.h"

unsigned long previousMillisForMoving = 0UL;
unsigned long previousMillisForInvulnerability = 0UL;
unsigned long previousMillisForSpeedingUp = 0UL;
unsigned long previousMillisForShooting = 0UL;
unsigned long previousMillisForMovingBullets = 0UL;
unsigned long previousMillisForMovingShipBullet = 0UL;
unsigned long previousMillisForMovingShip = 0UL;
unsigned long intervalForShooting = random(500UL, 1000UL);
unsigned long interval = 1600UL; // Is here because when monsters speed up, 
                                 // this value changes 
int columnsDestroyed = 0;
unsigned long secPassed = 0; //declare globally in case of restarting the game

typedef struct {
  int xCoord;
  int yCoord;
  bool isInvulnerable;
}Ship;
Ship spaceShip = {spaceShipX, spaceShipY, false};

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

Stats gamesStats = {0, 3};
Bullet spaceShipBullet = {0, 0, true}; 
const int maxBullets = 7 > columns * rows ? columns * rows : 7;
Bullet allBullets[maxBullets];

void setup() {  
  pinMode(buttonPinBlack, INPUT_PULLUP);
  pinMode(buttonPinBlue, INPUT_PULLUP);
  pinMode(buttonPinRed, INPUT_PULLUP);
  pinMode(buttonPinGreen, INPUT_PULLUP);
  pinMode(buttonPinYellow, INPUT_PULLUP);

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
  drawGrid();
  //Serial.println(startPositionX);
}

void loop() { 
  if(!powerOn) {
    if(digitalRead(buttonPinGreen) == LOW) {
      restartGame();
      powerOn = true;
    } else if (digitalRead(buttonPinYellow) == LOW) {
      quit();
    } 
    return;
  } 

  if(columnsDestroyed == columns) {
    int bonusPointsForSpeed = (rows * columns * 20 - 
        (millis() / 1000 - secPassed)) / 10 * 2; // BUG: when negative value, 
                                                 //show a random number (13122)
    Serial.println(millis() / 1000);
    Serial.println(bonusPointsForSpeed);
    drawWinningText(bonusPointsForSpeed, gamesStats.lifes * 10);
    drawStats((gamesStats.score + gamesStats.lifes * 10 + bonusPointsForSpeed), 
        0);
    powerOn = false;
    return;
  }

  drawStats(gamesStats.score, gamesStats.lifes);
  if(gamesStats.lifes == 0) {
    drawLoserText();
    powerOn = false;
    return;
  }

  drawGrid();
  moveShipBullets();
  moveMonsters();
  checkCollisionWithMonsters();
  //checkCollisionWithShip();
  monstersBulletsMove();
  monstersShoot();
  drawShip(White); 

  if (digitalRead(buttonPinBlack) == LOW){
    moveShip('L');
  }

  if (digitalRead(buttonPinBlue) == LOW){
    moveShip('R');
  }

  if (digitalRead(buttonPinRed) == LOW){
    shootFromShip();
  }
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
        colour == Black ? Black : Blue
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
  if (allBullets[b].isReadyToShoot) {
    return;
  }
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
  spaceShipBullet.isReadyToShoot = false;
  spaceShipBullet.xCoord = spaceShip.xCoord + shipWidth / 2;
  spaceShipBullet.yCoord = spaceShip.yCoord + heightOfBullet; 
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

  drawShipBullet(Black);
  if(spaceShipBullet.yCoord >= gridYLimit) {
    spaceShipBullet.isReadyToShoot = true;
    return;
  }       
  spaceShipBullet.yCoord += 1;
  drawShipBullet(Red);
}

void checkCollisionWithMonsters() {
  if (spaceShipBullet.isReadyToShoot) {
    return;
  } else if (spaceShipBullet.yCoord - heightOfBullet >= gridYLimit){
    return;
  }
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
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
        drawShipBullet(Black);
        allMonsters[r][c].isDeleted = true;
        monstersColumns[c] -= 1;
        gamesStats.score += (10 * rows) - 10 * r;
        if (monstersColumns[c] == 0) {
          columnsDestroyed += 1;
        }
        return;
      }
    }
  }
}

void moveShip(char direction) {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForMovingShip < intervalForMovingShip) {
    return;
  }  
  previousMillisForMovingShip = currentMillis;

  if (spaceShip.xCoord <= 0 && direction == 'L') {
    return;
  }
  drawShip(Black);

  if(spaceShip.xCoord > gridXLimit - shipWidth - moveDistanceForShip
      && direction == 'R') {
    spaceShip.xCoord = gridXLimit - shipWidth;
    drawShip(White);
    return;
  }

  if (direction == 'R') {
    spaceShip.xCoord += moveDistanceForShip;
  } else if (direction == 'L') {
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
      && interval >= 200UL
    ) { 
    interval -= 200UL;
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
  for(int b = 0; b < columns - columnsDestroyed; b++) { //instead going through
                                                        //all bullets, go just as
                                                        //much as there's columns
                                                        //left
    if(allBullets[b].isReadyToShoot && rand() % 100 > 50 ) {
      allBullets[b].isReadyToShoot = false;
      shootRandomly(b);
      continue;
    }
  }
}

void shootRandomly(int b) {
  int column = rand() % columns;
  int row = 0;
  if(monstersColumns[column] == 0) {
    return shootRandomly(b);
  }

  for ( int r = rows - 1; r >= 0; r-- ) {
    if(allMonsters[r][column].isDeleted == false) {
      row = r;
      break;
    }
  }

  allBullets[b].xCoord = allMonsters[row][column].xCoord + sideOfMonster / 2;
  allBullets[b].yCoord =  allMonsters[row][column].yCoord - sideOfMonster;
  drawMonstersBullet(b, Green);
}

void monstersBulletsMove() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForMovingBullets
      < intervalForMovingBullets) {
    return;
  }
  previousMillisForMovingBullets = currentMillis;

  for(int b = 0; b < maxBullets; b++) {  
    if(allBullets[b].isReadyToShoot) {
      continue;
    }
    drawMonstersBullet(b, Black);
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
  for(int c = columns - 1; c >= 0 && monstersMovingDirection == 'R'; c--) {
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

void restartGame() {
  ER5517.DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Black);
  previousMillisForMoving = 0UL;
  previousMillisForInvulnerability = 0UL;
  previousMillisForSpeedingUp = 0UL;
  previousMillisForShooting = 0UL;
  previousMillisForMovingBullets = 0UL;
  previousMillisForMovingShipBullet = 0UL;
  previousMillisForMovingShip = 0UL;
  intervalForShooting = random(500UL, 1000UL);
  interval = 1600UL;
  secPassed = millis() / 1000;

  columnsDestroyed = 0;
  gamesStats.lifes = 3;
  gamesStats.score = 0;
  spaceShipBullet = {0, 0, true};
  spaceShip.isInvulnerable = false;
  spaceShip.xCoord = spaceShipX;
  powerOn = true;
  monstersMovingDirection = 'R';

  createBulletsForMonsters();
  createMonsters();
}

void quit() {
  ER5517.DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Black);
  powerOn = false;
}

