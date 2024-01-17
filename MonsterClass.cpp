#include "MonsterClass.h"
#include "BulletClass.h"
#include "Constants.h"

Monster::Monster(int x, int y) {
  for (int horz=0; horz<sideOfMonster; horz++) {
    for (int ver=0; ver<sideOfMonster; ver++) {
      allCoords[horz][ver][0] = x+horz;
      allCoords[horz][ver][1] = y+ver;
    }
  }
  isDeleted = false;
  previousMillis = 0UL;

  pointerToALlBullets[numberOfMonster] = Bullet(x, y, false);
  coordOfTheTopLeftCorner[xCoord] = x+sideOfMonster;
  coordOfTheTopLeftCorner[yCoord] = y+sideOfMonster;
}

void Monster::deleteMonstrik() {
  this->isDeleted = true;
}

void Monster::move(char directionOfMovement) {
  if(isDeleted) {
    return;
  }
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    for (int horz=0; horz<sideOfMonster; horz++) {
      for (int ver=0; ver<sideOfMonster; ver++) {
        if (directionOfMovement == 'L') {
          allCoords[horz][ver][0] -= moveDistance;
        } else {
          allCoords[horz][ver][0] += moveDistance;
        }
      }
    }
 	  previousMillis = currentMillis;
  }
}
