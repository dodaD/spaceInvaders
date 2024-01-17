#ifndef MonsterClass_h
#define MonsterClass_h

#include "BulletClass.h"
#include "Constants.h"
#include <Arduino.h>

class Monster {
public:
  Monster(){
    // Default constructor body
    // You can initialize member variables if necessary
  }
  void deleteMonstrik();
  void move(char directionOfMovement);
  bool isDeleted;
  unsigned long previousMillis;
  int coordOfTheTopLeftCorner[2]; // TODO: should be in sketch.ino
};

#endif
