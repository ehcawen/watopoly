//  map.cpp
//  watopoly
//
//  Created by Zhihan Wen on 2016-07-14.
//  Copyright © 2016 Zhihan Wen. All rights reserved.
//

//
//  map.cpp
//  watopoly
//
//  Created by Zhihan Wen on 2016-07-14.
//  Copyright © 2016 Zhihan Wen. All rights reserved.
//


#include "map.h"
#include "academic.h"
#include "gym.h"
#include "residence.h"
#include "osap.h"
#include "goose.h"
#include "coop.h"
#include "slc.h"
#include "dctim.h"
#include "needles.h"
#include "tuition.h"
#include "gototim.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <algorithm>
#include <typeinfo>
#include <sstream>
using namespace std;

Map::Map(int totalPlayer) // constructor 1, for when new game is made
{
    numOfPlayer = totalPlayer;
    indexOfPlayer = 0;
    numOfCupLeft = 4;
    
    indexOfPlayer = 0;
    
    //read board.txt into cells
    cells = new char*[mapHeight];
    for (int i = 0; i < mapHeight; ++i) {
        cells[i] = new char[mapWidth];
    }
    ifstream readFile;
    readFile.open("board.txt");
    char ch;
    for(int j = 0; j < mapHeight; ++j) {
        for (int i = 0; i < mapWidth; ++i) {
            readFile>>noskipws>>ch;
            cells[j][i] = ch;
        }
    }
    readFile.close();
    
    // Player constructors
    bool used[8];
    string nameArray[8];
    char charArray[8] = {'G','B','D','P','S','$','L','T'};
    for(int i = 0; i < 8; i++)
    {
        used[i] = false;
    }
    string s;
    for(int i = 0; i < numOfPlayer; i++)
    {
        cout << "Please input the name for player " << i+1 << ": " << endl;
        cin >> s;
        
        if(cin.fail())
        {
            cin.clear();
            cin.ignore();
            cout << "Invalid input, try again!" << endl;
            i--;
        }else{
            bool ex = false;
            for(int j = 0; j < i; j++){
                if(nameArray[j]==s)
                    ex = true;
                break;
            }
            if(!ex && (s != "BANK")){nameArray[i] = s;}
            else{
                cout << "Invalid name, try again!" << endl;
                i--;
            }
        }
        
    }
    
    for(int i = 0; i < totalPlayer; i++)
    {
        cout << "Player " << i + 1 <<" "<<nameArray[i]<< "," << endl;
        cout << "What particle do you prefer?`" << endl;
        cout << "Available: ";
        for(int j = 0; j < 8;j ++)
        {
            if(!(used[j]))
            {
    	           cout << charArray[j] << " ";
            }
        }
        cout << endl;
        bool failInput = true;
        char c;
        cin >> c;
        string pname;
        char pparticle = '\0';
        for(int k = 0; k < 8; k++)
        {
            if(c == charArray[k] && !(used[k]))
            {
                pname = nameArray[i];
                pparticle = charArray[k];
                used[k] = true;
                failInput = false;
            }
        }
        
        if(failInput)
        {
            cout << "Invalid input or player name already exist!" << endl;
            cout << "Try again!" << endl;
            i--;
        }
        else
        {
            Player *p = new Player(pname, pparticle, this);
            playerInGame.emplace_back(p);
        }
    }
    currentPlayer = playerInGame.at(0);
    //Buidling constructors
    unit[0] = new Osap();
    unit[1] = new Academic("AL","Arts1",40,1,9,10,50,2,10,30,90,160,250);
    unit[2] = new Slc(8,10,2);
    unit[3] = new Academic("ML","Arts1",60,3,7,10,50,4,20,60,180,320,450);
    unit[4] = new Tuition();
    unit[5] = new Residence("MKV",5,200,5,10);
    unit[6] = new Academic("ECH","Arts2",100,6,4,10,50,6,30,90,270,400,550);
    unit[7] = new Needles(3,10,7);
    unit[8] = new Academic("PAS","Arts2",100,8,2,10,50,6,30,90,270,400,550);
    unit[9] = new Academic("HH","Arts2",120,9,1,10,50,8,40,100,300,450,600);
    unit[10] = new DcTim();
    unit[11] = new Academic("RCH","Eng",140,11,0,9,100,10,50,150,450,625,750);
    unit[12] = new Gym("PAC",12,150,0,8);
    unit[13] = new Academic("DWE","Eng",140,13,0,7,100,10,50,150,450,625,750);
    unit[14] = new Academic("CPH","Eng",160,14,0,6,100,12,60,180,500,700,900);
    unit[15] = new Residence("UWP",15,200,0,5);
    unit[16] = new Academic("LHI","Health",180,16,0,4,100,14,70,200,550,750,950);
    unit[17] = new Slc(0,3,17);
    unit[18] = new Academic("BMH","Health",180,18,0,2,100,14,70,200,550,750,950);
    unit[19] = new Academic("OPT","Health",200,19,0,1,100,16,80,220,600,800,1000);
    unit[20] = new Goose();
    unit[21] = new Academic("EV1","Env",220,21,1,0,150,18,90,250,700,875,1050);
    unit[22] = new Needles(2,0,22);
    unit[23] = new Academic("EV2","Env",220,23,3,0,150,18,90,250,700,875,1050);
    unit[24] = new Academic("EV3","Env",240,24,4,0,150,20,100,300,750,925,1100);
    unit[25] = new Residence("V1",25,200,5,0);
    unit[26] = new Academic("PHYS","Sci1",260,26,6,0,150,22,110,330,800,975,1150);
    unit[27] = new Academic("B1","Sci1",260,27,7,0,150,22,110,330,800,975,1150);
    unit[28] = new Gym("CIF",28,150,8,0);
    unit[29] = new Academic("B2","Sci1",280,29,9,0,150,24,120,360,850,1025,1200);
    unit[30] = new GotoTim();
    unit[31] = new Academic("EIT","Sci2",300,31,10,1,200,26,130,390,900,1100,1275);
    unit[32] = new Academic("ESC","Sci2",300,32,10,2,200,26,130,390,900,1100,1275);
    unit[33] = new Slc(10,3,33);
    unit[34] = new Academic("C2","Sci2",320,34,10,4,200,28,150,450,100,1200,1400);
    unit[35] = new Residence("REV",35,200,10,5);;
    unit[36] = new Needles(10,6,36);////!!!
    unit[37] = new Academic("MC","Math",350,37,10,7,200,35,175,500,110,1300,1500);//
    unit[38] = new Coop();
    unit[39] = new Academic("DC","Math",400,39,10,9,200,50,200,600,1400,1700,2000);
    //connect them into a chain
    unit[0]->setPrev(unit[numOfBuilding-1]);
    for (int i = 1; i < numOfBuilding-1; ++i) {
        unit[i]->setPrev(unit[i-1]);
        unit[i]->setNext(unit[i+1]);
    }
    unit[numOfBuilding-1]->setNext(unit[0]);
    
    
    for (int i = 0; i < (int)playerInGame.size(); ++i) {
        placePlayer(playerInGame.at(i)->particle, 10,10);
    }
}

Map::Map(string saveFileName) // constructor 2 for load game
{
    //read board.txt into cells
    cells = new char*[mapHeight];
    for (int i = 0; i < mapHeight; ++i) {
        cells[i] = new char[mapWidth];
    }
    ifstream read;
    read.open("board.txt");
    char ch;
    for(int j = 0; j < mapHeight; ++j) {
        for (int i = 0; i < mapWidth; ++i) {
            read>>noskipws>>ch;
            cells[j][i] = ch;
        }
    }
    read.close();
    
    //read loading data
    string FileName = saveFileName + ".txt";
    ifstream readIn;
    readIn.open(FileName);
    readIn >>skipws>> numOfPlayer;
    // allocated player (but do not put them on the map)
    for (int i = 0; i < numOfPlayer; ++i) {
        string pname;
        char pparticle;
        int cups;
        int cash;
        int pos;
        int sentToDC;
        int timesInDC = 0;
        readIn >>skipws>> pname >> pparticle >> cups >> cash >> pos;
        if (cups > 0) {
            numOfCupLeft -= cups;
        }
        Player *p = new Player (pname,pparticle,cups,cash,pos,this);
        if (pos==10){
            readIn >>skipws>> sentToDC;
            if (sentToDC == 0) {
                p->turnInDC = 3;
            }else{
                readIn >>skipws>> timesInDC;
                p->turnInDC = timesInDC;
            }
        }
        playerInGame.emplace_back(p);
    }
    
    //initial buildings
    unit[0] = new Osap();
    unit[1] = new Academic("AL","Arts1",40,1,9,10,50,2,10,30,90,160,250);
    unit[2] = new Slc(8,10,2);
    unit[3] = new Academic("ML","Arts1",60,3,7,10,50,4,20,60,180,320,450);
    unit[4] = new Tuition();
    unit[5] = new Residence("MKV",5,200,5,10);
    unit[6] = new Academic("ECH","Arts2",100,6,4,10,50,6,30,90,270,400,550);
    unit[7] = new Needles(3,10,7);
    unit[8] = new Academic("PAS","Arts2",100,8,2,10,50,6,30,90,270,400,550);
    unit[9] = new Academic("HH","Arts2",120,9,1,10,50,8,40,100,300,450,600);
    unit[10] = new DcTim();
    unit[11] = new Academic("RCH","Eng",140,11,0,9,100,10,50,150,450,625,750);
    unit[12] = new Gym("PAC",12,150,0,8);
    unit[13] = new Academic("DWE","Eng",140,13,0,7,100,10,50,150,450,625,750);
    unit[14] = new Academic("CPH","Eng",160,14,0,6,100,12,60,180,500,700,900);
    unit[15] = new Residence("UWP",15,200,0,5);
    unit[16] = new Academic("LHI","Health",180,16,0,4,100,14,70,200,550,750,950);
    unit[17] = new Slc(0,3,17);
    unit[18] = new Academic("BMH","Health",180,18,0,2,100,14,70,200,550,750,950);
    unit[19] = new Academic("OPT","Health",200,19,0,1,100,16,80,220,600,800,1000);
    unit[20] = new Goose();
    unit[21] = new Academic("EV1","Env",220,21,1,0,150,18,90,250,700,875,1050);
    unit[22] = new Needles(2,0,22);
    unit[23] = new Academic("EV2","Env",220,23,3,0,150,18,90,250,700,875,1050);
    unit[24] = new Academic("EV3","Env",240,24,4,0,150,20,100,300,750,925,1100);
    unit[25] = new Residence("V1",25,200,5,0);
    unit[26] = new Academic("PHYS","Sci1",260,26,6,0,150,22,110,330,800,975,1150);
    unit[27] = new Academic("B1","Sci1",260,27,7,0,150,22,110,330,800,975,1150);
    unit[28] = new Gym("CIF",28,150,8,0);
    unit[29] = new Academic("B2","Sci1",280,29,9,0,150,24,120,360,850,1025,1200);
    unit[30] = new GotoTim();
    unit[31] = new Academic("EIT","Sci2",300,31,10,1,200,26,130,390,900,1100,1275);
    unit[32] = new Academic("ESC","Sci2",300,32,10,2,200,26,130,390,900,1100,1275);
    unit[33] = new Slc(10,3,33);
    unit[34] = new Academic("C2","Sci2",320,34,10,4,200,28,150,450,100,1200,1400);
    unit[35] = new Residence("REV",35,200,10,5);;
    unit[36] = new Needles(10,6,36); ////!!!!!!!!!
    unit[37] = new Academic("MC","Math",350,37,10,7,200,35,175,500,110,1300,1500);/////!!!!
    unit[38] = new Coop();
    unit[39] = new Academic("DC","Math",400,39,10,9,200,50,200,600,1400,1700,2000);
    //connect them into a chain
    unit[0]->setPrev(unit[numOfBuilding-1]);
    for (int i = 1; i < numOfBuilding-1; ++i) {
        unit[i]->setPrev(unit[i-1]);
        unit[i]->setNext(unit[i+1]);
    }
    unit[numOfBuilding-1]->setNext(unit[0]);
    
    //set fields of propertysqrs
    for (int i = 0; i < numOfBuilding; ++i) {
        if (propertySqr *pqs = dynamic_cast<propertySqr*>(unit[i])) {
            string bname;
            string ownerName;
            int numOfImp;
            readIn >>skipws>> bname >> ownerName >> numOfImp;
            if (ownerName != "BANK" && ownerName != "") {
                //if owned
                for (int j = 0; j < numOfPlayer; ++j) {
                    //found owner
                    if (playerInGame.at(j)->name == ownerName) {
                        pqs->changeOwner(playerInGame.at(j));
                    }
                }
                if (numOfImp == -1) {
                    //if mortaged
                    pqs->changeMortagageStatus();
                }else if(numOfImp > 0){
                    //if is academic building and have improvements > 0
                    Academic *acadsqr = dynamic_cast<Academic*>(pqs);
                    for (int x = 0; x < numOfImp; ++x) {
                        acadsqr->addImp(acadsqr->getOwner());
                    }
                }
            }
        }
    }
    readIn.close();
    //finally add all fields to map but not sure which player is te current one
    currentPlayer = playerInGame.at(0);
    indexOfPlayer = 0;
    
    //change cells of map
    for (int i = 0; i < (int)playerInGame.size(); ++i) {
        playerInGame.at(i)->coordX = unit[playerInGame.at(i)->locationIndex]->getX();
        playerInGame.at(i)->coordY = unit[playerInGame.at(i)->locationIndex]->getY();
        placePlayer(playerInGame.at(i)->particle, playerInGame.at(i)->coordX, playerInGame.at(i)->coordY);
    }
}


void Map::placePlayer(char c, int posx, int posy){
    int x = posx * width;
    int y = posy * height + 4;
    bool Line1Full = true;
    int z;
    for (int i = 1; i <= 4; ++i) {
        if (cells[y][x+i] == ' ') {
            z = i;
            Line1Full = false;
            break;
        }
    }
    //if Line 1 is full of players
    if (Line1Full){
        y++;//change to next line
        for (int i = 1; i <= 4; ++i) {
            if (cells[y][x+i] == ' ') {
                z = i;
                break;
            }
        }
    }
    cells[y][x+z] = c;
    
}

void Map::changePlayer(char c, int originX, int originY, int newX, int newY){
    int x1 = originX * width;
    int y1 = originY * height + 4;
    int x2 = newX * width;
    int y2 = newY * height + 4;
    int q;
    //recover the position player was on
    bool InLine1 = false;
    for (int i = 1; i <= 4; ++i) {
        if (cells[y1][x1+i] == c) {
            q = i;
            InLine1 = true;
            break;
        }
    }
    //if not in Line 1 for players
    if (!InLine1){
        y1++;//change to next line
        for (int j = 1; j <= 4; ++j) {
            if (cells[y1][x1+j] == c) {
                q = j;
                break;
            }
        }
    }
    cells[y1][x1+q] = ' ';
    
    int p;
    //change the new player's position
    bool Line1Full = true;
    for (int i = 1; i <= 4; ++i) {
        if (cells[y2][x2+i] == ' ') {
            p = i;
            Line1Full = false;
            break;
        }
    }
    //
    
    //if Line 1 is full of players
    if (Line1Full){
        y2++;//change to next line
        for (int i = 1; i <= 4; ++i) {
            if (cells[y2][x2+i] == ' ') {
                p = i;
                break;
            }
        }
    }
    cells[y2][x2+p] = c;
}


Map::~Map()
{
    
    for(int i = 0; i < numOfPlayer; i++)
    {
        playerInGame[i]->~Player();
    }
    
    
    
    for(int i = 0; i < numOfBuilding; i++)
    {
        delete unit[i];
    }
    delete [] cells;
    //delete cells;
}

// print function operator overloading
ostream &operator<<(std::ostream &out, const Map &mp)
{
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            out << mp.cells[i][j];
        }
        //out <<noskipws<< endl;
    }
    out << endl;
    return  out;
}


void Map::rollDice()
{
    int origCoordX = currentPlayer->coordX;
    int origCoordY = currentPlayer->coordY;
    if(currentPlayer->turnInDC == 3)
    {
        bool sendPlayerDc = false;
        int numFromRoll; // give a random
        srand((int)time(0));
        int Row1 = 1+rand()%6;
        int Row2 = 1+rand()%6;
        if(Row1 == Row2)
        {
            cout << "rolled doubles" << endl;
            Row1 = 1+rand()%6;
            Row2 = 1+rand()%6;
            if(Row1 == Row2)
            {
                cout << "rolled doubles" << endl;
                Row1 = 1+rand()%6;
                Row2 = 1+rand()%6;
                if(Row1 == Row2)
                {
                    cout << "rolled doubles" << endl;
                    sendPlayerDc = true;
                    currentPlayer->turnInDC = 2;
                }
            }
        }
        
        numFromRoll = Row1 + Row2;
        cout<< "player rolled " << Row1 << " and " << Row2 << endl;
        
        if(!sendPlayerDc)
        {
            currentPlayer->locationIndex += numFromRoll;
            if(currentPlayer->locationIndex >= 40)
            {
                if (currentPlayer->locationIndex > 40) {
                    cout << "Passing OSAP and get $200!"<<endl;
                    currentPlayer->getPaid(200);
                }
                currentPlayer->locationIndex -= 40;
            }
            else if(currentPlayer->locationIndex < 0)
            {
                currentPlayer->locationIndex += 40;
            }
            currentPlayer->position = unit[currentPlayer->locationIndex];
            currentPlayer->coordX = currentPlayer->position->getX();
            currentPlayer->coordY = currentPlayer->position->getY();
            
            changePlayer(currentPlayer->particle,origCoordX,origCoordY,currentPlayer->coordX,
                         currentPlayer->coordY);
            cout << "Player moved to " << currentPlayer->locationIndex << endl;
            
            rollCup();
        }
        else
        {
            currentPlayer->turnInDC = 0;
            currentPlayer->locationIndex = 10;
            currentPlayer->position = unit[currentPlayer->locationIndex];
            currentPlayer->coordX = currentPlayer->position->getX();
            currentPlayer->coordY = currentPlayer->position->getY();
            changePlayer(currentPlayer->particle,origCoordX,origCoordY,currentPlayer->coordX,
                         currentPlayer->coordY);
            cout << "Player " << currentPlayer->name;
            cout << " just got sent to DC!" << endl;
        }
    }
    
    currentPlayer->notify(unit[currentPlayer->locationIndex]);
    
    numOfCupLeft = 0;
    for(int i = 0; i < numOfPlayer; i++)
    {
        numOfCupLeft += playerInGame[i]->getCup();
    }
}

void Map::rollCup()
{
    if(numOfCupLeft > 0 && (currentPlayer->locationIndex == 2 ||
                            currentPlayer->locationIndex == 17 ||
                            currentPlayer->locationIndex == 33 ||
                            currentPlayer->locationIndex == 7 ||
                            currentPlayer->locationIndex == 22 ||
                            currentPlayer->locationIndex == 36))
    {
        int randCup;
        srand((int)time(0));
        randCup = 1+rand()%100;
        if(randCup == 100)
        {
            currentPlayer->addCup();
            cout << "You got a Rim Up Cup! You got a free coffee!" << endl;
        }
    }
}

int readInt(int low, int high){
    int i;
    cin>>i;
    while(cin.fail()){
        cin.clear();
        cin.ignore();
        
        cout<<"Please enter valid input(i.e. integer)."<<endl;
        cin >>skipws>>i;
    }
    while (i<low||i>high) {
        cout<<"Out of range."<<endl;
        cin  >>skipws>> i;
        while(cin.fail()){
            cin.clear();
            cin.ignore();
            cout<<"Please enter valid input(i.e. integer)."<<endl;
            cin >>skipws>>i;
        }
    }
    return i;
}

void Map::rollDice(int r1, int r2)
{//test mode
    int origCoordX = currentPlayer->coordX;
    int origCoordY = currentPlayer->coordY;
    if(currentPlayer->turnInDC == 3)
    {
        bool sendPlayerDc = false;
        int numFromRoll; // give a random
        int Row1 = r1;
        int Row2 = r2;
        if(Row1 == Row2)
        {
            cout << "input another 2 rolls please" << endl;
            Row1 = readInt(1,6);
            Row2 = readInt(1,6);
            if(Row1 == Row2)
            {
                cout << "input another 2 rolls please" << endl;
                Row1 = readInt(1,6);
                Row2 = readInt(1,6);
                if(Row1 == Row2)
                {
                    cout << "rolled doubles" << endl;
                    sendPlayerDc = true;
                }
            }
        }
        
        numFromRoll = Row1 + Row2;
        
        if(!sendPlayerDc)
        {
            currentPlayer->locationIndex += numFromRoll;
            if(currentPlayer->locationIndex >= 40)
            {
                if (currentPlayer->locationIndex > 40) {
                    cout << "Passing OSAP and get $200!"<<endl;
                    currentPlayer->getPaid(200);
                }
                currentPlayer->locationIndex -= 40;
                
            }
            else if(currentPlayer->locationIndex < 0)
            {
                currentPlayer->locationIndex += 40;
            }
            currentPlayer->position = unit[currentPlayer->locationIndex];
            currentPlayer->coordX = currentPlayer->position->getX();
            currentPlayer->coordY = currentPlayer->position->getY();
            
            changePlayer(currentPlayer->particle,origCoordX,origCoordY,currentPlayer->coordX,
                         currentPlayer->coordY);
            rollCup();
        }
        else
        {
            currentPlayer->turnInDC = 0;
            currentPlayer->locationIndex = 10;
            currentPlayer->position = unit[currentPlayer->locationIndex];
            currentPlayer->coordX = currentPlayer->position->getX();
            currentPlayer->coordY = currentPlayer->position->getY();
            changePlayer(currentPlayer->particle,origCoordX,origCoordY,currentPlayer->coordX,
                         currentPlayer->coordY);
            cout << "Player " << currentPlayer->name;
            cout << " just got sent to DC!" << endl;
        }
    }
    
    if(currentPlayer->turnInDC == 3) {
        currentPlayer->notify(unit[currentPlayer->locationIndex]);
    }else{
        currentPlayer->notify(unit[currentPlayer->locationIndex], r1,r2);
    }
    
    numOfCupLeft = 0;
    for(int i = 0; i < numOfPlayer; i++)
    {
        numOfCupLeft += playerInGame[i]->getCup();
    }
}

void Map::saveHelper(string saveFileName){
    ofstream writeFile;
    writeFile.open(saveFileName);
    writeFile << numOfPlayer << endl; //number of players
    for (int i = 0; i < (int)playerInGame.size(); ++i) {    //save all players
        if(!playerInGame.at(i)->isBroken()){
            writeFile << playerInGame.at(i)->name;
            writeFile << " ";
            writeFile <<playerInGame.at(i)->particle;
            writeFile<<" "<<playerInGame.at(i)->getCup()<<" "<<playerInGame.at(i)->getCash()<<" "<<playerInGame.at(i)->locationIndex<<" ";
            if(playerInGame.at(i)->locationIndex == 10){
                if (playerInGame.at(i)->turnInDC < 3 && playerInGame.at(i)->turnInDC >= 0) {
                    writeFile << "1 " << playerInGame.at(i)->turnInDC << endl;
                }else{
                    writeFile << "0" << endl;
                }
            }else{
                writeFile << endl;
            }
        }
        
    }
    for (int i = 0; i < numOfBuilding; ++i) {       //save all buildings
        if(propertySqr* tmp = dynamic_cast<propertySqr*>(unit[i])){
            writeFile << unit[i]->name << " ";
            if (tmp->getOwner()!=nullptr) {
                //if owner not null
                writeFile << tmp->getOwner()->name << " ";
                //if it has improvements(academic)
                if(Academic *aca = dynamic_cast<Academic*>(unit[i])){
                    if (aca->mortagaged) {
                        writeFile << "-1 ";
                    }else{
                        writeFile << aca->getImp();
                    }
                }else{
                    writeFile << "0 ";
                }
                writeFile << endl;
            }else{
                //no owner
                writeFile << "BANK 0" << endl;
            }
        }
    }
    writeFile.close();
}
void Map::saveGame(string FileName){
    string saveFileName = FileName + ".txt";
    ifstream ifile(saveFileName);
    if(ifile)
    {
        cout << "File already existed, replace?(Y/N)"<<endl;
        string replaceRespond;
        cin >>skipws>> replaceRespond;
        if(replaceRespond == "Y")
        {   // Replace(Delete all content in the file first)
            remove(saveFileName.c_str());
            saveHelper(saveFileName);
            cout << "Game saved and " << saveFileName << " replaced!" << endl;
        }else{
            if(cin.eof())return;
        }
    }
    else
    {   //save game to a new file
        saveHelper(saveFileName);
        cout << "Game saved and new file " << saveFileName << " created!" << endl;
    }
}

void Map::nextPlayer()
{
    if(this->indexOfPlayer == numOfPlayer - 1){
        indexOfPlayer=0;
    }else{
        this->indexOfPlayer++;
    }
    currentPlayer = this->playerInGame[indexOfPlayer];
    cout<< "Now it's player" << indexOfPlayer + 1 << "'s turn!" << endl;
    
    if(this->playerInGame[indexOfPlayer]->isBroken())
    {
        cout << this->playerInGame[indexOfPlayer]->name << "is broken!" << endl;
        nextPlayer();
    }
}

void Map::auction(propertySqr *p)
{
    int currentPlayerRound = this->indexOfPlayer;
    
    int highestBid = 0;
    int bidder = -1;
    for(int i = 0; i < this->numOfPlayer; i++)
    {
        if(bidder == i)
        {
            // bid successfully
            p->changeOwner(playerInGame[i]);
            this->playerInGame[i]->pay(highestBid);
            
            cout << playerInGame[i]->name << " just won the bidding!" << endl;
            //Let player pay
            this->indexOfPlayer = currentPlayerRound;
            break;
        }
        
        this->indexOfPlayer = i;
        
        cout << "Current bidding is " << highestBid << "!" << endl;
        cout << "Player " << i + 1 << " is now bidding!" << endl;
        cout << "Raise the bid? (Y/N)" << endl;
        
        string raiseBid;
        cin >> raiseBid;
        
        
        while(raiseBid != "Y" && raiseBid != "N"){
            if(cin.eof())return;
            cout << "Invalid input, try again. (you may only input Y or N)" << endl;
            
            cin >> raiseBid;
        }
        
        
        if(raiseBid == "Y")
        {
            int currentBid;
            
            bool tryBidAgain = true;
            bool notGiveUp = true;
            while(tryBidAgain && (!(cin >> currentBid) || currentBid < highestBid
                                  || currentBid > playerInGame[i]->getCash()))
            {
                if(cin.fail())
                {
                    if(cin.eof())return;
                    cout << "Invalid input, please input a number!" << endl;
                    cout << "Input Y to try again or anything else to";
                    cout << "give up this round" << endl;
                    
                    string giveUpBid;
                    cin >> giveUpBid;
                    if(giveUpBid == "Y")
                    {
                        tryBidAgain = true;
                    }
                    else
                    {
                        cout << "Player " << this->indexOfPlayer + 1;
                        cout << " just gave up this round of bidding!" << endl;
                        tryBidAgain = false;
                        notGiveUp = false;
                    }
                }
                else if(currentBid > highestBid)
                {
                    cout << "You can't offer your current bidding!" << endl;
                    cout << "Input Y to try again or anything else to give up this round" << endl;
                    
                    string giveUpBid;
                    cin >> giveUpBid;
                    if(giveUpBid == "Y")
                    {
                        tryBidAgain = true;
                    }
                    else
                    {
                        cout << "Player " << this->indexOfPlayer + 1;
                        cout << " just gave up this round of bidding!" << endl;
                        tryBidAgain = false;
                        notGiveUp = false;
                    }
                }
                else
                {
                    cout << "Your bidding price is lower than current highest bidding!" << endl;
                    cout << "Input Y to try again or anything else to give up this round" << endl;
                    
                    string giveUpBid;
                    cin >> giveUpBid;
                    
                    if(giveUpBid == "Y")
                    {
                        tryBidAgain = true;
                    }
                    else
                    {
                        cout << "Player " << this->indexOfPlayer + 1 << " just gave up this round of bidding!" << endl;
                        tryBidAgain = false;
                        notGiveUp = false;
                    }
                }
            }
            
            if(notGiveUp)
            {
                highestBid = currentBid;
                bidder = i;
            }
        }
        else
        {
            cout << "Player " << this->indexOfPlayer + 1;
            cout << " just gave up this round of bidding!" << endl;
        }
        
        if(bidder == -1 && i == this->numOfPlayer - 1)
        {
            if(p->getOwner() != nullptr)
            {
                p->changeOwner(nullptr);
            }
            cout << "No player put a price";
            cout << " so the house is returned to the bank!" << endl;
            this->indexOfPlayer = currentPlayerRound;
            break;
        }
        
        if(i == this->numOfPlayer - 1)
        {
            i = -1;
        }
    }
}

void Map::allAsset(){
    for (int i = 0; i < (int)playerInGame.size(); ++i) {
        if (!playerInGame.at(i)->isBroken()) {
            playerInGame.at(i)->asset();
            cout << endl;
        }
    }
}


int Map::getNumPlayer(){
    int n = 0;
    for(auto x : playerInGame){
        if(!x->isBroken())++n;
    }
    return n;
}

string Map::oneLeft(){
    for(auto x : playerInGame){
        if(!x->isBroken())return x->name;
    }
    return "";
}

int numDigits(int number){//helper
    int digits = 0;
    while (number) {
        number /= 10;
        digits++;
    }
    return digits;
}

//trade
void Map::trade(string name, string give, string receive){
    if(name  == currentPlayer->name){
        cout<<"CANNOT TRADE WITH YOURSELF!"<<endl;
        return;
    }
    stringstream ss1{give};
    stringstream ss2{receive};
    int lenG= (int)give.length();
    int lenR = (int)receive.length();
    int giveMoney  = 0;
    int receiveMoney = 0;
    
    Player *p = nullptr;
    propertySqr *buildgive = nullptr;
    propertySqr *buildreceive = nullptr;
    
    for(auto x : playerInGame){//find player to trade with
        if(name == x->name){
            p = x;
            break;
        }
    }
    
    if(!p){//not found
        cout<<"Player to trade with does not exist."<<endl;
        return;
    }
    
    ss1>>giveMoney;
    ss2>>receiveMoney;
    
    if(giveMoney){//contain a int
        if(numDigits(giveMoney)!=lenG){
            cout<<"Wrong input of give."<<endl;
            return;
        }
    }else{//not contain int, search building
        for(int i = 0; i<numOfBuilding; i++){
            if(give==unit[i]->name){
                buildgive = dynamic_cast<propertySqr *>(unit[i]);
                break;
            }
            
        }
    }
    
    if(receiveMoney){//contain a int
        if(numDigits(receiveMoney)!=lenR){
            cout<<"Wrong input of reveive."<<endl;
            return;
        }
    }else{//not contain int, search building
        for(int i = 0; i<numOfBuilding; i++){
            if(receive==unit[i]->name){
                buildreceive = dynamic_cast<propertySqr *>(unit[i]);
                break;
            }
            
        }
    }
    
    if(buildgive&&buildreceive){
        p->askedTrade(currentPlayer, buildgive,buildreceive);
    }else if(giveMoney&&buildreceive){
        p->askedTrade(currentPlayer, giveMoney,buildreceive);
    }else if(buildgive&&receiveMoney){
        p->askedTrade(currentPlayer, buildgive,receiveMoney);
    }else if(receiveMoney&&giveMoney){
        p->askedTrade(currentPlayer,giveMoney,receiveMoney);
    }else{
        cout<<"Invalid trade."<<endl;
    }
    
    
}


propertySqr* Map::findProp(string name){
    for(int i = 0; i<numOfBuilding; i++){
        if(name==unit[i]->name){
            if(propertySqr*p = dynamic_cast<propertySqr*>(unit[i])){
                return p;
            }else{
                return nullptr;
            }
        }
    }
    return nullptr;
}
const int DCtim = 10;
Building* Map::getBuilding(int i){
    return  unit[i];
}



