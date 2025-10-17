#include "MazeTile.h"
using namespace std;

MazeTile::MazeTile(int r, int c) {
  row = r;
  col = c;
}

int MazeTile::getRow() const {
  return row;
}

int MazeTile::getCol() const {
  return col;
}

void MazeTile::interact(Player& p, GameManager& gm) {
  // default- nothing, RMB ONLY FOR INTERACTIVE TILES
}

void MazeTile::trigger(Player& p, GameManager& gm) {
  // default- nothing, ONLY FOR DOORS
}

MazeTile::~MazeTile() {
}
