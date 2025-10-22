// main_raylib.cpp
#include "GameManager.h"
#include "MazeTile.h"
#include "WallTile.h"
#include "FloorTile.h"
#include "FoodTile.h"
#include "GuardTile.h"
#include "DoorTile.h"  // for checking isTriggered()
#include "Award.h"
#include "Trap.h"
#include "Food.h"
#include "Menu.h" // Added for menu system

#include "raylib.h"
#include <string>
#include <algorithm>

// ---------- texture pack ----------
struct Textures {
    Texture2D wall{};
    Texture2D floor{};
    Texture2D player{};
    Texture2D guard{};

    Texture2D foodBanana{};
    Texture2D foodFish{};
    Texture2D foodTomato{};

    Texture2D doorClosed{};
    Texture2D doorOpen{};
};

static Textures gTex;

static void LoadAllTextures() {
    gTex.wall       = LoadTexture("assets/wall.png");
    gTex.floor      = LoadTexture("assets/floor.png");
    gTex.player     = LoadTexture("assets/player.png");
    gTex.guard      = LoadTexture("assets/guard.png");

    gTex.foodBanana = LoadTexture("assets/banana.png");
    gTex.foodFish   = LoadTexture("assets/fish.png");
    gTex.foodTomato = LoadTexture("assets/tomato.png");

    gTex.doorClosed = LoadTexture("assets/door_closed.png");
    gTex.doorOpen   = LoadTexture("assets/door_open.png");
}

static void UnloadAllTextures() {
    UnloadTexture(gTex.wall);
    UnloadTexture(gTex.floor);
    UnloadTexture(gTex.player);
    UnloadTexture(gTex.guard);
    UnloadTexture(gTex.foodBanana);
    UnloadTexture(gTex.foodFish);
    UnloadTexture(gTex.foodTomato);
    UnloadTexture(gTex.doorClosed);
    UnloadTexture(gTex.doorOpen);
}

// Draw a texture centered inside a grid cell
static void DrawInCell(const Texture2D& tex, int r, int c, int cell, int margin) {
    if (tex.id == 0) return; // not loaded
    Rectangle src = { 0, 0, (float)tex.width, (float)tex.height };
    float scale = (float)cell / (float)std::max(tex.width, tex.height);
    float w = tex.width * scale;
    float h = tex.height * scale;
    float x = (float) (margin + c*cell) + (cell - w) * 0.5f;
    float y = (float) (margin + r*cell) + (cell - h) * 0.5f;
    Rectangle dst = { x, y, w, h };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
}

static void DrawGame(GameManager& gm, int cell, int margin) {
    const int rows = gm.getRows();
    const int cols = gm.getCols();

    // board background panel
    DrawRectangle(margin-4, margin-4, cols*cell+8, rows*cell+8, Color{28,28,34,255});

    // draw cells
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            MazeTile* t = gm.tileAt(r, c);

            // floor base
            if (dynamic_cast<WallTile*>(t)) {
                DrawInCell(gTex.wall, r, c, cell, margin);
                continue;
            } else {
                // draw floor under everything else
                DrawInCell(gTex.floor, r, c, cell, margin);
            }

            // door (Award or Trap both subclass DoorTile)
            if (auto door = dynamic_cast<DoorTile*>(t)) {
                // show closed until triggered, then open
                if (door->isTriggered()) {
                    DrawInCell(gTex.doorOpen, r, c, cell, margin);
                } else {
                    DrawInCell(gTex.doorClosed, r, c, cell, margin);
                }
            }

            // food
            if (auto ft = dynamic_cast<FoodTile*>(t)) {
                Food* f = ft->getFood();
                if (f) {
                    std::string name = f->getName();
                    // pick texture by name
                    if (name == "Banana") {
                        DrawInCell(gTex.foodBanana, r, c, cell, margin);
                    } else if (name == "Fish") {
                        DrawInCell(gTex.foodFish, r, c, cell, margin);
                    } else { // Tomato or generic
                        DrawInCell(gTex.foodTomato, r, c, cell, margin);
                    }
                }
            }

            // guard
            if (dynamic_cast<GuardTile*>(t)) {
                DrawInCell(gTex.guard, r, c, cell, margin);
            }
        }
    }

    // draw player last (on top)
    Player& p = gm.getPlayer();
    DrawInCell(gTex.player, p.getRow(), p.getCol(), cell, margin);
}

int main() {
    // Initialize window parameters
    const int cellSize = 40;
    const int margin = 12;
    // Temporary dimensions (will adjust after difficulty selection)
    const int tempCols = 10; // Default to easy mode size
    const int tempRows = 10;
    const int screenW = tempCols * cellSize + margin * 2;
    const int screenH = tempRows * cellSize + margin * 2 + 80;

    InitWindow(screenW, screenH, "Maze (raylib)");
    SetTargetFPS(60);
    LoadAllTextures();

    // Game state variables
    bool hardMode = false;
    GameState state = GameState::MAIN_MENU;
    bool shouldExit = false;
    GameManager* gm = nullptr; // Initialize after difficulty selection
    Menu menu(screenW, screenH);

    while (!WindowShouldClose() && !shouldExit) {
        // Handle input based on state
        if (state == GameState::MAIN_MENU) {
            menu.HandleMainMenuInput(hardMode, state, shouldExit);
            if (state == GameState::PLAYING) {
                // Initialize game after difficulty selection
                if (gm != nullptr) {
                    delete gm;
                }
                gm = new GameManager(hardMode);
                gm->initLevel();
                // Resize window based on difficulty
                int cols = gm->getCols();
                int rows = gm->getRows();
                int newScreenW = cols * cellSize + margin * 2;
                int newScreenH = rows * cellSize + margin * 2 + 80;
                if (newScreenW != screenW || newScreenH != screenH) {
                    SetWindowSize(newScreenW, newScreenH);
                }
            }
        } else if (state == GameState::PLAYING) {
            menu.HandlePauseButtonClick(state);
            if (!gm->isGameOver() && !gm->isWin()) {
                if (IsKeyPressed(KEY_W)) gm->handleInput('w');
                if (IsKeyPressed(KEY_S)) gm->handleInput('s');
                if (IsKeyPressed(KEY_A)) gm->handleInput('a');
                if (IsKeyPressed(KEY_D)) gm->handleInput('d');
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_I)) gm->handleInput('i');
            }
        } else if (state == GameState::PAUSED) {
            menu.HandlePauseMenuInput(state, shouldExit);
        }

        // Update game timer only when playing
        if (state == GameState::PLAYING && gm != nullptr) {
            float dt = GetFrameTime();
            if (dt > 0.f && !gm->isGameOver() && !gm->isWin()) {
                gm->update(dt);
            }
            if (gm->isGameOver() || gm->isWin()) {
                state = GameState::GAME_OVER;
            }
        }

        bool drewBlind = false;

        // Draw
        BeginDrawing();
        ClearBackground(Color{18,18,24,255});

        if (state == GameState::MAIN_MENU) {
            menu.DrawMainMenu();
        } else if (state == GameState::PLAYING && gm != nullptr) {
            DrawGame(*gm, cellSize, margin);
            menu.DrawPauseButton();
            // HUD
            int hudY = margin + gm->getRows() * cellSize + 12;
            DrawText(TextFormat("Strength: %d", gm->getPlayer().getStrength()), margin, hudY, 22, RAYWHITE);
            DrawText(TextFormat("Time: %d", (int)gm->getTimeRemaining()), margin + 240, hudY, 22, RAYWHITE);
            DrawText("(WASD move, SPACE/I interact, ESC quit)", margin, hudY + 26, 16, Color{160,160,175,255});

            // Blind overlay
            if (gm->getBlindTime() > 0.0) {
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
                const char* msg = "BLINDED!";
                int fs = 40;
                int w = MeasureText(msg, fs);
                DrawText(msg, GetScreenWidth() / 2 - w / 2, GetScreenHeight() / 2 - 20, fs, RAYWHITE);
                drewBlind = true;
            }
        } else if (state == GameState::PAUSED) {
            DrawGame(*gm, cellSize, margin);
            menu.DrawPauseButton();
            menu.DrawPauseMenu();
        } else if (state == GameState::GAME_OVER && gm != nullptr) {
            DrawGame(*gm, cellSize, margin);
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
            const char* line = gm->isWin() ? "YOU WIN!" : "YOU LOSE!";
            int size = 48;
            int w = MeasureText(line, size);
            DrawText(line, GetScreenWidth() / 2 - w / 2, GetScreenHeight() / 2 - 32, size, RAYWHITE);
            const char* hint = "Press any key to exit";
            int w2 = MeasureText(hint, 20);
            DrawText(hint, GetScreenWidth() / 2 - w2 / 2, GetScreenHeight() / 2 + 22, 20, RAYWHITE);
        }

        EndDrawing();

        // error handling for blind
        try {
            if (state == GameState::PLAYING && gm != nullptr &&
                gm->getBlindTime() > 0.0f && !drewBlind) {
                throw std::runtime_error("GUI blind overlay not drawn while blinded.");
            }
        } catch (const std::runtime_error& e) {
            std::cout << "GUI error: " << e.what() << std::endl;
        }

        // Allow any key to exit after game over
        if (state == GameState::GAME_OVER && GetKeyPressed() != 0) {
            shouldExit = true;
        }
    }

    // Cleanup
    if (gm != nullptr) {
        delete gm;
    }
    UnloadAllTextures();
    CloseWindow();
    return 0;
}