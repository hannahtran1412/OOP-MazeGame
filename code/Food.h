// Represents food item with a name and nutrition value
#ifndef FOOD_H
#define FOOD_H

#include <string>
using namespace std;

class Player;

class Food {
 private:
  string name;
  int nutritionValue;  // strength that player gained from food

 public:
  // constructor
  Food(string n, int v);

  // destructor
  virtual ~Food();

  // getters
  virtual int getValue();
  virtual string getName();

  // add food's nutritionVale to player's strength
  virtual void use(Player& p);
};
#endif