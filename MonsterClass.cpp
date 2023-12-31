#include "MonsterClass.h"
#include "Constants.h"

Kubik::Kubik(int x, int y) {
  for (int horz=0; horz<sideOfKubik; horz++) {
    for (int ver=0; ver<sideOfKubik; ver++) {
      allCoords[horz][ver][0] = x+horz;
      allCoords[horz][ver][1] = y+ver;
    }
  }
  isDeleted = false;
  coordOfTheTopLeftCorner[xCord] = x+sideOfKubik;
  coordOfTheTopLeftCorner[yCord] = y+sideOfKubik;
  goingWest = true;
}

void Kubik::deleteMonstrik() {
  this->isDeleted = true;
}

unsigned long previousMillis = 0UL;
unsigned long interval = 500UL;

void Kubik::move() {
  if(isDeleted) {
    return;
  }
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    if(allCoords[0][0][0] <= moveDistance) {
      goingWest = false;
    }else if(allCoords[sideOfKubik-1][sideOfKubik-1][0] >= 70) {
      goingWest = true;
    }
  
    for (int horz=0; horz<sideOfKubik; horz++) {
      for (int ver=0; ver<sideOfKubik; ver++) {
        if (goingWest) {
          allCoords[horz][ver][0] -= moveDistance;
        } else {
          allCoords[horz][ver][0] += moveDistance;
        }
      }
    }
 	  previousMillis = currentMillis;
  }
}