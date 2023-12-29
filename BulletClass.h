#ifndef BulletClass_h
#define BulletClass_h

#include <Arduino.h>
#include "Constants.h"

class Bullet {
  public:
    Bullet(int x, int y);
    void deleteOneself();
    void move();
    bool stopMove = false;
    int allCoords[widthOfBullet][heightOfBullet][2];
    void returnBack(int x, int y);
  private:
    int coordOfTheTopLeftCorner[2];
};

#endif