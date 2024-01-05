#ifndef ShipClass_h
#define ShipClass_h

#include <Arduino.h>

class Ship {
  public:
    Ship() {
        // Default constructor body
        // You can initialize member variables if necessary
    }
    Ship(int x, int y);
    int allCoords[shipWidth][shipHeight][2];
    void move(bool goingWest);
    bool stopMove 
  private:
    int coordOfTheTopLeftCorner[2];
};

#endif