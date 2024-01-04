#include "BulletClass.h"
#include "Constants.h"
#include "arraysToGraph.h"
Bullet allBullets[NumberOfBullets];

Bullet::Bullet(int x, int y, bool IsMovingUp) {
  returnBack(x, y);
  coordOfTheTopLeftCorner[xCord] = x;
  coordOfTheTopLeftCorner[yCord] = y;
  movingUp = IsMovingUp;
};

void Bullet::move() {
  if(stopMove) {
    return;
  }

  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {  
      if(movingUp) {
        allCoords[w][h][yCord] += 1;
      }else {
        allCoords[w][h][yCord] -= 1;
      }
      if(allCoords[w][h][yCord] == 99 || allCoords[w][h][yCord] == 0) {
        stopMove = true;
        returnBack(coordOfTheTopLeftCorner[xCord], coordOfTheTopLeftCorner[yCord]);
        return;
      }
    }
  }
}

void Bullet::returnBack(int x, int y) {
  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      if(movingUp) {
        allCoords[w][h][xCord] = x+w;
        allCoords[w][h][yCord] = y+h;
      }else {
        allCoords[w][h][xCord] = x-w;
        allCoords[w][h][yCord] = y-h;
      }

    }
  }

  coordOfTheTopLeftCorner[xCord] = x+widthOfBullet;
  coordOfTheTopLeftCorner[yCord] = y+heightOfBullet;
  stopMove = false;
  haveReturned = true;
}

