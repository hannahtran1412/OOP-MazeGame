#include "FoodTile.h"

#include <iostream>

#include "GameManager.h"
#include "Player.h"

using namespace std;

// constructor
FoodTile::FoodTile(int r, int c, Food* f) : MazeTile(r, c), food(f) {}

// destructor
FoodTile::~FoodTile() {
  delete food;
  food = nullptr;
}

// walkable tile
bool FoodTile::isWalkable() const { return true; }

void FoodTile::interact(Player& p, GameManager& gm) {
  if (food != nullptr) {
    // Store name and value BEFORE deleting food
    string foodName = food->getName();
    int foodValue = food->getValue();

    // apply the food effect to player (prints collected message if enabled)
    food->use(p);

    // delete food object
    delete food;
    food = nullptr;

    // Now safe to print info using stored values
    cout << "Picked up " << foodName << "! Strength increased by " << foodValue
         << endl;

    // replace with floor tile
    gm.replaceWithFloor(getRow(), getCol());

    return;
  } else {
    cout << "No food left here." << endl;
  }
}

Food* FoodTile::getFood() { return food; }