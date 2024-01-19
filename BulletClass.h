#ifndef BulletClass_h
#define BulletClass_h

#include <Arduino.h>
#include "Constants.h"

class Bullet {
  public:
    Bullet() {
      isDeleted = false;
      stopMove = false;
    }
    void delete();
    bool isDeleted;
    bool stopMove;
};

#endif
