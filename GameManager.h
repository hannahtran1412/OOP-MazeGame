#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <iostream>
#include <vector>
using namespace std;

// Forward declarations
class MazeTile;
class Player;

class GameManager {
private:
  int rows;
  int cols;

  // 2D grid of raw pointers to tiles
  vector< vector<MazeTile*> > maze;

  Player* playerPtr; // Player object 

  double timeRemaining;
  bool hardMode;
  bool gameOver;
  bool win;
  bool guardDefeated;

  // guard's location so we can weaken it (Award)
  int guardRow;
  int guardCol;

  // List of possible door coordinates
  std::vector<std::pair<int, int>> doorSlots; 
  int minDoors;
  int maxDoors;

public:
  GameManager(bool hard);

  // Level setup
  void initLevel();
  void spawnDoorsForLevel();

  // Input and movement
  void handleInput(char key);
  void tryMovePlayer(int dr, int dc); //dr,dc = delta row, delta column

  // Rendering (simple CLI debug)
  void render();

  // Timer and game checks
  void update(double dt);
  void checkWinLose();

  // Helpers
  bool inBounds(int r, int c) const;
  MazeTile* tileAt(int r, int c) const;
  void replaceWithFloor(int r, int c);

  // Place items
  void placeFood(int r, int c, int val); //value
  void placeGuard(int r, int c, int hp); //hp

  // Award/Trap helpers
  void addTime(double dt);
  void weakenGuard(int amount);

  // State setters (called by tiles)
  void markGuardDefeated();
  void endGameLose();

  // Save / load using txt file
  void saveGame(const std::string& filename) const;
  bool loadGame(const std::string& filename);

  // getters
  int getRows() const;
  int getCols() const;
  double getTimeRemaining() const;
  bool isGameOver() const;
  bool isWin() const;

  // Access to player for tiles
  Player& getPlayer();

  ~GameManager();
};

#endif
