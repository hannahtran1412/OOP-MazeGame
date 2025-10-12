#include "GuardTile.h"

GuardTile::GuardTile(int r, int c, Guard g) : MazeTile(r, c), guard(g) {
}

bool GuardTile::isWalkable() const {
    return true;
}

void GuardTile::interact(Player& p, GameManager& gm) {
    if (guard.fight(p)) {
        gm.guardDefeated = true;
        gm.replaceWithFloor(row, col);
    } else {
        gm.gameOver = true;
    }
}