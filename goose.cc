#include "goose.h"
#include <iostream>
using namespace std;

Goose::Goose(){
    name = "Goose";
    prev = nullptr;
    next = nullptr;
    number = 20;
    coordX = 0;
    coordY = 0;
}
void Goose::notify(Player* p){
    cout << "DANGER! YOU ARE ATTACKED BY GEESE." << endl;
}


