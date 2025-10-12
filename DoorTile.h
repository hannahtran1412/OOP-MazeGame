#ifndef DOORTILE_H
#define DOORTILE_H

#include "MazeTile.h"

class Player;
class GameManager;

class DoorTile : public MazeTile {
protected:
    bool triggered; // check if user already interacted with this door

public:
    // constructor
    DoorTile(int r, int c);

    // virtual destructor
    virtual ~DoorTile();

    // override MazeTile method
    bool isWalkable() const override;

    // Interaction key does nothing on doors; they trigger automatically
    void interact(Player& p, GameManager& gm) override;
    
    // pure virtual trigger method for abstract classes (Award and Trap)
    virtual void trigger(Player& p, GameManager& gm) override = 0;
};

// Award
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

// Trap
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