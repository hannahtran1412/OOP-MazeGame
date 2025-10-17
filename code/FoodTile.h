#ifndef FOODTILE_H
#define FOODTILE_H

#include "MazeTile.h"
#include "Food.h"

class Player;
class GameManager;

class FoodTile : public MazeTile {
private:
  Food* food;

public:
  // constructor
  FoodTile(int r, int c, Food* f);

  // destructor
  ~FoodTile();
  
  // override pure virtual methods from MazeTile
  bool isWalkable() const override;

  // interaction when player presses spacebar
  void interact(Player& p, GameManager& gm) override;

  // getter for the food
  Food* getFood();
};

#endif