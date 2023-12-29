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

Bullet spaceBullet(35, 0);
Kubik allMonstriks[NumberOfMonsters];

void setup() {
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
  Serial.begin(9600);
  createMonsters();
}

void loop() {
  spaceBullet.move();
  drawFigures(White);
  bool haveCrushed = checkCollision(allMonstriks[0]);
  if(haveCrushed) {
    drawFigures(Black);
    spaceBullet.stopMove = true;
    spaceBullet.returnBack(35, 0);
    for (int horz=0; horz<sideOfKubik; horz++) {
      for (int ver=0; ver<sideOfKubik; ver++) {
        ER5517.DrawPixel(adjustCoordX(allMonstriks[0].allCoords[horz][ver][xCord]), adjustCoordY(allMonstriks[0].allCoords[horz][ver][yCord]), Black);
      }
    }
    allMonstriks[0].deleteMonstrik();
  } 
  drawFigures(Black);
  /*Serial.print("Y: ");
  Serial.println(spaceBullet.allCoords[0][0][1]);
  Serial.print("X: ");
  Serial.println(spaceBullet.allCoords[0][0][0]);
  Serial.println(haveCrushed ? "crushed" : "not crushed");
  Serial.println("******");
  delay(1000);*/
  // put your main code here, to run repeatedly:
}

void createMonsters() {
  for(int i=0; i<NumberOfMonsters; i++) {
    allMonstriks[i] = Kubik (firstKubikX+sideOfKubik*i, firstKubikY+sideOfKubik*i);
  }
}

void drawFigures(int colour) {
  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      ER5517.DrawPixel(adjustCoordX(spaceBullet.allCoords[w][h][xCord]), adjustCoordY(spaceBullet.allCoords[w][h][yCord]), colour);
    }
  }

  for (int i=0; i<NumberOfMonsters; i++) {
    drawKubik(allMonstriks[i], colour);
  }
}

void drawKubik(Kubik instance, int colour) {
  if(instance.isDeleted) {
    return;
  }
  for (int horz=0; horz<sideOfKubik; horz++) {
    for (int ver=0; ver<sideOfKubik; ver++) {
      ER5517.DrawPixel(adjustCoordX(instance.allCoords[horz][ver][xCord]), adjustCoordY(instance.allCoords[horz][ver][yCord]), colour);
    }
  }
}

bool checkCollision(Kubik instance) {
  if(spaceBullet.stopMove || instance.isDeleted) {
    return;
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