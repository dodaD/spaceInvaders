#ifndef Display_h
#define Display_h

#include <Arduino.h>

class Display {
  public:
    Display(){
      //hz
    };
    void drawPixel(int x, int y, int colour); 
};
#endif