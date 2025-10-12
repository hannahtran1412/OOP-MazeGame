#include "Player.h"
#include <iostream>

Player::Player(std::string name, int startR, int startC, int startStr) {
    this->name = name;
    this->row = startR; //Or directly to 1?
    this->col = startC;
    this->strength = startStr;
}

int Player::getRow() const {
    return row;
}

int Player::getCol() const {
    return col;
}

int Player::getStrength() const {
    return strength;
}

void Player::setPos(int r, int c) {
    this->row = r;
    this->col = c;
}

void Player::addStrength(int amount) {
    this->strength += amount;
}