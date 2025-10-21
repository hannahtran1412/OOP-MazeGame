#include "Guard.h"

// constructor — initialize health and defeat state
Guard::Guard(int hp) {
  this->health = hp;
  this->defeated = false;
}

bool Guard::fight(Player& p) {
  if (p.getStrength() >= health) {
    defeated = true;
    return true;
  }
  return false;
}

void Guard::weaken(int amount) {
  this->health -= amount;
  if (health < 0) {
    health = 0;
  }
}

int Guard::getHealth() const { return health; }

bool Guard::isDefeated() const { return defeated; }