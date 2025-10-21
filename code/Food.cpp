#include "Food.h"

#include <iostream>

#include "Player.h"
using namespace std;

// constructor
Food::Food(string n, int v) : name(n), nutritionValue(v) {}

// destructor
Food::~Food() {}

int Food::getValue() { return nutritionValue; }

string Food::getName() { return name; }

void Food::use(Player& p) {
  p.addStrength(nutritionValue);
  // cout << "Collected " << name << "! Strength increased by " <<
  // nutritionValue << endl;
}
