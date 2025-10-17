#ifndef FLOORTILE_H
#define FLOORTILE_H

#include "MazeTile.h"

class FloorTile : public MazeTile {
public:
  FloorTile(int r, int c);

  virtual bool isWalkable() const override;
  virtual void interact(Player& p, GameManager& gm) override;
};

#endif