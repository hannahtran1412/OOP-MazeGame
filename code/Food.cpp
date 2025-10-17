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
  //cout << "Collected " << name << "! Strength increased by " << nutritionValue << endl;
}


