#ifndef GUARDTILE_H
#define GUARDTILE_H

#include "MazeTile.h"
#include "Guard.h"
#include "GameManager.h"


class GuardTile : public MazeTile {
protected:
    Guard guard;
public:
    GuardTile(int r, int c, Guard g);
    bool isWalkable() const override;
    void interact(Player& p, GameManager& gm) override;
    Guard& getGuard();          // returns reference to internal Guard
    void weakenGuard(int amt);  // convenience that calls guard.weaken(amt)
};

#endif