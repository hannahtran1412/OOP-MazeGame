// Subclass of DoorTile
// Decreases time and disables map visibility when triggered
#ifndef TRAP_H
#define TRAP_H

#include "DoorTile.h"

class Trap : public DoorTile {
 private:
  double timePenalty;   // time reduced from the timer
  double blindSeconds;  // (optional) not used in CLI

 public:
  // constructor
  Trap(int r, int c, double tPenalty, double blindSec);

  // override trigger method from DoorTile
  virtual void trigger(Player& p, GameManager& gm) override;
};

#endif