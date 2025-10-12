#include "GameManager.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

int main() {
    // Ask difficulty
    char mode;
    cout << "Choose mode: (E)asy or (H)ard: ";
    cin >> mode;

    bool hardMode = (mode == 'H' || mode == 'h');

    GameManager gm(hardMode);
    gm.initLevel();

    // game loop
    while (!gm.isGameOver() && !gm.isWin()) {
        gm.render();

        // Get input
        char key;
        cout << "Input: ";
        cin >> key;

        gm.handleInput(key);

        // simulate 1 second passing per input (just for demo)
        gm.update(1.0);

        // delay for readability
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    gm.render();
    cout << "Game Ended." << endl;
    return 0;
}