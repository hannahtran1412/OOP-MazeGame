#ifndef MENU_H
#define MENU_H

#include "raylib.h"

enum class GameState { MAIN_MENU, PLAYING, PAUSED, GAME_OVER };

class Menu {
private:
    Rectangle easyButton;
    Rectangle hardButton;
    Rectangle quitButton;

    Rectangle pauseButton;
    Rectangle continueButton;
    Rectangle quitPauseButton;

public:
    Menu(int screenW, int screenH);

    void DrawMainMenu();

    void HandleMainMenuInput(bool& hardMode, GameState& state, bool& shouldExit);

    void DrawPauseButton();

    void HandlePauseButtonClick(GameState& state);

    void DrawPauseMenu();

    void HandlePauseMenuInput(GameState& state, bool& shouldExit);
};

#endif