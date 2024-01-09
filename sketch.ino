// TODO: зробити штуку яка буде перетворювати координати сітки під координати екрану

// ---- DONE
// ~13 mosters, continious moving slowly, shooting
// player's bullet shoots back continiously
// player's bullet can kill monsters 

// ---- TO BE DONE
// player's ship, can be moved with stick, shooting when button clicked - one at a time
// mosters bullet can kill player's ship - after this, blinking invulnerable - 2s
// ship has 3 (depends on difficulty) lives, game over when no lives left - you can restart game
// if all monsters are dead, you won the game
// render monsters istead of boxes

// ---- TO BE DONE 2
// list of games - you highlight game, you can run it
// when open game you see title and pic / author / whatever
// "start game" - when you click, you choosing difficulty level, also "quit" - back to menu, "leaderloard"
// "leaderboard" - when you click, you open a table with row number and score sorted, only 20 top scorers
// "start game" - game starts 
// when you lose or win, you see leaderboard with your position
// when you lose, you can choose between replay or exit to start menu
// game needs score, need counting logic + visible for player


#include "BulletClass.h"
#include "MonsterClass.h"
#include "Constants.h"
#include "TFTMC050_3.h" // TODO: rename
#include "Display.h"

Kubik allMonstriks[rows][columns]; // TODO: rename
Bullet allBullets[numberOfBullets];
//Bullet* allBullets = allBulletsArray;
Display display;
bool moveWest = true; //TODO: rename west coast

// put your setup code here, to run once:
void setup() {
  allBullets[numberOfBullets-1] = Bullet(35, 0, true); // TODO: rename
  Serial.begin(9600);
  pinMode(5,   OUTPUT);
  digitalWrite(5, HIGH);//Disable  SD 
  pinMode(2,   OUTPUT);
  digitalWrite(2, HIGH);//Disable  RTP   
  
  // ya hz
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
  drawKubks(Black); //erase kubiks
  drawBullets(Black); //erase old one
  bulletsMove();
  drawBullets(White); //draw new one

  checkCoordsOfMonsters();
  for(int r=0; r<rows; r++) {
    for(int c=0; c<columns; c++) {
      allMonstriks[r][c].move(moveWest);
      bool haveCrushed = checkCollision(allMonstriks[r][c]);
      if(haveCrushed) {
          allMonstriks[r][c].deleteMonstrik();
          allBullets[r*c+1].isDeleted = true;
          allBullets[numberOfBullets-1].returnBack(35, 0);
        }
    }
  }


  /*for (int i=0; i<numberOfMonsters; i++) {
    allMonstriks[i].move(moveWest);
    bool haveCrushed = checkCollision(allMonstriks[i]);
    if(haveCrushed) {
      drawBullet(allBullets[numberOfBullets-1], Black);
      allBullets[numberOfBullets-1].returnBack(35, 0);
      drawBullet(allBullets[numberOfBullets-1], White);
      allBullets[numberOfBullets-1].stopMove = true;
      drawKubik(allMonstriks[i], Black);
      drawBullet(allBullets[i], Black);
      allMonstriks[i].deleteMonstrik();
      allBullets[i].isDeleted = true;
    }
  }*/
  drawKubiks(White); //draw new one
}

void bulletsMove() {
  for (int i=0; i<numberOfBullets-1; i++) {
    allBullets[i].move();
    if(allBullets[i].haveReturned) {
      allBullets[i].returnBack(allMonstriks[i].allCoords[sideOfKubik/2][sideOfKubik-1][xCord], allMonstriks[i].allCoords[sideOfKubik/2][sideOfKubik-1][yCord]);
      allBullets[i].haveReturned = false;
    }
  }
  allBullets[numberOfBullets-1].move();
}

void checkCoordsOfMonsters() {
  for (int i=0; i<numberOfMonsters; i++) {
    if(allMonstriks[i].allCoords[0][0][xCord] <= 0+moveDistance) {
      moveWest = false;
    }else if(allMonstriks[i].allCoords[sideOfKubik-1][sideOfKubik-1][xCord] >= 70-moveDistance) {
      moveWest = true;
    } 
  }
}

//void createMonsters() {
//  int gap = 0;
//  // TODO: gap ne potriben
//  int rowGap = 0;
//
//  for(int i=0; i<numberOfMonsters; i++) {
//    gap = (i + columns) % columns;
//    rowGap = (i + rows) % rows;
//    // FIXME: i+n means duplicates in array on different rows, same number
//    allMonstriks[i] = Kubik(
//          firstKubikX+gap+sideOfKubik*i,
//          firstKubikY+sideOfKubik+rowGap,
//          i,
//          allBullets
//      );
//    // TODO: podumati nad cim (dvumernij-masiv)
//  }
//}

void createMonstersAndBullets() {
  for(int r=0; r<rows; r++) {
    for(int c=0; c<columns; c++) {
      allMonstriks[r][c] = Kubik(firstKubikX+allMonstriks[r][c-1].coordOfTheTopLeftCorner[xCord]+1, firstKubikY+allMonstriks[r-1][c].coordOfTheTopLeftCorner[yCord]+2);
      allBullets[r*c+1] = Bullet(allMonstriks[r][c].allCoords[sideOfKubik/2][0], allMonstriks[r][c].allCoords[0][sideOfKubik/2],false);
    }
  }
}

void drawKubiks(int colour) {
  for (int i=0; i<numberOfMonsters; i++) {
    drawKubik(allMonstriks[i], colour);
  }
}

void drawKubik(Kubik instance, int colour) {
  if(instance.isDeleted) {
    return;
  }
  for (int horz=0; horz<sideOfKubik; horz++) {
    for (int ver=0; ver<sideOfKubik; ver++) {
     display.drawPixel(adjustCoordX(instance.allCoords[horz][ver][xCord]), adjustCoordY(instance.allCoords[horz][ver][yCord]), colour);
    }
  }
}

void drawBullets(int colour) {
  for (int i=0; i<numberOfBullets; i++) {
    drawBullet(allBullets[i], colour);
  }
}

void drawBullet(Bullet instance,int colour) {
  if(instance.isDeleted) {
    return;
  }
  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      display.drawPixel(adjustCoordX(instance.allCoords[w][h][xCord]), adjustCoordY(instance.allCoords[w][h][yCord]), colour);
    }
  }
}

bool checkCollision(Kubik instance) {
  if(allBullets[numberOfBullets-1].stopMove || instance.isDeleted) {
    return false;
  }
  int xOfBullet = allBullets[numberOfBullets-1].allCoords[0][0][xCord];
  int yOfBullet = allBullets[numberOfBullets-1].allCoords[0][0][yCord];
  int xOfMonstrikLeft = instance.allCoords[0][0][xCord];
  int yOfMonstrikLeft = instance.allCoords[0][0][yCord];
  int xOfMonstrikRight = instance.allCoords[sideOfKubik-1][0][xCord];
  int yOfMonstrikRight = instance.allCoords[0][sideOfKubik-1][yCord];

  if(yOfBullet >= yOfMonstrikLeft && yOfBullet <= yOfMonstrikRight+heightOfBullet-1 && xOfBullet >= xOfMonstrikLeft && xOfBullet <= xOfMonstrikRight+widthOfBullet-1){
    return true;
  }

  return false;
}

int adjustCoordX(float oldXCoord) {
  float newXCoord = (oldXCoord / 70.0 * LCD_XSIZE_TFT);
  return newXCoord;
}

int adjustCoordY(float oldYCoord) {
  float newYCoord = LCD_YSIZE_TFT - (oldYCoord / 100.0 * LCD_YSIZE_TFT);
  return newYCoord;
}
