#include "Constants.h"
#include "ScreenDriver.h"
#include "Display.cpp"

char monstersMovingDirection = 'L'; 
//TODO: google about enum
int monstersColumns[columns];
int spaceShipBulletCoord[2] = {35, 0};
int startPositionX = 70;
int startPositionY = 35;
unsigned long previousMillis;

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
} Bullet;

Bullet allBullets[maxBullets];
int maxBullets = 7 > column*rows ? column*rows : 7;

typedef struct {
  int xCoord;
  int yCoord;
  bool isInvulnerable;
} Ship;

Ship spaceShip = {35, shipHeight, 3, true, false};
Bullet spaceShipBullet; 

typedef struct {
  int score;
  int lifes;
} gameStats;

gamesStats stats = {0, 3};

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
  bulletsMove();

  changeDirectionOfMonsters();
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

}

void createMonsters() {
  int columnGap = 1;
  int rowGap = 2;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      monstersColumns[c] = rows; //TODO: ask if I should keep it here
      allMonstersCoords[r][c].xCoord = startPositionX +
                                        c * sideOfMonster + 
                                        columnGap * c;
      allMonstersCoords[r][c].yCoord = startPositionY +
                                        r * sideOfMonster + 
                                        rowGap * r;
      allMonsters[r][c].isDeleted = false;
    }
  }
}

void bulletsMove() {
  for(int b = 0; b < maxBullets; b++) {
    if(allBullets.isReadyToShoot && rand() % 100 > 50 ) {
      Monster randomMonster = getRandomMonster();
      allBullets[b].xCoord = randomMonster.xCoord;
      allBullets[b].yCoord = randomMonster.yCoord;
      continue;
    }
    allBullets[b].yCoord -= 1;
  }
}

Monster getRandomMonster() {
  int column = rand() % columns;
  int row = rand() % rows;
  if(allMonsters[row][column].isDeleted) {
    return getRandomCoords();
  }
  return allMonsters[row][column];
}

void lowerMonsters() { 
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < columns; c++) {
      allMonsters[r][c].yCoord -= 1;
    }
  }
}

void deleteMonster (int row, int column) {
  allMonsters[row][column].isDeleted = true;
  monstersColumns[c] -= 1;
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
        } else {
          allMonstersCoords[row][column][xCoord] += moveDistance;
        }
      }
    }
    previousMillis = currentMillis;
  }
}

void changeDirectionOfMonsters() {
  if (monstersMovingDirection == 'R') {
    for (int c=columns; c>0; c--) { // <- and this loop as one; switch columns
                                    //  and rows
      if (monstersColumns[c] != 0) { 
        for (int r=0; r<rows; r++) {
          if (allMonsters[r][c].isDeleted) {
            continue;
          }
          if (allMonstersCoords[r][c][xCoord] >= 70
              && allMonsters[r][c][yCoord] != 0
             ) {
            monstersMovingDirection = 'L';
            lowerMonsters();
            break;
          }
        }
      }
    }
    return;
  }

  for (int c=0; c<columns; c++) {// TODO: <- re-write this loop(c above in func)
    if (monstersColumns[c] != 0) { 
      for (int r=0; r<rows; r++) {
        if (allMonsters[r][i].isDeleted) {
          continue;
        }
        if (allMonstersCoords[r][c][xCoord] <= 0
          && allMonsters[r][c][yCoord] != 0
          ) {
          monstersMovingDirection = 'R';
          lowerMonsters();
          break;
        }
      }
    }
  }
}

bool checkCollisionWithShip() {
  for (int b = 0; b < maxBullets; b++) {
    if (allBullets[b].isReadyToShoot) {
      continue;
    }

    if (spaceShip.yCoord >= allBullets[b].yCoord - heightOfBullet
      && spaceShip.yCoord-shipHeight <= allBullets[b].yCoord
      && spaceShip.xCoord >= allBullets[b].xCoord + widthOfBullet
      && spaceShip.xCoord.shipWidth <= allBullets[b].xCoord
      ){
      allBullets[b].isReadyToShoot = true;
      stats.lifes -= 1;
      return true;
    }
  }
  return false;
}

bool checkCollisionWithMonsters() {
  for (int r = 0; r<rows; r++) {
    for (int c = 0; c<columns; c++) {
      if (allMonsters[r][c].isDeleted) {
        continue;
      }
      if (allMonsters[r][c].yCoord >= spaceShip.yCoord - heightOfBullet
          && allMonsters[r][c].yCoord-sideOfMonster <= spaceShip.yCoord
          && allMonsters[r][c].xCoord <= spaceShip.xCoord+shipWidth
          && allMonsters[r][c].xCoord+sideOfMonster >= spaceShip.xCoord
         ){
        allMonsters[r][c].isDeleted = true;
        return true;
      }
    }
  }
}

  void shootFromShip() {
      if(!spaceShipBullet.isReadyToShoot) {
          return;
      }
    }
