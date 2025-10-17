#ifndef MAZETILE_H
#define MAZETILE_H

#include <iostream>
using namespace std;

class Player;
class GameManager;

class MazeTile {
protected:
  int row;
  int col;

public:
  MazeTile(int r, int c);
  virtual ~MazeTile();

  int getRow() const;
  int getCol() const;

  // PURE virtual bc each tile decides walkability
  virtual bool isWalkable() const = 0;

  // SPACE- default does nothing, override for interactive tiles
  virtual void interact(Player& p, GameManager& gm);

  // trigger for doors- default doesnothing, DO NOT PURE virtual so only DoorTile needs to override.
  virtual void trigger(Player& p, GameManager& gm);
};

#endif
