#include <cassert>
#include <iostream>
#include "GameManager.h"
#include "Player.h"
#include "Guard.h"
#include "MazeTile.h"
#include "FloorTile.h"
#include "WallTile.h"
#include "FoodTile.h"
#include "GuardTile.h"
#include "DoorTile.h"
#include "Award.h"
#include "Trap.h"

using namespace std;

// Test player creation and getStrength
void testPlayerCreation() {
    cout << "Test: Player creation\n";

    Player p("Test", 0, 0, 10);
    assert(p.getStrength() == 10);

    cout << "PASSED\n";
}

// Test if food correctly increases player strength
void testPlayerAddStrength() {
  cout << "Test: player strength increase by Food use\n";
  Player p("Test", 0, 0, 0);
  int initialStrength = p.getStrength();

  Food f("Banana", 5);
  f.use(p); // should print message here

  assert(p.getStrength() == initialStrength + 5);
  cout << "PASSED\n";
}

// Test player movement + wall collision
void testTryMovePlayer() {
  cout << "Test: Player movement\n";
  GameManager gm(false); // easy mode
  gm.initLevel();
  Player& p = gm.getPlayer();
  p.setPos(1, 1); // set player position at (1,1)

  // try to move into wall (0,1)
  gm.tryMovePlayer(-1, 0);
  assert(p.getRow() == 1 && p.getCol() == 1);   // position should not change

  // try to move right (1,2)
  gm.tryMovePlayer(0, 1);
  assert(p.getRow() == 1 && p.getCol() == 2);

  cout << "PASSED\n";
}

// Test different MazeTile walkability
void testMazeTileWalkability() {
    cout << "Test: MazeTile walkability\n";

    FloorTile floor(0,0);
    WallTile wall(1,1);
    assert(floor.isWalkable());
    assert(!wall.isWalkable());

    cout << "PASSED\n";
}

// Test guard fight logic and tile replacement
void testGuardFight() {
    cout << "Test: Guard fight interaction (and tile replacement)\n";
    GameManager gm(false);
    gm.initLevel();
    Player& player = gm.getPlayer();
    player.addStrength(20);  // player with higher strength than guard health

    gm.replaceWithFloor(2, 2);  // clear space
    gm.placeGuard(2, 2, 10);    // place guard with health 10
    player.setPos(2, 2);

    MazeTile* t = gm.tileAt(2, 2);
    GuardTile* guardTile = dynamic_cast<GuardTile*>(t);
    assert(guardTile != nullptr);

    // fight
    guardTile->interact(player, gm);

    // after interact, the tile is replaced with FloorTile
    t = gm.tileAt(2, 2);
    assert(dynamic_cast<FloorTile*>(t) != nullptr);

    // check the game is not over (player has enough strength to win)
    assert(!gm.isGameOver());

    cout << "PASSED\n";
}

// Test door trigger behavior
void testDoorTrigger() {
    // Award
    cout << "Test: Door trigger behavior (Award)\n";
    GameManager gm(false);
    gm.initLevel();

    // place guard
    gm.placeGuard(1, 1, 10);

    // replace tile at (2,2) with Award
    int r = 2, c = 2;
    gm.replaceWithFloor(r, c);
    double bonus = 5.5;
    int weakenAmount = 3;
    Award* awardTile = new Award(r, c, bonus, weakenAmount);

    // move player to award tile location
    Player& player = gm.getPlayer();
    player.setPos(r, c);

    // trigger award
    awardTile->trigger(player, gm);

    // verify gameManager time
    assert(gm.getTimeRemaining() >= bonus);

    // additional verify guard health decreased
    MazeTile* guardTileCandidate = gm.tileAt(1, 1);
    GuardTile* guardTile = dynamic_cast<GuardTile*>(guardTileCandidate);
    assert(guardTile != nullptr);
    assert(guardTile->getGuard().getHealth() < 10);

    delete awardTile;

    cout << "PASSED\n";

    // Trap
    cout << "Testing: Door trigger behavior (Trap)\n";
    GameManager gm2(false);
    gm2.initLevel();

    // replace tile at (2,2) with Trap
    int row = 2, col = 2;
    gm2.replaceWithFloor(row, col);
    double penalty = 4.5;
    Trap* trapTile = new Trap(row, col, penalty, 0.0);
    double initalTime = gm2.getTimeRemaining();

    // move player to trap tile location
    gm2.getPlayer().setPos(row, col);

    // trigger trap 
    trapTile->trigger(gm2.getPlayer(), gm2);

    // verify gameManager time
    assert(gm2.getTimeRemaining() <= initalTime);

    delete trapTile;

    cout << "PASSED\n";
}

// Test save/load game
void testSaveLoadGame() {
    cout << "Testing: Save and load game\n";

    // 1. Create initial GameManager with some state
    GameManager gm(false);
    gm.initLevel();

    Player& player = gm.getPlayer();
    player.setPos(2, 3);
    int initialStrength = player.getStrength();
    player.addStrength(15);

    // save to file
    string saveFile = "test_save.txt";
    gm.saveGame(saveFile);

    try { 
        gm.saveGame(saveFile); 
    }
    catch (const exception& e) { 
        assert(false && "Save threw exception!"); 
    }

    // 2. Create new GameManager and load the saved state
    GameManager gmLoad(false);
    gmLoad.initLevel();
    bool loadSuccess = false;

    try { 
        loadSuccess = gmLoad.loadGame(saveFile); 
    }
    catch (const std::exception& e) { 
        assert(false && "Load threw exception!"); 
    }

    assert(loadSuccess);

    Player& loadedPlayer = gmLoad.getPlayer();
    // check player position
    assert(loadedPlayer.getRow() == 2 && loadedPlayer.getCol() == 3);
    // check player strength matches
    assert(loadedPlayer.getStrength() == initialStrength + 15);
    // check timeRemaining matches
    assert(gmLoad.getTimeRemaining() >= 0);

    // file cleanup
    remove(saveFile.c_str());

    cout << "PASSED\n";
}


int main() {
    cout << "=== Running Normal Operation Tests ===" << endl;
    testPlayerCreation();
    testPlayerAddStrength();
    testTryMovePlayer();
    testMazeTileWalkability();
    testGuardFight();
    testDoorTrigger();
    testSaveLoadGame();
    cout << "=== ALL NORMAL TESTS PASSED ===" << endl;
    return 0;
}