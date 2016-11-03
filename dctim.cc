#include "dctim.h"
#include "player.h"
#include "map.h"
#include <string>
#include <iostream>
using namespace std;
const int turnInDc = 3;
const int lastIndex = 39;

DcTim::DcTim(){
    name = "DCTims";
    prev = nullptr;
    next = nullptr;
    coordX = 0;
    coordY = 10;
    number = 10;
}

void DcTim::leaveDC(Player *p,int point){
    int origX = p->coordX;
    int origY = p->coordY;
    if(p->locationIndex + point > lastIndex){
        p->locationIndex = p->locationIndex + point-1;
    }else{
        p->locationIndex += point;
    }
	
	for(int i = 0; i < point; i++){
		p->position = p->position->getNext();
	}
	p->coordX = p->position->getX();
	p->coordY = p->position->getY();
	p->turnInDC = turnInDc;
    p->getMap()->changePlayer(p->particle,origX,origY,p->coordX,p->coordY);
    cout<<"Left DC Tims Line!"<<endl;
    p->position->notify(p);
}

void DcTim::notify(Player* p, int roll1,int roll2){
    
    if(p->turnInDC == 3)
    {
        cout << "Player passes DC Tims line but doesn't want a cup of coffee." << endl;
    }
    else
    {
        cout << p->name << " stayed in DC Tims for " << p->turnInDC + 1;
        cout << " rounds already." << endl;
			     
        int sum;
        srand((int)time(0));
        sum = roll1 + roll2;
        
        if(roll1 == roll2 && p->turnInDC)
        {
            leaveDC(p,sum);
        }
        else if(p->turnInDC == 2)
        {
            if(p->getCup() != 0 && p->getCash() >= 50)
            {
                cout << "Input 1 to pay by cash or 2 to pay by Rim cup!" << endl;
                int choice;
                cin >> choice;
                
                while(cin.fail() || (choice != 1 && choice != 2))
                {
                    cout << "Invalid input! Only input 1 or 2 please" << endl;
                }
                
                if(choice == 1)
                {
                    p->pay(50);
                    leaveDC(p,sum);
                }
                else
                {
                    p->lostCup();
                    leaveDC(p,sum);
                }
            }
            else if(p->getCup() == 0 && p->getCash() >= 50)
            {
                cout << "You lost $50 for leaving DC Tims!" << endl;
                p->pay(50);
                leaveDC(p,sum);
            }
            else if(p->getCup() > 0 && p->getCash() < 50)
            {
                cout << "You lost a Rim cup for leaving DC Tims!" << endl;
                p->lostCup();
                leaveDC(p,sum);
            }
            // no cup no money
            else
            {
                p->pay(50);
            }
        }
        else
        {
            if(p->getCup() != 0 && p->getCash() >= 50)
            {
                cout << "Input 1 to pay by cash or 2 to pay by Rim cup to leave early!" << endl;
                cout << "Or input anything else to choose to stay!" << endl;
                
                int choice;
                cin >> choice;
                
                if(cin.fail() || (choice != 1 && choice != 2))
                {
                    p->turnInDC++;
                }
                else if(choice == 1)
                {
                    p->pay(50);
                    leaveDC(p,sum);
                }
                else
                {
                    p->lostCup();
                    leaveDC(p,sum);
                }
            }
            else if(p->getCup() == 0 && p->getCash() >= 50)
            {
                cout << "Do you want to pay $50 for leaving DC Tims?" << endl;
                cout << "Input Y to accept or refuse by inputting anything else!" << endl;
                string acceptPayment;
                cin >> acceptPayment;
                
                if(acceptPayment == "Y")
                {
                    p->pay(50);
                    leaveDC(p,sum);
                }
                else
                {
                    p->turnInDC++;
                }
            }
            else if(p->getCup() != 0 && p->getCash() < 50)
            {
                cout << "Do you want to pay a Rim cup for leaving DC Tims?" << endl;
                cout << "Input Y to accept or refuse by inputting anything else!" << endl;
                string acceptPayment;
                cin >> acceptPayment;
                
                if(acceptPayment == "Y")
                {
                    p->lostCup();
                    leaveDC(p,sum);
                }
                else
                {
                    p->turnInDC++;
                }
            }
            // no cup no money
            else
            {
                p->turnInDC++;
            }
        }
    }
}

void DcTim::notify(Player* p){
    if(p->turnInDC == 3)
    {
        cout << "Player passes DC Tims line but doesn't want a cup of coffee." << endl;
    }
    else
    {
        cout << p->name << " stayed in DC Tims for " << p->turnInDC + 1;
        cout << " rounds already." << endl;
			     
        int sum;
        srand((int)time(0));
        int roll1 = 1+rand()%6;
        int roll2 = 1+rand()%6;
        sum = roll1 + roll2;
        
        if(roll1 == roll2)
        {
            leaveDC(p,sum);
        }
        else if(p->turnInDC == 2)
        {
            if(p->getCup() != 0 && p->getCash() >= 50)
            {
                cout << "Input 1 to pay by cash or 2 to pay by Rim cup!" << endl;
                int choice;
                cin >> choice;
                
                while(cin.fail() || (choice != 1 && choice != 2))
                {
                    cout << "Invalid input! Only input 1 or 2 please" << endl;
                }
                
                if(choice == 1)
                {
                    p->pay(50);
                    leaveDC(p,sum);
                }
                else
                {
                    p->lostCup();
                    leaveDC(p,sum);
                }
            }
            else if(p->getCup() == 0 && p->getCash() >= 50)
            {
                cout << "You lost $50 for leaving DC Tims!" << endl;
                p->pay(50);
                leaveDC(p,sum);
            }
            else if(p->getCup() > 0 && p->getCash() < 50)
            {
                cout << "You lost a Rim cup for leaving DC Tims!" << endl;
                p->lostCup();
                leaveDC(p,sum);
            }
            // no cup no money
            else
            {
                p->pay(50);
            }
        }
        else
        {
            if(p->getCup() != 0 && p->getCash() >= 50)
            {
                cout << "Input 1 to pay by cash or 2 to pay by Rim cup to leave early!" << endl;
                cout << "Or input anything else to choose to stay!" << endl;
                
                int choice;
                cin >> choice;
                
                if(cin.fail() || (choice != 1 && choice != 2))
                {
                    p->turnInDC++;
                }
                else if(choice == 1)
                {
                    p->pay(50);
                    leaveDC(p,sum);
                }
                else
                {
                    p->lostCup();
                    leaveDC(p,sum);
                }
            }
            else if(p->getCup() == 0 && p->getCash() >= 50)
            {
                cout << "Do you want to pay $50 for leaving DC Tims?" << endl;
                cout << "Input Y to accept or refuse by inputting anything else!" << endl;
                string acceptPayment;
                cin >> acceptPayment;
                
                if(acceptPayment == "Y")
                {
                    p->pay(50);
                    leaveDC(p,sum);
                }
                else
                {
                    p->turnInDC++;
                }
            }
            else if(p->getCup() != 0 && p->getCash() < 50)
            {
                cout << "Do you want to pay a Rim cup for leaving DC Tims?" << endl;
                cout << "Input Y to accept or refuse by inputting anything else!" << endl;
                string acceptPayment;
                cin >> acceptPayment;
                
                if(acceptPayment == "Y")
                {
                    p->lostCup();
                    leaveDC(p,sum);
                }
                else
                {
                    p->turnInDC++;
                }
            }
            // no cup no money
            else
            {
                p->turnInDC++;
            }
        }
    }
}


