// MazeTile subclass
// Represent a walkable floor tile with no interaction
#ifndef FLOORTILE_H
#define FLOORTILE_H

#include "MazeTile.h"

class FloorTile : public MazeTile {
 public:
  // constructor
  FloorTile(int r, int c);

  // override MazeTile method
  virtual bool isWalkable() const override;

  virtual void interact(Player& p, GameManager& gm) override;  // does nothing
};

#endif