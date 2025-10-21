#include "FloorTile.h"
using namespace std;

// constructor — initialize position
FloorTile::FloorTile(int r, int c) : MazeTile(r, c) {}

// walkable tile
bool FloorTile::isWalkable() const { return true; }

void FloorTile::interact(Player& p, GameManager& gm) {
  // Floor does nothing on interaction
}