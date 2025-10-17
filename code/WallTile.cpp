#include "WallTile.h"
using namespace std;

WallTile::WallTile(int r, int c) : MazeTile(r, c) {
}

bool WallTile::isWalkable() const {
  return false;
}

void WallTile::interact(Player& p, GameManager& gm) {
  //NOTHING bc its a wall duhh
}