// Base class for all maze tiles
// Stores grid layout
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
  // constructor
  MazeTile(int r, int c);
  // virtual destructor
  virtual ~MazeTile();

  int getRow() const;
  int getCol() const;

  // returns if the tile is walkable (PURE virtual)
  virtual bool isWalkable() const = 0;

  // SPACE - override for interactive tiles (default does nothing)
  virtual void interact(Player& p, GameManager& gm);

  // trigger for doors (default does nothing)
  // DO NOT PURE virtual so only DoorTile needs to override
  virtual void trigger(Player& p, GameManager& gm);
};

#endif
