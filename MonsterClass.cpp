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
}

void Kubik::deleteMonstrik() {
  this->isDeleted = true;
}