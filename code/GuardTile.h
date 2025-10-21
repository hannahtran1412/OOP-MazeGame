// MazeTile subclass
// Holds a guard object, allows player to fight guard
#ifndef GUARDTILE_H
#define GUARDTILE_H

#include "Guard.h"
#include "MazeTile.h"

class GameManager;  // forward declare

class GuardTile : public MazeTile {
 protected:
  Guard guard;

 public:
  GuardTile(int r, int c, Guard g);
  bool isWalkable() const override;
  
  // initiates fight, triggers win or lose in GameManager
  void interact(Player& p, GameManager& gm) override;

  Guard& getGuard();          // returns reference to internal Guard
  void weakenGuard(int amt);  // convenience that calls guard.weaken(amt)
};

#endif