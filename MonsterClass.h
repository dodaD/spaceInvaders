#ifndef MonsterClass_h
#define MonsterClass_h

#include "Constants.h"
#include <Arduino.h>

class Kubik {
  public:
    Kubik() {
        // Default constructor body
        // You can initialize member variables if necessary
    }
    Kubik(int x, int y);
    int allCoords[ sideOfKubik ][ sideOfKubik ][2];
    void deleteMonstrik();
    bool isDeleted;
  private:
    int coordOfTheTopLeftCorner[2];
};

#endif