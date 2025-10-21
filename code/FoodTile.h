// MazeTile subclass
// Holds a Food object, allows player interaction to consume it
#ifndef FOODTILE_H
#define FOODTILE_H

#include "Food.h"
#include "MazeTile.h"

class Player;
class GameManager;

class FoodTile : public MazeTile {
 private:
  Food* food;  // pointer to food on this tile

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