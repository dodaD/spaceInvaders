//TODO: зробити штуку яка буде перетворювати координати сітки під координати екрану
#define screenHeight 800
#define screenWidth 400
#define NumberOfMonsters 1
#define firstKubikX 35
#define firstKubikY 70
#include "BulletClass.h"
#include "MonsterClass.h"
#include "Constants.h"
#include "TFTMC050_3.h"
#include "Display.h"

Bullet spaceBullet(35, 0);
Kubik allMonstriks[NumberOfMonsters];
Display display;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
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
  
  createMonsters(1); //number of rows in argument
}

void moveSpaceBullet() {
  Serial.println("Ok");
  spaceBullet.move();
}

void loop() {
  drawKubiks(White);
  drawBullet(Black); //erase old one
  spaceBullet.move();
  drawBullet(White); //draw new one

  for (int i=0; i<NumberOfMonsters; i++) {
    drawKubiks(Black);
    allMonstriks[i].move();
    bool haveCrushed = checkCollision(allMonstriks[i]);
    if(haveCrushed) {
      drawBullet(Black);
      spaceBullet.returnBack(35, 0);
      drawBullet(White);
      spaceBullet.stopMove = true;
      drawKubik(allMonstriks[i], Black);
      allMonstriks[i].deleteMonstrik();
    } 
  }

  // put your main code here, to run repeatedly:
}

void createMonsters(int rows) {
  int gap = 0;
  int rowGap = 0;

  for(int n=0; n<rows; n++) {
    gap = 0;
    for(int i=0; i<NumberOfMonsters/rows; i++) {
      allMonstriks[i+n] = Kubik (firstKubikX+gap+sideOfKubik*i, firstKubikY+sideOfKubik*n+rowGap);
      gap += 1;
    }
    rowGap += 2;
  }
}

void drawKubiks(int colour) {
  for (int i=0; i<NumberOfMonsters; i++) {
    drawKubik(allMonstriks[i], colour);
  }
}

void drawBullet(int colour) {
  if(spaceBullet.stopMove) {
    return;
  } 
  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      display.drawPixel(adjustCoordX(spaceBullet.allCoords[w][h][xCord]), adjustCoordY(spaceBullet.allCoords[w][h][yCord]), colour);
    }
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

bool checkCollision(Kubik instance) {
  if(spaceBullet.stopMove || instance.isDeleted) {
    return false;
  }
  for (int horz=0; horz<sideOfKubik; horz++) {
    for (int ver=0; ver<sideOfKubik; ver++) {
      for (int w=0; w<widthOfBullet; w++) {
        for (int h=0; h<heightOfBullet; h++) {
          bool haveSameX = instance.allCoords[horz][ver][xCord] == spaceBullet.allCoords[w][h][xCord];
          bool haveSameY = instance.allCoords[horz][ver][yCord] == spaceBullet.allCoords[w][h][yCord];
          if(haveSameY == true && haveSameX == true) {
            return true;
          }
        }
      } 
    }
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