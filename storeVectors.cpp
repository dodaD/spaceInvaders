#include "Constants.h"
#include <iostream>
#include <vector>

std::vector<std::vector<int>> victor = {
  {1, 2, 3, 4},
  {5, 6, 7, 8},
  {9, 12, 13, 14},
  {11, 12, 13, 14}
};


std::vector<std::vector<std::array<int, 2>>> allMonstersCoords;

std::vector<std::vector<int>> allBulletsCoords;


void deleteMonster(int row, int column) {
  allMonstersCoords[row].erase(allMonstersCoords[row].begin() + column - 1);
}

void deleteBullet(int row, int column) {
  allBulletsCoords[row].erase(allBulletsCoords[row].begin() + column - 1);
}
/*int main() {
  std::cout << "\n";

  for (std::vector<int> d1: victor)
  for (int d2: d1)
  std::cout << d2 << ' ';

  std::cout << "\n";

  victor[2].erase(victor[2].begin());

  for (std::vector<int> d1: victor)
  for (int d2: d1)
  std::cout << d2 << ' ';

  return 0;
}*/

void lowerMonsters() {
  for (std::vector<std::array<int, 2>>& monster : allMonstersCoords) {
    for (std::array<int, 2>& allCoords : monster) {
      allCoords[1] -= 1;
    }
  }
}

/*void createMonsters() {
  for (std::vector<std::array<int, 2>>& monster : allMonstersCoords) {
    for (std::array<int, 2>& allCoords : monster) {
      allCoords = {4 * 9 + 1, 6 * 7 + 2};
    };
  }
}*/

void createMonsters() {
  for (std::vector<std::array<int, 2>>& monster : allMonstersCoords) {
    for (std::array<int, 2>& allCoords : monster) {
      allCoords = {
      startPositionX + c * sideOfMonster + 1 * c,
      startPositionY + r * sideOfMonster + 2 * r
      };
    };
  }
}
