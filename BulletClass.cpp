#include "BulletClass.h"
#include "Constants.h"

Bullet::Bullet(int x, int y, bool IsMovingUp) {
  returnBack(x, y);
  coordOfTheTopLeftCorner[xCord] = x;
  coordOfTheTopLeftCorner[yCord] = y;
  movingUp = IsMovingUp;
  isDeleted = false;
};

void Bullet::move() {
  if(stopMove || isDeleted) {
    return;
  }

  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      allCoords[w][h][yCord] += (movingUp ? 1 : -1);
      if(allCoords[w][h][yCord] >= 70 || allCoords[w][h][yCord] <= 0) {
        haveReturned = true;
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

