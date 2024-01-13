#ifndef MonsterClass_h
#define MonsterClass_h

#include "BulletClass.h"
#include "Constants.h"
#include <Arduino.h>

class Kubik {
public:
  Monster() {
    // Default constructor body
    // You can initialize member variables if necessary
  }
  void deleteMonstrik();
  void move(bool goingWest);
  bool isDeleted;
  unsigned long previousMillis;
  int coordOfTheTopLeftCorner[2]; // TODO: should be in sketch.ino
};

#endif
