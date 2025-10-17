#include "Award.h"
#include "GameManager.h"
#include "GuardTile.h"

#include <iostream>
using namespace std;

// constructor — initialize values and set triggered = false
Award::Award(int r, int c, double tBonus, int weaken) : DoorTile(r, c), timeBonus(tBonus), guardWeakenAmount(weaken) {
  triggered = false;
}

void Award::trigger(Player& p, GameManager& gm) {
  if (!triggered) {
    triggered = true;
    cout << "Award triggered! +" << timeBonus << " seconds, guard weakened by " << guardWeakenAmount << endl;

    // add time
    gm.addTime(timeBonus);

    // weaken the guard
    gm.weakenGuard(guardWeakenAmount);

    // replace with floor tile
    //gm.replaceWithFloor(getRow(), getCol());
  }
}