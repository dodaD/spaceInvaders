#ifndef MonsterClass_h
#define MonsterClass_h

#include "BulletClass.h"
#include "Constants.h"
#include <Arduino.h>

class Monster {
public:
  Monster() {
    isDeleted = false;
  }
  void deleteMonstrik();
  bool isDeleted;
};

#endif
