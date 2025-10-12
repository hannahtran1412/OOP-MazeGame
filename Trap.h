#ifndef TRAP_H
#define TRAP_H

#include "DoorTile.h"

class Trap : public DoorTile {
private:
    double timePenalty;
    double blindSeconds; // (optional) not used in CLI

public:
    Trap(int r, int c, double tPenalty, double blindSec);

    virtual void trigger(Player& p, GameManager& gm) override;
};

#endif