#include "Award.h"
#include "GameManager.h"
#include "GuardTile.h"
using namespace std;

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