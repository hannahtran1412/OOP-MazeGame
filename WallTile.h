#ifndef WALLTILE_H
#define WALLTILE_H

#include "MazeTile.h"

class WallTile : public MazeTile {
public:
  WallTile(int r, int c);

  virtual bool isWalkable() const override;
  virtual void interact(Player& p, GameManager& gm) override;
};

#endif