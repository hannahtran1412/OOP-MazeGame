#include "FoodTile.h"
#include "Player.h"
#include "GameManager.h"
#include <iostream>

using namespace std;

FoodTile::FoodTile(int r, int c, Food* f) : MazeTile(r, c), food(f) {}

FoodTile::~FoodTile() {
  delete food;
  food = nullptr;
}

bool FoodTile::isWalkable() const {
  return true;
}

void FoodTile::interact(Player& p, GameManager& gm) {
  if (food != nullptr) {
    // add food's nutritionValue value to player's strength
    food->use(p);

    // clean up the food object
    delete food;
    food = nullptr;

    // replace this tile with a floor tile
    gm.replaceWithFloor(getRow(), getCol());

    cout << "Picked up " << food->getName() << "! Strength increased by " << food->getValue() << endl;

  } else {
    cout << "No food left here." << endl;
  }
}

Food* FoodTile::getFood() {
  return food;
}