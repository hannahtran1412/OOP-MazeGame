#include "Food.h"
#include "Player.h"
#include <iostream>
using namespace std;

Food::Food(string n, int v) : name(n), nutritionValue(v) {}

Food::~Food() {}

int Food::getValue() { 
  return nutritionValue;
}

string Food::getName() { 
  return name; 
}

void Food::use(Player &p) {
  p.addStrength(nutritionValue);
  cout << "Collected " << name << "! Strength increased by " << nutritionValue << endl;
}

/*
Tomato::Tomato() : Food("Tomato", 2) {}
Banana::Banana() : Food("Banana", 1) {}
Fish::Fish() : Food("Fish", 3) {}
*/
