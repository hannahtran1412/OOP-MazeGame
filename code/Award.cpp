#include "Award.h"

#include <iostream>

#include "GameManager.h"
#include "GuardTile.h"
using namespace std;

// constructor — initialize values and set triggered = false
Award::Award(int r, int c, double tBonus, int weaken)
    : DoorTile(r, c), timeBonus(tBonus), guardWeakenAmount(weaken) {
  triggered = false;
}

void Award::trigger(Player& p, GameManager& gm) {
  // only trigger once
  if (!triggered) {    // if the award hasn't been triggered yet
    triggered = true;  // mark as triggered
    cout << "Award triggered! +" << timeBonus << " seconds, guard weakened by "
         << guardWeakenAmount << endl;

    // add time
    gm.addTime(timeBonus);

    // weaken the guard
    gm.weakenGuard(guardWeakenAmount);

    // replace with floor tile
    // gm.replaceWithFloor(getRow(), getCol());
  }
}