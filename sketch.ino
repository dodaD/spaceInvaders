int buttonPinBlack = A0;
int buttonPinBlue = A1;
int buttonPinRed = A2;
int buttonPinGreen = A4;
int buttonPinYellow = A3;
bool isGameOver = false;

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
unsigned long interval = originalSpeed; // Is here because when monsters speed up, 
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
char monstersAnimationPhase = 'O';
typedef struct {
  int xCoord;
  int yCoord;
  bool isDeleted;
  int typeOfMonster;
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
const int maxBullets = 7 > columns ? columns : 7;
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
 // drawGrid();
  //Serial.println(startPositionX);
}

void loop() {
  if(isGameOver) {
    if(digitalRead(buttonPinGreen) == LOW) {
      restartGame();
    } else if (digitalRead(buttonPinYellow) == LOW) {
      quit();
    } 
    return;
  } 

  if(columnsDestroyed == columns) {
    int bonusTime = rows * columns * 20 
      < millis() / 1000 - secPassed
      ? 0
      :  rows * columns * 20 - (millis() / 1000 - secPassed); 
    int bonusPointsForSpeed = bonusTime / 10 * 2;
    drawWinningText(bonusPointsForSpeed, gamesStats.lifes * 10);
    drawStats((gamesStats.score + gamesStats.lifes * 10 + bonusPointsForSpeed), 
        0);
    isGameOver = true;
    return;
  }

  drawStats(gamesStats.score, gamesStats.lifes);
  if(gamesStats.lifes == 0) {
    drawLoserText();
    isGameOver = true;
    return;
  }

  // drawGrid();
  moveShipBullets();
  moveMonsters();
  checkCollisionWithMonsters();
  checkCollisionWithShip();
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
  if (allMonsters[r][c].typeOfMonster == 1) {
    reDrawMonster1(allMonsters[r][c].xCoord,
        allMonsters[r][c].yCoord,
        monstersMovingDirection, White);
    return;
  } else if (allMonsters[r][c].typeOfMonster == 2) {
    reDrawMonster2(allMonsters[r][c].xCoord,
        allMonsters[r][c].yCoord,
        monstersMovingDirection, White);
    return;
  }
  reDrawMonster3(allMonsters[r][c].xCoord,
      allMonsters[r][c].yCoord,
      monstersMovingDirection, White);
}

void drawAnimation(int r, int c, int colour) {
  if (allMonsters[r][c].typeOfMonster == 1) {
    if (colour == Black) {
      drawMonster1Animation(allMonsters[r][c].xCoord,
          allMonsters[r][c].yCoord,
          colour,
          monstersAnimationPhase == 'O' ? 'C' : 'O'); 
      return;
    }
    drawMonster1Animation(allMonsters[r][c].xCoord,
        allMonsters[r][c].yCoord,
        colour,
        monstersAnimationPhase); 
    return;
  } else if (allMonsters[r][c].typeOfMonster == 2) {
    if (colour == Black) {
      drawMonster2Animation(allMonsters[r][c].xCoord,
          allMonsters[r][c].yCoord,
          colour,
          monstersAnimationPhase == 'O' ? 'C' : 'O'); 
      return;
    }
    drawMonster2Animation(allMonsters[r][c].xCoord,
        allMonsters[r][c].yCoord,
        colour,
        monstersAnimationPhase);
    return;
  }

  if(colour == Black) {
    drawMonster3Animation(allMonsters[r][c].xCoord,
        allMonsters[r][c].yCoord,
        colour,
        monstersAnimationPhase == 'O' ? 'C' : 'O'); 
    return;
  }
  drawMonster3Animation(allMonsters[r][c].xCoord,
      allMonsters[r][c].yCoord,
      colour,
      monstersAnimationPhase); 
}

void drawBase (int r, int c) {
  if (allMonsters[r][c].typeOfMonster == 1) {
    drawMonster1Base(allMonsters[r][c].xCoord,
        allMonsters[r][c].yCoord,
        White);
    return;
  } else if (allMonsters[r][c].typeOfMonster == 2) {
    drawMonster2Base(allMonsters[r][c].xCoord,
        allMonsters[r][c].yCoord,
        White);
    return;
  }
  drawMonster3Base(allMonsters[r][c].xCoord,
      allMonsters[r][c].yCoord,
      White);
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
        drawFigure(allMonsters[r][c].xCoord,
            allMonsters[r][c].yCoord, 
            sideOfMonster, 
            sideOfMonster, 
            Black
            );
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
  int typesOfMonsters = 3;
  int currentMonsterType = 0;
  int amountOfRowsWithTheSameType = rows / typesOfMonsters;

  for (int c = 0; c < columns; c++) {
    monstersColumns[c] = rows;
  }

  for (int r = 0; r < rows; r++) {
    if(r % amountOfRowsWithTheSameType == 0 &&
        currentMonsterType < typesOfMonsters
      ) {
      currentMonsterType++;
    }

    for (int c = 0; c < columns; c++) {
      int positionX = startPositionX + c * sideOfMonster + columnGap * c;
      int positionY = startPositionY - r * sideOfMonster - rowGap * r;

      allMonsters[r][c].xCoord = positionX;
      allMonsters[r][c].yCoord = positionY;
      allMonsters[r][c].typeOfMonster = currentMonsterType;
      allMonsters[r][c].isDeleted = false;
      drawBase(r, c);
    }
  }    
}

void moveMonsters() {
  if(monstersChangeDirection()) {
    return;
  }
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisForMoving < interval) {
    return;
  }
  previousMillisForMoving = currentMillis;

  if(intervalForSpeedingUp <
      currentMillis - previousMillisForSpeedingUp
      && interval >= speedDecerease
    ) { 
    interval -= speedDecerease;
    previousMillisForSpeedingUp = currentMillis;
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      if(allMonsters[r][c].isDeleted) {
        continue;
      }
      if (monstersMovingDirection == 'L') {
        drawAnimation(r, c, Black);
        drawMonster(r, c, White);

        allMonsters[r][c].xCoord -= moveDistanceForMonsters;

        drawAnimation(r, c, White);
      } else if(monstersMovingDirection == 'R') {
        drawAnimation(r, c, Black);
        drawMonster(r, c, White);

        allMonsters[r][c].xCoord += moveDistanceForMonsters;

        drawAnimation(r, c, White);
      }
    }
  } 
  changeAnimation();
}

void changeAnimation() {
  if(monstersAnimationPhase == 'O') {
    monstersAnimationPhase = 'C';
  } else {
    monstersAnimationPhase = 'O';
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

  if(columnsDestroyed >= maxBullets) {
    return;
  }

  int columnsThatHadBullet[maxBullets - columnsDestroyed];
  
  intervalForShooting = random(500UL, 1000UL); // TODO: discus randomizer logic
  previousMillisForShooting = currentMillis;
  for(int b = 0; b < maxBullets - columnsDestroyed; b++) { //instead going through
                                                        //all bullets, go just as
                                                        //much as there's columns
                                                        //left
    if(allBullets[b].isReadyToShoot && rand() % 100 > 50 ) {
      int column = chooseRandomColumn(columnsThatHadBullet, 
          (maxBullets - columnsDestroyed));
      columnsThatHadBullet[b] = column;
      int row = chooseRow(column);

      allBullets[b].isReadyToShoot = false;
      allBullets[b].xCoord = allMonsters[row][column].xCoord + sideOfMonster / 2;
      allBullets[b].yCoord =  allMonsters[row][column].yCoord - sideOfMonster;
      drawMonstersBullet(b, Green);
      continue;
    }
  }
}

int chooseRandomColumn(int arr[], int sizeOfArr) {
  int column = rand() % columns;
  if (monstersColumns[column] == 0) {
    return chooseRandomColumn(arr, sizeOfArr);
  }
  for (int i = 0; i < sizeOfArr; i++) {
      if (column == arr[i]) {
        return chooseRandomColumn(arr, sizeOfArr);
        }
    }
  return column;
}

int chooseRow(int column) {
  int row = 0;
  for (int r = rows - 1; r >= 0; r--) {
    if(allMonsters[r][column].isDeleted == false) {
      row = r;
      break;
    }
  }
  return row;
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

bool monstersChangeDirection() { 
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
        return true;
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
      if(allMonsters[r][c].xCoord <= 0) {
        monstersMovingDirection = 'R';
        lowerMonsters(); 
        return true;
      }
    }
  }
  return false;
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
      drawFigure(allMonsters[r][c].xCoord,
          allMonsters[r][c].yCoord, 
          sideOfMonster, 
          sideOfMonster, 
          Black
          );
      allMonsters[r][c].yCoord -= 1;
      drawBase(r, c);

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
  interval = originalSpeed;
  secPassed = millis() / 1000;

  columnsDestroyed = 0;
  gamesStats.lifes = 3;
  gamesStats.score = 0;
  spaceShipBullet.isReadyToShoot = true;
  spaceShip.isInvulnerable = false;
  spaceShip.xCoord = spaceShipX;
  isGameOver = false;
  monstersMovingDirection = 'R';

  createBulletsForMonsters();
  createMonsters();
}

void quit() {
  ER5517.DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,Black);
  isGameOver = true;
}

