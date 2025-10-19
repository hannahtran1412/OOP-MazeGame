#include <cassert>
#include <iostream>
#include "GameManager.h"
#include "Player.h"
#include "Guard.h"
#include "MazeTile.h"
#include "WallTile.h"
#include "FoodTile.h"
#include "GuardTile.h"
#include "DoorTile.h"
#include "Award.h"
#include "Trap.h"

using namespace std;

// Test negative position values
void testMovementInvalidCoordinates() {
  cout << "Test: Negative position\n";
  Player p("Test", 0, 0, 0);
  p.setPos(-2,-4);

  // invalid position, must clamp to >=0
  assert(p.getRow() >= 0 && p.getCol() >= 0);

  cout << "PASSED\n";
}

// Test out-of-bounds movement
void testMovementOutOfBounds() {
  cout << "Test: out-of-bounds movement\n";
  GameManager gm(false);
  gm.initLevel();

  Player& p = gm.getPlayer();
  p.setPos(0,0);

  gm.tryMovePlayer(-1,0); // up out-of-bounds
  assert(p.getRow() == 0);
  gm.tryMovePlayer(0,-1); // left out-of-bounds
  assert(p.getCol() == 0);

  cout << "PASSED\n";
}

// Test food interaction when tile has null (no food)
void testInvalidFoodPickup() {
  cout << "Test: Food pickup on empty tile\n";
  Player p("Test", 0, 0, 0);
  FoodTile foodTile(0, 0, nullptr);

  int initialStrength = p.getStrength();

  GameManager gm(false); // should print message here
  gm.initLevel();

  foodTile.interact(p, gm); // Pass the valid reference

  assert(p.getStrength() == initialStrength);
  cout << "PASSED\n";
}

void testFileLoadNonexistent() {
  cout << "Test: Load non-existent save file\n";
  GameManager gm(false);
  bool loaded = false;

  try { 
    loaded = gm.loadGame("does_not_exist.txt"); 
  }
  catch (...) { 
    loaded = false; 
  }

  assert(!loaded);

  cout << "PASSED\n";
}


int main() {
  cout << "=== Running Edge Case Tests ===" << endl;
  testMovementInvalidCoordinates();
  testMovementOutOfBounds();
  testInvalidFoodPickup();
  testFileLoadNonexistent();
  cout << "=== ALL EDGE CASE TESTS PASSED ===" << endl;
  return 0;
}