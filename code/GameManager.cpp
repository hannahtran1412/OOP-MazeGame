// GameManager.cpp
#include "GameManager.h"

#include "Player.h"
#include "MazeTile.h"
#include "WallTile.h"
#include "FloorTile.h"
#include "FoodTile.h"
#include "Food.h"
#include "GuardTile.h"
#include "Guard.h"
#include "DoorTile.h"
#include "Award.h"
#include "Trap.h"

#include <cstdlib>   // rand, srand, system
#include <ctime>     // time
#include <iostream>

//file read and write
#include <fstream>   // std::ifstream, std::ofstream
#include <sstream>   // std::istringstream

#include <stdexcept>  // runtime_error

using namespace std;


// ------------------------------
// Constructor / Destructor
// ------------------------------
GameManager::GameManager(bool hard) {
  // set difficulty flags and board size
  hardMode = hard;

  if (hardMode) {
    rows = 15;
    cols = 15;
  } else {
    rows = 10;
    cols = 10;
  }

  if (hardMode) {
    timeRemaining = 60.0;
  } else {
    timeRemaining = 120.0;
  }

  gameOver = false;
  win = false;
  guardDefeated = false;

  // guard location (unknown at start)
  guardRow = -1;
  guardCol = -1;

  // player on heap (matches Player* in header)
  playerPtr = new Player("Hero",1, 1, 0);

  // prepare the maze grid (rows x cols) with nullptr
  maze.resize(rows);
  for (int r = 0; r < rows; r = r + 1) {
    maze[r].resize(cols);
    for (int c = 0; c < cols; c = c + 1) {
      maze[r][c] = NULL;
    }
  }

  // seed the random generator (for doors,food, etc.)
  srand((unsigned)time(NULL));
}

GameManager::~GameManager() {
  // delete all tiles
  for (int r = 0; r < rows; r = r + 1) {
    for (int c = 0; c < cols; c = c + 1) {
        if (maze[r][c] != NULL) {
          delete maze[r][c];
          maze[r][c] = NULL;
        }
    }
  }
  // delete player
  if (playerPtr != NULL) {
    delete playerPtr;
    playerPtr = NULL;
  }
}

// ------------------------------
// Level setup
// ------------------------------
void GameManager::initLevel() {
  // clear old tiles (if any)
  for (int r = 0; r < rows; r = r + 1) {
    for (int c = 0; c < cols; c = c + 1) {
      if (maze[r][c] != NULL) {
        delete maze[r][c];
        maze[r][c] = NULL;
      }
    }
  }

  // build from a prefab layout
  if (!hardMode) {
    vector<string> layout = getEasyLayout();
    buildFromLayout(layout);
  } else {
    vector<string> layout = getHardLayout();
    buildFromLayout(layout);
  }
  int hp = hardMode ? 12 : 8;
  if (guardRow == -1 || guardCol == -1) {
    placeGuard(rows - 2, cols - 2, hp);
  }
    // place foods
  if (hardMode) {
    // scatter a few foods randomly (values 2..4)
    int foodsToPlace = 4;
    int placed = 0;
    int attempts = 0;

  while (placed < foodsToPlace && attempts < 300) {
    attempts = attempts + 1;

    int rr = 1 + (rand() % (rows - 2));
    int cc = 1 + (rand() % (cols - 2));

    MazeTile* t = tileAt(rr, cc);
    if (t != NULL) {
      if (t->isWalkable()) {
        int val = 2 + (rand() % 4); // 2,3,4,5
        placeFood(rr, cc, val);
        placed = placed + 1;
      }
    }
  }
  } 
  else {
    // easy mode: put foods roughly along a gentle path
    //placeFood(2, 2, 2);
    placeFood(4, 4, 3);
    placeFood(3, 3, 1); // value = 1 barely useful
  }

  // spawn doors based on difficulty
  // (fixed slots; place a random count in range)
  // this will overwrite the floor at those positions with Award/Trap
  // only if the slot is walkable
  spawnDoorsForLevel();
}

void GameManager::buildFromLayout(const vector<string>& layout) {
  // Safety: match rows/cols
  if ((int)layout.size() != rows) {
    cout << "Layout row count does not match GameManager rows.\n";
    return;
  }
  for (int r = 0; r < rows; r = r + 1) {
    if ((int)layout[r].size() != cols) {
      cout << "Layout col count does not match GameManager cols at row " << r << ".\n";
      return;
    }
  }

  // Clear guard position
  guardRow = -1;
  guardCol = -1;

  // Clear previous door slots
  doorSlots.clear();

  // Build tiles from characters
  //  '#' = Wall, '.' = Floor, 'P' = Player, 'G' = Guard,'F' = Food
  // 'A' = Award, 'T' = Trap, 'D' = Door (record slot; spawnDoorsForLevel() will decide A/T later)
  for (int r = 0; r < rows; r = r + 1) {
    for (int c = 0; c < cols; c = c + 1) {
      char ch = layout[r][c];

      // delete old tile
      if (maze[r][c] != NULL) {
        delete maze[r][c];
        maze[r][c] = NULL;
      }

      if (ch == '#') {
        maze[r][c] = new WallTile(r, c);
      } else if (ch == '.') {
        maze[r][c] = new FloorTile(r, c);
      } else if (ch == 'P') {
        maze[r][c] = new FloorTile(r, c);
        // set player start here
        playerPtr->setPos(r, c);
      } else if (ch == 'G') {
        maze[r][c] = new FloorTile(r, c);
        guardRow = r;
        guardCol = c;
      } else if (ch == 'F') {
        // place food value (2)
        maze[r][c] = new FoodTile(r, c, new Food("Healing Potion", 2));
      } else if (ch == 'A') {
        // place an Award (values tuned by difficulty)
        double bonus = hardMode ? 12.0 : 10.0;
        int weaken = hardMode ? 2 : 1;
        maze[r][c] = new Award(r, c, bonus, weaken);
      } else if (ch == 'T') {
        double penalty = hardMode ? 20.0 : 10.0;
        double blind = hardMode ? 8.0 : 8.0; // ignored in CLI
        maze[r][c] = new Trap(r, c, penalty, blind);
      } else if (ch == 'D') {
        // put a floor for now, and record the slot;
        // spawnDoorsForLevel() will convert some to Award/Trap
        maze[r][c] = new FloorTile(r, c);
        doorSlots.push_back(make_pair(r, c));
      } else {
        // unknown: default to floor
        maze[r][c] = new FloorTile(r, c);
      }
    }
  }

  // If layout specified guard position, place guard now
  if (inBounds(guardRow, guardCol)) {
    int hp = hardMode ? 12 : 8;
    // Replace whatever is there with GuardTile
    if (maze[guardRow][guardCol] != NULL) {
      delete maze[guardRow][guardCol];
      maze[guardRow][guardCol] = NULL;
    }
    Guard g(hp);
    maze[guardRow][guardCol] = new GuardTile(guardRow, guardCol, g);
  }
}

vector<string> GameManager::getEasyLayout() const {
  // 10x10
  vector<string> L;

  L.push_back("##########");
  L.push_back("#P..#....#");
  L.push_back("#.#.#.##.#");
  L.push_back("#.#...#..#");
  L.push_back("#.###.#F.#");
  L.push_back("#...#.#..#");
  L.push_back("###.#.##.#");
  L.push_back("#..D...#G#");
  L.push_back("#..F#..D.#");
  L.push_back("##########");

  return L;
}

vector<string> GameManager::getHardLayout() const {
  // 15x15
  vector<string> L;

  L.push_back("###############");
  L.push_back("#P..#....#..F.#");
  L.push_back("#.#.#.##.#.#..#");
  L.push_back("#.#...#..#.#D.#");
  L.push_back("#.###.#F.#.#..#");
  L.push_back("#...#.#..#.#..#");
  L.push_back("###.#.##.#.##.#");
  L.push_back("#..D...#......#");
  L.push_back("#..#.#.##.#.###");
  L.push_back("#F.#.#..#.#...#");
  L.push_back("#..#D#..#.#.#.#");
  L.push_back("#.##.#.##.#.#.#");
  L.push_back("#....#....#.#G#");
  L.push_back("#..F#..D..#...#");
  L.push_back("###############");

  return L;
}

// ------------------------------
// DOOR SPAWNING (RANDOM ANYWHERE, EASY: 2-3, HARD: 3-5)
// ------------------------------
void GameManager::spawnDoorsForLevel() {
  // how many doors?
  int minDoorsLocal = hardMode ? 3 : 2;
  int maxDoorsLocal = hardMode ? 5 : 3;
  if (maxDoorsLocal < minDoorsLocal) maxDoorsLocal = minDoorsLocal;

  int range   = (maxDoorsLocal - minDoorsLocal + 1);
  int toPlace = minDoorsLocal + (range > 1 ? rand() % range : 0);

  int placed   = 0;
  int attempts = 0;

  // place doors on random walkable floors (not walls, not guard, not player)
  while (placed < toPlace && attempts < 2000) {
    attempts = attempts + 1;

    int r = 1 + (rand() % (rows - 2)); // avoid borders
    int c = 1 + (rand() % (cols - 2));

    // skip the player's current cell
    if (playerPtr && playerPtr->getRow() == r && playerPtr->getCol() == c) {
      continue;
    }

    MazeTile* t = tileAt(r, c);
    if (t == NULL) continue;
    if (!t->isWalkable()) continue;                 // no walls
    if (dynamic_cast<GuardTile*>(t) != NULL) continue; // don't overwrite guard

    // flip a coin: Award or Trap
    bool makeAward = (rand() % 2 == 0);

    // replace current tile with door (Award or Trap)
    delete maze[r][c];
    maze[r][c] = NULL;

    if (makeAward) {
      double bonus = hardMode ? 12.0 : 10.0;
      int weaken   = hardMode ? 2    : 1;
      maze[r][c]   = new Award(r, c, bonus, weaken);
    } else {
      double penalty = hardMode ? 20.0 : 10.0; // you set 20 hard / 10 easy above
      double blind   = 8.0;                    // blackout seconds (GUI uses this)
      maze[r][c]     = new Trap(r, c, penalty, blind);
    }

    placed = placed + 1;
  }
}

// ------------------------------
// Input and movement
// ------------------------------
void GameManager::handleInput(char key) {
  if (gameOver || win) {
    return;
  }

  if (key == 'w' || key == 'W') {
    tryMovePlayer(-1, 0);
  } else if (key == 's' || key == 'S') {
    tryMovePlayer(1, 0);
  } else if (key == 'a' || key == 'A') {
    tryMovePlayer(0, -1);
  } else if (key == 'd' || key == 'D') {
    tryMovePlayer(0, 1);  
  }
  else if (key == ' ' || key == 'i' || key == 'I') {
    // INTERACT: space (GUI) or I (CLI)
    MazeTile* t = tileAt(playerPtr->getRow(), playerPtr->getCol());
    if (t != NULL) {
      t->interact(*playerPtr, *this);
    }
    checkWinLose();
  } 
  else if (key == 'q' || key == 'Q') {
    endGameLose(); // or set a quit flag; for now, treat as quit/lose
  }
  else if (key == 'p' || key == 'P') {
    saveGame("save.txt");
  }
  else if (key == 'l' || key == 'L') {
    loadGame("save.txt");
  }
}

void GameManager::tryMovePlayer(int dr, int dc) {
  int newRow = playerPtr->getRow() + dr;
  int newCol = playerPtr->getCol() + dc;

  if (!inBounds(newRow, newCol)) {
    return; // out of bounds
  }

  MazeTile* t = tileAt(newRow, newCol);
  if (t == NULL) {
    return;
  }

  if (!t->isWalkable()) {
    return; // wall blocks
  }

  // move player
  playerPtr->setPos(newRow, newCol);

  // stepping on a tile: doors trigger automatically here
  t->trigger(*playerPtr, *this);

  checkWinLose();
}

// ------------------------------
// Rendering (CLI)
// ------------------------------
void GameManager::render() {
#ifdef _WIN32
system("cls");  //window
#else
system("clear");  //mac
#endif

// Symbols:
// '#' = wall, '.' = floor, 'F' = food, 'G' = guard, 'A' = award, 'T' = trap, 'P' = player
for (int r = 0; r < rows; r = r + 1) {
  for (int c = 0; c < cols; c = c + 1) {
    if (playerPtr->getRow() == r && playerPtr->getCol() == c) {
        cout << 'P';
    } 
    else {
      MazeTile* t = maze[r][c];
        if (t == NULL) {
            cout << '?';
        } 
        else {
          if (dynamic_cast<WallTile*>(t) != NULL) {
             cout << '#';
          } 
          else if (dynamic_cast<FoodTile*>(t) != NULL) {
            cout << 'F';
          } 
          else if (dynamic_cast<GuardTile*>(t) != NULL) {
            cout << 'G';
          }
          else if (dynamic_cast<Award*>(t) != NULL) {
            cout << 'A';
          } 
          else if (dynamic_cast<Trap*>(t) != NULL) {
            cout << 'T';
          } 
          else {
            cout << '.';
          }
        }
    }
  }
  cout << "\n";
}

cout << "Strength: " << playerPtr->getStrength()
  << "   Time: " << (int)timeRemaining
  << "   GuardDefeated: " << (guardDefeated ? "yes" : "no")
  << "   (WASD move, SPACE / I interact, Q quit)"
  << endl;

if (win) {
  cout << "YOU WIN!\n";
} 
else if (gameOver) {
  cout << "YOU LOSE!\n";
}
}

// ------------------------------
// Timer / win-lose
// ------------------------------
void GameManager::update(double dt) {
  if (gameOver || win) {
    return;
  }

  timeRemaining = timeRemaining - dt;

  if (timeRemaining <= 0.0) {
    timeRemaining = 0.0;
    gameOver = true;
  }

  if (blindTimer > 0.0) {
    blindTimer = blindTimer - dt;
    if (blindTimer < 0.0) blindTimer = 0.0;
  }

  checkWinLose();
}

void GameManager::checkWinLose() {
  if (guardDefeated) {
    win = true;
  }
}

// ------------------------------
// Helpers
// ------------------------------
bool GameManager::inBounds(int r, int c) const {
  if (r < 0) return false;
  if (c < 0) return false;
  if (r >= rows) return false;
  if (c >= cols) return false;
  return true;
}

MazeTile* GameManager::tileAt(int r, int c) const {
  if (!inBounds(r, c)) {
    return NULL;
  }
  return maze[r][c];
}

void GameManager::replaceWithFloor(int r, int c) {
  if (!inBounds(r, c)) {
    return;
  }
  if (maze[r][c] != NULL) {
    delete maze[r][c];
    maze[r][c] = NULL;
  }
  maze[r][c] = new FloorTile(r, c);
}

// ------------------------------
// Placement helpers
// ------------------------------
void GameManager::placeFood(int r, int c, int val) {
  if (!inBounds(r, c)) {
    return;
  }
  if (maze[r][c] != NULL) {
    delete maze[r][c];
    maze[r][c] = NULL;
  }

  // food values and names
  string name;
  if (val >= 5) {
    name = "Triple Essence";
  } 
  else {
    if (val >= 3) {
        name = "Fire Elixir";
    }
    else {
      name = "Healing Potion";
    }
  }

  Food* f = new Food(name, val);
  maze[r][c] = new FoodTile(r, c, f);
}

void GameManager::placeGuard(int r, int c, int hp) {
  if (!inBounds(r, c)) {
    return;
  }
  if (maze[r][c] != NULL) {
    delete maze[r][c];
    maze[r][c] = NULL;
  }

  Guard g(hp);
  maze[r][c] = new GuardTile(r, c, g);

  guardRow = r;
  guardCol = c;
}

// ------------------------------
// Award/Trap helpers (called by tiles)
// ------------------------------
void GameManager::addTime(double dt) {
  timeRemaining = timeRemaining + dt;
  if (timeRemaining < 0.0) {
    timeRemaining = 0.0;
  }
}

void GameManager::weakenGuard(int amount) {
// Find the guard tile at the tracked location and weaken it
if (!inBounds(guardRow, guardCol)) {
  return;
}

MazeTile* t = tileAt(guardRow, guardCol);
GuardTile* gt = dynamic_cast<GuardTile*>(t);

if (gt != NULL) {
  gt->getGuard().weaken(amount);
}
}

// ------------------------------
// State setters
// ------------------------------
void GameManager::markGuardDefeated() {
  guardDefeated = true;
}

void GameManager::endGameLose() {
  gameOver = true;
}

// ------------------------------
// Save / Load 
// ------------------------------
void GameManager::saveGame(const string& filename) const {
  try {
  // create a file stream (used to write data into a file)
  ofstream out;
  // Open file for writing (overwrite)
  out.open(filename.c_str(), ios::out);
  if (!out.is_open()) {
    throw runtime_error("Could not open save file.");
  }


  // Save basic state (player, time)
  // PLAYER <row> <col> <strength>
  out << "PLAYER " 
    << playerPtr->getRow() << " "
    << playerPtr->getCol() << " "
    << playerPtr->getStrength() << "\n";

  // TIME <seconds_left>
  out << "TIME " << timeRemaining << "\n";

  // GUARD <row> <col> <hp> <defeatedFlag 0/1>
  // If you ever move the guard elsewhere, guardRow/guardCol should track it.
  int defeatedFlag = guardDefeated ? 1 : 0;
  out << "GUARD " << guardRow << " " << guardCol << " ";

  // Try to read current guard HP from the board if present- if defeated, write 0.
  int guardHPToSave = 0;
  MazeTile* gtTile = tileAt(guardRow, guardCol);
  GuardTile* gt = NULL;
  if (gtTile != NULL) {
    gt = dynamic_cast<GuardTile*>(gtTile);
  }
  if (gt != NULL && !guardDefeated) {
    guardHPToSave = gt->getGuard().getHealth();
  }
  out << guardHPToSave << " " << defeatedFlag << "\n";

  // FOOD r c val
  // AWARD r c
  // TRAP r c
  // (Walls/floors are rebuilt automatically on load.)
  for (int r = 0; r < rows; r = r + 1) {
    for (int c = 0; c < cols; c = c + 1) {
      // Skip player position (we already saved it)
      if (playerPtr->getRow() == r && playerPtr->getCol() == c) {
        continue;
      }

      MazeTile* t = maze[r][c];
      if (t == NULL) {
        continue;
      }

      // Don’t serialize walls/floors we rebuild them.
      // 1) Food
      FoodTile* ft = dynamic_cast<FoodTile*>(t);
      if (ft != NULL) {
        Food* f = ft->getFood();
        if (f != NULL) {
          // Save its value
          out << "FOOD " << r << " " << c << " " << f->getValue() << "\n";
        }
        continue;
      }

      // 2) Doors (Award / Trap)
      Award* aw = dynamic_cast<Award*>(t);
      if (aw != NULL) {
        out << "AWARD " << r << " " << c << "\n";
        continue;
      }
      Trap* tr = dynamic_cast<Trap*>(t);
      if (tr != NULL) {
        out << "TRAP " << r << " " << c << "\n";
        continue;
      }

      // 3) Guard — already saved via GUARD line, skip here
      GuardTile* gtt = dynamic_cast<GuardTile*>(t);
      if (gtt != NULL) {
        continue;
      }
    }
  }

  // end of file
  out << "END\n";
  // close file after saving
  out.close();
  cout << "Game saved to " << filename << "\n";
  } 

  catch (const runtime_error& e) {
  // if something goes wrong, show the message we threw earlier
  cout << "Save error: " << e.what() << "\n";
  return;  // saveGame is void
  }

}

bool GameManager::loadGame(const string& filename) {
  try {
  // create a file stream (used to read data from a file)
  ifstream in;
  //open file for read
  in.open(filename.c_str(), ios::in);
  if (!in.is_open()) {
    throw runtime_error("Could not open save file to load.");
  }

  // 1) Wipe current board tiles to avoid leaks
  for (int r = 0; r < rows; r = r + 1) {
    for (int c = 0; c < cols; c = c + 1) {
      if (maze[r][c] != NULL) {
      delete maze[r][c];
      maze[r][c] = NULL;
      }
    }
  }

  // 2) Rebuild base: border walls + inner floor (same as your init walls/floors)
  for (int r = 0; r < rows; r = r + 1) {
    for (int c = 0; c < cols; c = c + 1) {
      bool border = (r == 0 || c == 0 || r == rows - 1 || c == cols - 1);
      if (border) {
        maze[r][c] = new WallTile(r, c);
      } else {
        maze[r][c] = new FloorTile(r, c);
      }
    }
  }

  // 3) Defaults before reading
  int pRow = 1;
  int pCol = 1;
  int pStr = 0;
  double loadedTime = timeRemaining; // keep old if not present
  int gRow = rows - 2;
  int gCol = cols - 2;
  int gHP = 0;
  int gDefeatedFlag = 0;

  // 4) Read line by line
  string line;
    while (getline(in, line)) {
      if (line.size() == 0) {
        continue;
      }
      if (line == "END") {
        break;
      }

      istringstream iss(line);
      string tag;
      iss >> tag;

      if (tag == "PLAYER") {
        iss >> pRow >> pCol >> pStr;
      }
      else if (tag == "TIME") {
        iss >> loadedTime;
      }
      else if (tag == "GUARD") {
        iss >> gRow >> gCol >> gHP >> gDefeatedFlag;
      }
      else if (tag == "FOOD") {
        int r, c, val;
        iss >> r >> c >> val;
        if (inBounds(r, c)) {
          // replace whatever is there with a FoodTile
          if (maze[r][c] != NULL) {
              delete maze[r][c];
              maze[r][c] = NULL;
          }
          maze[r][c] = new FoodTile(r, c, new Food("Food", val));
        }
      }
      else if (tag == "AWARD") {
        int r, c;
        iss >> r >> c;
        if (inBounds(r, c)) {
          if (maze[r][c] != NULL) {
            delete maze[r][c];
            maze[r][c] = NULL;
          }
          // Use simple default award numbers (same each time)
          double bonus = hardMode ? 12.0 : 10.0;
          int weaken = hardMode ? 2 : 1;
          maze[r][c] = new Award(r, c, bonus, weaken);
        }
      }
      else if (tag == "TRAP") {
        int r, c;
        iss >> r >> c;
        if (inBounds(r, c)) {
          if (maze[r][c] != NULL) {
            delete maze[r][c];
            maze[r][c] = NULL;
          }
          double penalty = hardMode ? 10.0 : 8.0;
          double blind = hardMode ? 6.0 : 0.0; // CLI ignores blind
          maze[r][c] = new Trap(r, c, penalty, blind);
        }
      }
    // else ignore
    }

  in.close();

  // 5) Place / remove guard based on flag
  if (gDefeatedFlag == 1) {
    // defeated: mark state + make that tile floor
    guardDefeated = true;
    if (inBounds(gRow, gCol)) {
        if (maze[gRow][gCol] != NULL) {
          delete maze[gRow][gCol];
          maze[gRow][gCol] = NULL;
        }
        maze[gRow][gCol] = new FloorTile(gRow, gCol);
    }
  } 
  else {
    guardDefeated = false;
    if (inBounds(gRow, gCol)) {
      if (maze[gRow][gCol] != NULL) {
        delete maze[gRow][gCol];
        maze[gRow][gCol] = NULL;
      }
      // Recreate guard
      Guard g(gHP);
      maze[gRow][gCol] = new GuardTile(gRow, gCol, g);
    }
  }
  // Track guard coords for “weakenGuard”
  guardRow = gRow;
  guardCol = gCol;

  // 6) Restore player and timer and reset flags
  if (inBounds(pRow, pCol)) {
    playerPtr->setPos(pRow, pCol);
  }
  // Strength:
  // There is no direct setter in your Player (only addStrength),
  // reset to 0, then add up to desired
  {
    // crude reset: move to 0 by subtracting current (if positive)
    int cur = playerPtr->getStrength();
    if (cur > 0) {
        playerPtr->addStrength(-cur);
    }
    playerPtr->addStrength(pStr);
  }

  timeRemaining = loadedTime;
  gameOver = false;
  win = false;

  cout << "Game loaded from " << filename << "\n";
  return true;
  }

  catch (const runtime_error& e) {
  // if something went wrong (like file not found), show message
  cout << "Load error: " << e.what() << "\n";
  return false;
  }
}

// ------------------------------
// Getters
// ------------------------------
int GameManager::getRows() const { return rows; }
int GameManager::getCols() const { return cols; }
double GameManager::getTimeRemaining() const { return timeRemaining; }
bool GameManager::isGameOver() const { return gameOver; }
bool GameManager::isWin() const { return win; }
Player& GameManager::getPlayer() { return *playerPtr; }
const Player& GameManager::getPlayer() const{ return *playerPtr; }