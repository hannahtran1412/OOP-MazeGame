#ifndef FOOD_H
#define FOOD_H

#include <string>
using namespace std;

class Player;

class Food {
    private:
        string name;
        int nutritionValue;

    public:
        // constructor
        Food(string n, int v);

        // destructor
        virtual ~Food();
        
        // getters
        virtual int getValue();
        virtual string getName();

        // add food's nutritionVale to player's strength
        virtual void use(Player &p);
};

/*
derived classes for specific food types?
class Tomato : public Food {
    public:
        Tomato();
    };

class Banana : public Food {
    public:
        Banana();
    };

class Fish : public Food {
    public:
        Fish();
    };
*/


#endif