// MazeTile subclass
// Representing an unwalkable wall tile with no interaction.
#ifndef WALLTILE_H
#define WALLTILE_H

#include "MazeTile.h"

class WallTile : public MazeTile {
 public:
  // constructor
  WallTile(int r, int c);

  // override MazeTile method
  virtual bool isWalkable() const override;
  virtual void interact(Player& p, GameManager& gm) override;
};

#endif