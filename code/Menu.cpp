#include "Menu.h"

Menu::Menu(int screenW, int screenH) {
    int buttonW = 200;
    int buttonH = 50;
    int centerX = screenW / 2 - buttonW / 2;

    easyButton = { (float)centerX, 150, (float)buttonW, (float)buttonH };
    hardButton = { (float)centerX, 220, (float)buttonW, (float)buttonH };
    quitButton = { (float)centerX, 290, (float)buttonW, (float)buttonH };

    pauseButton = { (float)screenW - 80, 10, 70, 30 };

    // Center buttons for pause menu
    continueButton = { (float)centerX, (float)screenH / 2.0f - (float)buttonH - 30.0f, (float)buttonW, (float)buttonH };
    quitPauseButton = { (float)centerX, (float)screenH / 2.0f + 30.0f, (float)buttonW, (float)buttonH };
}

void Menu::DrawMainMenu() {
    DrawText("Maze Game", GetScreenWidth() / 2 - MeasureText("Maze Game", 40) / 2, 50, 40, RAYWHITE);

    DrawRectangleRec(easyButton, BLUE);
    DrawText("Easy", (int)easyButton.x + 70, (int)easyButton.y + 10, 30, WHITE);

    DrawRectangleRec(hardButton, BLUE);
    DrawText("Hard", (int)hardButton.x + 70, (int)hardButton.y + 10, 30, WHITE);

    DrawRectangleRec(quitButton, BLUE);
    DrawText("Quit", (int)quitButton.x + 70, (int)quitButton.y + 10, 30, WHITE);
}

void Menu::HandleMainMenuInput(bool& hardMode, GameState& state, bool& shouldExit) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, easyButton)) {
            hardMode = false;
            state = GameState::PLAYING;
        } else if (CheckCollisionPointRec(mouse, hardButton)) {
            hardMode = true;
            state = GameState::PLAYING;
        } else if (CheckCollisionPointRec(mouse, quitButton)) {
            shouldExit = true;
        }
    }
}

void Menu::DrawPauseButton() {
    DrawRectangleRec(pauseButton, Color{80, 80, 80, 255});
    DrawText("Pause", (int)pauseButton.x + 5, (int)pauseButton.y + 5, 20, BLACK);
}

void Menu::HandlePauseButtonClick(GameState& state) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, pauseButton)) {
            state = GameState::PAUSED;
        }
    }
}

void Menu::DrawPauseMenu() {
    // Draw pause menu background in top-left corner
    DrawRectangle(0, 0, 220, 100, Color{0, 0, 128, 100});
    DrawText("Paused", 10, 10, 40, RAYWHITE);

    // Draw centered buttons
    DrawRectangleRec(continueButton, Color{0, 128, 128, 255}); 
    DrawText("Continue", (int)continueButton.x + 30, (int)continueButton.y + 10, 30, WHITE);

    DrawRectangleRec(quitPauseButton, Color{0, 128, 128, 255}); 
    DrawText("Quit", (int)quitPauseButton.x + 70, (int)quitPauseButton.y + 10, 30, WHITE);
}

void Menu::HandlePauseMenuInput(GameState& state, bool& shouldExit) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, continueButton)) {
            state = GameState::PLAYING;
        } else if (CheckCollisionPointRec(mouse, quitPauseButton)) {
            shouldExit = true;
        }
    }
}