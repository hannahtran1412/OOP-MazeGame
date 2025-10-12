#ifndef TRAP_H
#define TRAP_H

#include "DoorTile.h"

class Trap : public DoorTile {
private:
    double timePenalty;
    double blindSeconds;

public:
    // constructor
    Trap(int r, int c, double tPenalty, double blindSec);

    // override trigger method from DoorTile
    void trigger(Player& p, GameManager& gm) override;
};

#endif