#ifndef AWARD_H
#define AWARD_H

#include "DoorTile.h"

class Award : public DoorTile {
private:
  double timeBonus;
  int guardWeakenAmount;

public:
  // constructor
  Award(int r, int c, double tBonus, int weaken);

  // override trigger method from DoorTile
  void trigger(Player& p, GameManager& gm) override;
};

#endif