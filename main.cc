//  main.cpp
//  watopoly
//
//  Created by Zhihan Wen on 2016-07-14.
//  Copyright © 2016 Zhihan Wen. All rights reserved.
//


//Official rules consulted:
//http://richard_wilding.tripod.com/monorules.htm

#include <iostream>
#include <vector>
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
#include <sstream>

using namespace std;

class player;
const string TEST = "-testing";
const string LOAD = "-load";

const string roll = "roll";
const string NEXT = "next";
const string trade = "trade";
const string improve = "improve";
const string buy = "buy";
const string sell = "sell";
const string mort  = "mortgage";
const string unmort = "unmortgage";
const string br = "bankrupt";
const string asset = "assets";
const string all = "all";
const string save = "save";

int readRoll(int low, int high){
    char c;
    cin>>noskipws>>c;
    if(c=='\n')return -1;
    int i;
    cin>>skipws>>i;
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


void gameStart(Map *m, bool test){
    cout<<*m;
	bool rolled = false;
    string s;
    stringstream ss;
    while(1){
        if(m->currentPlayer->isBroken()){
            m->nextPlayer();
        }
        cout<<m->currentPlayer->name<<"'s turn"<<endl;
        cout<<"Please enter command: "<<endl;
        //start input
        try{
        cin >>skipws>>s;
        if(s==roll){//roll
            
            if(rolled){
                cout<<"cannot roll anymore"<<endl;
            }else{
                if(test){
                    int i1 = readRoll(0,numOfBuilding-1);
                    int i2 = readRoll(0,numOfBuilding-1);
                    if(i1>=0&&i2>=0){
                        m->rollDice(i1, i2);}
                    else{
                        m->rollDice();
                    }
                    rolled = true;
                }else{
                    m->rollDice();
                    rolled = true;
                }
                rolled = true;
                cout<<noskipws<<*m<<endl;
            }
            //cout<<m->currentPlayer->locationIndex<<endl;
            //cout<<"("<<m->currentPlayer->coordX<<","<<m->currentPlayer->coordY<<")"<<endl;
            
            
        }else if (s==NEXT){//next
            if(rolled){
            	rolled = false;
                m->nextPlayer();
                rolled = false;
            }else{
                cout<<"Please roll."<<endl;
            }
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s == trade){
            string name;
            string give;
            string receive;
            cin >>skipws>>name;
            cin >>skipws>>give;
            cin >>skipws>>receive;
            m->trade(name, give, receive);
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s == improve){
            string name;
            cin >>skipws>>name;
            propertySqr *p = m->findProp(name);
            cin >>skipws>>s;
            if(s!="buy"&&s != "sell"){
                cout<<"Please enter valid command."<<endl;
            }else{
                if(p){
                    if(Academic *a = dynamic_cast<Academic *>(p)){
                        if(s=="buy"){
                            m->currentPlayer->improve(a);
                        }else if(s=="sell"){
                            m->currentPlayer->sellImp(a);
                        }
                    }else{
                        cout<<"Can only improve Academic buildings."<<endl;
                    }
                    
                }else{
                    cout<<"Building not found."<<endl;
                }
            }
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s == mort){
            string name;
            cin >>skipws>>name;
            propertySqr *p = m->findProp(name);
            m->currentPlayer->mortgage(p);
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s == unmort){
            string name;
            cin >>skipws>>name;
            propertySqr *p = m->findProp(name);
            m->currentPlayer->unmortgage(p);
            
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s == br){//bankrupt
            cout<<"Cannot declare bankruptcy at this point."<<endl;
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s == asset){
            m->currentPlayer->asset();
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s == all){
            m->allAsset();
            
            cout<<noskipws<<*m<<endl;
            
        }else if (s== save){
            cin  >>skipws>>s;
            m->saveGame(s);
        
            
            
        }else if(s == "q"){
            return;
        }else{
            if(cin.eof())break;
            cout<<"Please enter valid command."<<endl;
        }
        }
        catch(...){
            throw;
        }
        //cout<<noskipws<<*m<<endl;
        if(m->getNumPlayer() ==1){
            cout<<m->oneLeft()<<" won the game!"<<endl;
            return;
        }
    }
}


int main(int argc, const char * argv[]) {
    stringstream ss;
    string s;
    int numPlayer;
    bool test = false;
    try{
        if(argc == 1){//new game
            cout << "How many players?" << endl;
            numPlayer = readInt(6,8);
            //cin >>skipws>>numPlayer;
            
            Map *game = new Map(numPlayer);
            gameStart(game,test);
            delete game;
        }else if(argc ==2){//testing mode & new game
            if(argv[1]==TEST){
                test = true;
                cout << "How many players?" << endl;
                numPlayer = readInt(2, 8);
                Map *game = new Map(numPlayer);
                gameStart(game,test);
                delete game;
            }else{
                cout<<argv[1]<<": command not found"<<endl;
                return 0;
            }
        }else if(argc ==3){//load game
            if(argv[1]==LOAD){
                Map *game = new Map(argv[2]);
                gameStart(game,test);
                delete game;
            }else{
                cout<<argv[1]<<": command not found"<<endl;
                return 0;
            }
        }else if(argc == 4){//testing & load
            if(argv[1]==LOAD){
                if(argv[3]==TEST){
                    Map *game = new Map(argv[2]);
                    test =true;
                    gameStart(game,test);
                    delete game;
                }else{
                    cout<<argv[3]<<": command not found"<<endl;
                    return 0;
                }
            }else if(argv[1]==TEST){
                if(argv[2]==LOAD){
                    Map *game = new Map(argv[3]);
                    test =true;
                    gameStart(game,test);
                    delete game;
                }else{
                    cout<<argv[2]<<": command not found"<<endl;
                    return 0;
                }
            }
        }else{//incorrect num of args
            cout<<"Incorrect number of args."<<endl;
            return 0;
        }
        
        while(1){
            cout<<"Another round?(Y/other)"<<endl;
            cin >>skipws>>s;
            if(s=="Y"){
                cout << "How many players?" << endl;
                if(test){
                    numPlayer = readInt(2,8);
                }else{
                    numPlayer = readInt(6,8);
                }
                Map *game = new Map(numPlayer);
                gameStart(game,test);
                delete game;
            }else{
                break;
            }
        }
    }
    catch(...){
        cout<<"Reach EOF"<<endl;
        return 0;
    }
    
    cout<<"Bye~"<<endl;
    return 0;
}



