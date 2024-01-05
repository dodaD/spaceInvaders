#ifndef BulletClass_h
#define BulletClass_h

#include <Arduino.h>
#include "Constants.h"

class Bullet {
  public:
    Bullet() {
      // Default constructor body
      // You can initialize member variables if necessary
    }
    Bullet(int x, int y, bool IsMovingUp);
    void deleteOneself();
    void move();
    bool stopMove = false;
    int allCoords[widthOfBullet][heightOfBullet][2];
    void returnBack(int x, int y);
    bool movingUp;
    bool haveReturned;
    bool isDeleted;
  private:
    int coordOfTheTopLeftCorner[2];
};

#endif