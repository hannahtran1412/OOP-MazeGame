#include "WallTile.h"
using namespace std;

// constructor — initialize position
WallTile::WallTile(int r, int c) : MazeTile(r, c) {}

// unwalkable tile
bool WallTile::isWalkable() const { return false; }

void WallTile::interact(Player& p, GameManager& gm) {
  // Wall does nothing on interaction
}