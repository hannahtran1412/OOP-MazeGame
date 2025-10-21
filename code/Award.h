// Subclass of DoorTile
// Increases time and decreases guard strength when triggered
#ifndef AWARD_H
#define AWARD_H

#include "DoorTile.h"

class Award : public DoorTile {
 private:
  double timeBonus;       // time added to the timer
  int guardWeakenAmount;  // amount to weaken the guard’s strength

 public:
  // constructor
  Award(int r, int c, double tBonus, int weaken);

  // override trigger method from DoorTile
  void trigger(Player& p, GameManager& gm) override;
};

#endif