#include "MonsterClass.h"
#include "BulletClass.h"
#include "Constants.h"

Monster::Monster(int x, int y) {
  for (int horz=0; horz<sideOfKubik; horz++) {
    for (int ver=0; ver<sideOfKubik; ver++) {
      allCoords[horz][ver][0] = x+horz;
      allCoords[horz][ver][1] = y+ver;
    }
  }
  isDeleted = false;
  previousMillis = 0UL;

  pointerToALlBullets[numberOfKubik] = Bullet(x, y, false);
  coordOfTheTopLeftCorner[xCord] = x+sideOfKubik;
  coordOfTheTopLeftCorner[yCord] = y+sideOfKubik;
}

void Monster::deleteMonstrik() {
  this->isDeleted = true;
}

void Monster::move(bool goingWest) {
  if(isDeleted) {
    return;
  }
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
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
