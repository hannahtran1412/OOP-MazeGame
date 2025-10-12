#include "GuardTile.h"
#include "GameManager.h" 

GuardTile::GuardTile(int r, int c, Guard g) : MazeTile(r, c), guard(g) {
}

bool GuardTile::isWalkable() const {
    return true;
}

void GuardTile::interact(Player& p, GameManager& gm){
  // Pressing SPACE on this tile initiates a fight:
  bool playerWins = guard.fight(p);

  if (playerWins){
    // Tell the GameManager the guard is defeated.
    gm.markGuardDefeated();
    gm.replaceWithFloor(getRow(), getCol());
  }
  else{
    gm.endGameLose();
  }
}

Guard& GuardTile::getGuard() {
    return guard;
}

void GuardTile::weakenGuard(int amt) {
    guard.weaken(amt);
}