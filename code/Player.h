// Represents the player with name, position and strength
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>

class Player {
 protected:
  std::string name;
  int row;
  int col;
  int strength;

 public:
  Player(std::string name, int startR = 1, int startC = 1, int startStr = 0);
  int getRow() const;
  int getCol() const;
  int getStrength() const;
  void setPos(int r, int c);
  void addStrength(int amount);
};

#endif