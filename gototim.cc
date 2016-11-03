#include "gototim.h"
#include "dctim.h"
#include "player.h"
#include "map.h"
#include <string>
#include <iostream>
using namespace std;

GotoTim::GotoTim(){
    name = "GOTOTIMS";
    prev = nullptr;
    next = nullptr;
    number = 30;
    coordX = 10;
    coordY = 0;
}
void GotoTim::notify(Player* p)
{
    int origCoordX = p->coordX;
    int origCoordY = p->coordY;
	 p->turnInDC = 0;
	 p->locationIndex = 10;
     p->position = p->getMap()->getBuilding(10);
     p->coordX =  p->position->getX();
	 p->coordY =  p->position->getY();
    p->getMap()->changePlayer(p->particle,origCoordX,origCoordY,p->coordX,p->coordY);
	cout << "Player " <<  p->name;
	cout << " just got sent to DC!" << endl;
	p->getMap()->getBuilding(10)->notify(p);
}


