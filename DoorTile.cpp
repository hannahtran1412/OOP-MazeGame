#include "DoorTile.h"
#include "Player.h"
#include "GameManager.h"
#include <iostream>

using namespace std;

DoorTile::DoorTile(int r, int c) : MazeTile(r, c), triggered(false) {}

DoorTile::~DoorTile() {}

bool DoorTile::isWalkable() const {
  return true;
}

void DoorTile::interact(Player& p, GameManager& gm) {
  cout << "Door triggers automatically when you step on it." << endl;
}

