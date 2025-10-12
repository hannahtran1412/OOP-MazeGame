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

  // Store name and value BEFORE deleting food
  string foodName = food->getName();
  int foodValue = food->getValue();

  // apply the food effect to player (this already prints collected message if enabled)
  food->use(p);

  // delete food object
  delete food;
  food = nullptr;

  // replace with floor tile
  gm.replaceWithFloor(getRow(), getCol());

  // Now safe to print info using stored values
  cout << "Picked up " << foodName << "! Strength increased by " << foodValue << endl;

  } 
  else {
    cout << "No food left here." << endl;
  }
}

Food* FoodTile::getFood() {
  return food;
}