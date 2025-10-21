#include "MazeTile.h"
using namespace std;

// constructor — initialize position
MazeTile::MazeTile(int r, int c) {
  row = r;
  col = c;
}

int MazeTile::getRow() const { return row; }

int MazeTile::getCol() const { return col; }

void MazeTile::interact(Player& p, GameManager& gm) {
  // default- nothing, RMB ONLY FOR INTERACTIVE TILES
}

void MazeTile::trigger(Player& p, GameManager& gm) {
  // default- nothing, ONLY FOR DOORS
}

// destructor
MazeTile::~MazeTile() {}
