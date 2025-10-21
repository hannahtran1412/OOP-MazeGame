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
  try {
    // if there is no food, throw an error
    if (food == nullptr) {
      throw runtime_error("No food left here.");
    }

    // store info before deleting
    string foodName = food->getName();
    int foodValue = food->getValue();

    // apply effect to player (adds strength)
    food->use(p);

    // delete food to avoid reusing
    delete food;
    food = nullptr;

    cout << "Picked up " << foodName << "! Strength increased by " << foodValue << endl;

    // replace with floor tile after pickup
    gm.replaceWithFloor(getRow(), getCol());
  }

  catch (const runtime_error& e) {
    cout << "Food error: " << e.what() << endl;
  }
}

Food* FoodTile::getFood() { return food; }