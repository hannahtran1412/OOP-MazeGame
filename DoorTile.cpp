#include "DoorTile.h"
#include "Player.h"
#include "GameManager.h"
#include <iostream>

using namespace std;

DoorTile::DoorTile(int r, int c) : MazeTile(r, c), triggered(false) {}

DoorTile::~DoorTile() {}

bool DoorTile::isWalkable() const {
    return true;
}

void DoorTile::interact(Player& p, GameManager& gm) {
    cout << "Door triggers automatically when you step on it." << endl;
}


// Award implementations

Award::Award(int r, int c, double tBonus, int weaken)
    : DoorTile(r, c), timeBonus(tBonus), guardWeakenAmount(weaken) {}

void Award::trigger(Player& p, GameManager& gm) {
    if (!triggered) {
        triggered = true;
        cout << "Award triggered! +" << timeBonus << " seconds, guard weakened by " << guardWeakenAmount << endl;

        // add time
        gm.addTime(timeBonus);

        // weaken the guard
        gm.weakenGuard(guardWeakenAmount);

        // replace with floor tile
        gm.replaceWithFloor(getRow(), getCol());
    }
}

// Trap implementations

Trap::Trap(int r, int c, double tPenalty, double blindSec)
    : DoorTile(r, c), timePenalty(tPenalty), blindSeconds(blindSec) {}

void Trap::trigger(Player& p, GameManager& gm) {
    if (!triggered) {
        triggered = true;
        std::cout << "Trap triggered! -" << timePenalty << " seconds, blinded for " << blindSeconds << " seconds!" << std::endl;

        // reduce time
        gm.addTime(-timePenalty);

        // blindness effect would be handled in the GUI layer

        // replace with floor tile
        gm.replaceWithFloor(getRow(), getCol());
    }
}
