#include "Trap.h"
#include "GameManager.h"
using namespace std;

Trap::Trap(int r, int c, double tPenalty, double blindSec) : DoorTile(r, c) {
  timePenalty = tPenalty;
  blindSeconds = blindSec;
}

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