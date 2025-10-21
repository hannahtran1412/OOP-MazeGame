#include "DoorTile.h"

#include <iostream>

#include "GameManager.h"
#include "Player.h"

using namespace std;

// constructor — initialize position and set triggered = false (untriggered)
DoorTile::DoorTile(int r, int c) : MazeTile(r, c), triggered(false) {}

// destructor
DoorTile::~DoorTile() {}

// walkable tile
bool DoorTile::isWalkable() const { return true; }

// doors activate automatically when stepped on
void DoorTile::interact(Player& p, GameManager& gm) {
  cout << "Door triggers automatically when you step on it." << endl;
}
