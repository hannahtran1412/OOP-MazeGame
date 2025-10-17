#ifndef GUARD_H
#define GUARD_H

#include "Player.h"

class Guard {
protected:
    int health;
    bool defeated;
public:
    Guard(int hp);
    bool fight(Player& p);
    void weaken(int amount);
    int getHealth() const;
    bool isDefeated() const;
};

#endif