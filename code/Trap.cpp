#include "Trap.h"

#include <iostream>

#include "GameManager.h"
using namespace std;

// constructor — initialize values and set triggered = false (untriggered)
Trap::Trap(int r, int c, double tPenalty, double blindSec) : DoorTile(r, c) {
  timePenalty = tPenalty;
  blindSeconds = blindSec;
  triggered = false;  // just to make sure trap doesnt trigger automatically
}

void Trap::trigger(Player& p, GameManager& gm) {
  // only trigger once
  if (!triggered) {    // if the trap hasn't been triggered yet
    triggered = true;  // mark as triggered
    std::cout << "Trap triggered! -" << timePenalty << " seconds, blinded for "
              << blindSeconds << " seconds!" << std::endl;

    // reduce time
    gm.addTime(-timePenalty);

    // blindness effect would be handled in the GUI layer
    gm.applyBlind(blindSeconds);  // start GUI blindness countdown

    // replace with floor tile
    // gm.replaceWithFloor(getRow(), getCol());
  }
}