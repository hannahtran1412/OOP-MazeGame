// Represents guard with a health and defeat state
#ifndef GUARD_H
#define GUARD_H

#include "Player.h"

class Guard {
 protected:
  int health;
  bool defeated;  // if guard is defeated
 public:
  Guard(int hp);
  bool fight(Player& p);  // returns true if player defeats guard
  void weaken(int amount);
  int getHealth() const;
  bool isDefeated() const;
};

#endif