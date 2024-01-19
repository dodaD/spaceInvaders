#include "BulletClass.h"
#include "Constants.h"

/*void Bullet::move() {
  if(stopMove || isDeleted) {
    return;
  }

  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      allCoords[w][h][yCoord] += (movingUp ? 1 : -1);
      if(allCoords[w][h][yCoord] >= 70 || allCoords[w][h][yCoord] <= 0) {
        haveReturned = true;
      }
    }
  }
})*/

void Bullet::delete() {
  isDeleted = true;
}

/*void Bullet::returnBack(int x, int y) {
  for (int w=0; w<widthOfBullet; w++) {
    for (int h=0; h<heightOfBullet; h++) {
      if(movingUp) {
        allCoords[w][h][xCoord] = x+w;
        allCoords[w][h][yCoord] = y+h;
      }else {
        allCoords[w][h][xCoord] = x-w;
        allCoords[w][h][yCoord] = y-h;
      }
    }
  }

  coordOfTheTopLeftCorner[xCoord] = x+widthOfBullet;
  coordOfTheTopLeftCorner[yCoord] = y+heightOfBullet;
  stopMove = false;
  haveReturned = true;
}*/

