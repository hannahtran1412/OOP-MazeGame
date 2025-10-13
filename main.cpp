#include "GameManager.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic> // to avoid corruption when 2 bools run at the same time
#include <limits> // to valid input
using namespace std;

int main() {
    // Ask difficulty
    char mode;

    // check if input is valid
    bool validInput = false;
    while (!validInput) {
        cout << "Choose mode: (E)asy or (H)ard: ";
        cin >> mode;
        if (mode == 'E' || mode == 'e' || mode == 'H' || mode == 'h') {
            validInput = true;
        } else {
            cout << "Invalid input. Please press 'E' for Easy or 'H' for Hard." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    bool hardMode = (mode == 'H' || mode == 'h');

    GameManager gm(hardMode);
    gm.initLevel();
    gm.render();

    // timer thread control
    atomic<bool> running(true);
    //ticks one per second
    thread timerThread([&]() {
      while (running && !gm.isGameOver() && !gm.isWin()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        gm.update(1.0); // -1s time remaining
        gm.render();    // refresh display
      }
    });


    // game loop
    while (!gm.isGameOver() && !gm.isWin()) {
        gm.render();

        // Get input
        char key;
        cout << "Input: ";
        cin >> key;

        gm.handleInput(key);

        // delay for readability
        this_thread::sleep_for(chrono::milliseconds(200));
    }

    gm.render();
    cout << "Game Ended." << endl;
    return 0;

    running = false;            // 1. Signal thread to stop looping
    if (timerThread.joinable()) // 2. Check if thread is still active
    timerThread.join();     // 3. Safely wait for it to finish

    return 0;
}