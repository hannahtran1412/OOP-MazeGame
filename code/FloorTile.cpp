#include "FloorTile.h"
using namespace std;

FloorTile::FloorTile(int r, int c) : MazeTile(r, c) {
}

bool FloorTile::isWalkable() const {
  return true;
}

void FloorTile::interact(Player& p, GameManager& gm) {
  //NOTHING bc normal floor does nothing
}