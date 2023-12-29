#include "BulletClass.h"
#include "Constants.h"

Bullet::Bullet(int x, int y) {
  returnBack(x, y);
  coordOfTheTopLeftCorner[xCord] = x;
  coordOfTheTopLeftCorner[yCord] = y;
};

void Bullet::move() {
  if(stopMove) {
    return;
  }
  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {  
      allCoords[w][h][yCord] += 1;
    }
  }
}

void Bullet::returnBack(int x, int y) {
  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      allCoords[w][h][xCord] = x+w;
      allCoords[w][h][yCord] = y+h;
    }
  }

  coordOfTheTopLeftCorner[xCord] = x+widthOfBullet;
  coordOfTheTopLeftCorner[yCord] = y+heightOfBullet;
}


