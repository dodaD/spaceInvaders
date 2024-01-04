#include "MonsterClass.h"
#include "BulletClass.h"
#include "Constants.h"
#include "arraysToGraph.h"

Kubik::Kubik(int x, int y, int n) {
  for (int horz=0; horz<sideOfKubik; horz++) {
    for (int ver=0; ver<sideOfKubik; ver++) {
      allCoords[horz][ver][0] = x+horz;
      allCoords[horz][ver][1] = y+ver;
    }
  }
  isDeleted = false;
  previousMillis = 0UL;
  goingWest = true;
  numberOfKubik = n;
  allBullets[numberOfKubik] = Bullet(x, y, false);
  coordOfTheTopLeftCorner[xCord] = x+sideOfKubik;
  coordOfTheTopLeftCorner[yCord] = y+sideOfKubik;
}

void Kubik::deleteMonstrik() {
  this->isDeleted = true;
}

void Kubik::move() {
  if(isDeleted) {
    return;
  }
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    if(allCoords[0][0][0] <= moveDistance*numberOfKubik+sideOfKubik*numberOfKubik) {
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
