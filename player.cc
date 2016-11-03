//
//  player.cpp
//  watopoly
//
//  Created by Zhihan Wen on 2016-07-14.
//  Copyright © 2016 Zhihan Wen. All rights reserved.
//

#include "player.h"
#include "academic.h"
#include "gym.h"
#include "residence.h"
#include "map.h"
#include "dctim.h"
#include <typeinfo>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <string>

using namespace std;

const int notInDc = 3;
const int initCash = 1500;
const double interest = 0.1;

Player::~Player(){
    
}

Player::Player(string name, char particle, Map *m):name{name}, particle{particle}, m{m}{
    cash = initCash;
    cups = 0;
    broken = false;
    turnInDC = notInDc;
    position = nullptr;
    coordX = 10;
    coordY = 10;
    locationIndex = 0;
}

Player::Player(std::string name, char particle,int cups,int cash, int pos, Map *m):name{name}, particle{particle}, m{m}{
    this->cash = cash;
    this->cups = cups;
    this->locationIndex = pos;
    turnInDC = notInDc;
    position = nullptr;
    coordX = 10;
    coordY = 10;
}

Player::Player(std::string name, char particle,int cups,int cash, int pos,int dc, Map *m):name{name}, particle{particle}, m{m}{
    this->cash = cash;
    this->cups = cups;
    this->locationIndex = pos;
    turnInDC = dc;
    position = nullptr;
    coordX = 10;
    coordY = 10;
}

void Player::notify(Building *b){
    position = b;
    locationIndex = b->number;
    notifyOb();
}

void Player::notify(Building *b,int roll1, int roll2){
    position = b;
    locationIndex = b->number;
    if(DcTim * d = dynamic_cast<DcTim *>(b)){
        d->notify(this,roll1,roll2);
    }
}

void Player::notifyOb(){
    position->notify(this);
}

bool Player::isBroken(){
    return broken;
}

const int Player::getCash(){
    return cash;
}

const int Player::getCup(){
    return cups;
}

void Player::addCup(){
    cups++;
}

void Player::lostCup(){
    cups--;
}

void Player::goToDC(Building *dc){
    coordX = 0;
    coordY = 10;
    position = dc;
    turnInDC = 0;
}

void Player::getPaid(int n){
    cash += n;
}

int Player::total(){
    int sum = 0;
    for(auto x : ownings) {
        for(auto y : x){
            sum +=  y->TotalVal();
        }
    }
    sum += cash;
    return sum;
}

bool Player::blockImp(Academic *a){
    bool haveIMP = false;
    
    
    for(auto x : ownings){
        if(Academic *a1 = dynamic_cast<Academic *>(x[0])){
            if(a1->getBlock() == a->getBlock()){
                for(auto y : x){
                    if(dynamic_cast<Academic*>(y)->getImp() != 0){
                        haveIMP = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    
    return haveIMP;
}


//pay

void Player::bankruptcy(){//auction
    broken = true;
    int i = 0;
    for (auto x : ownings){
        
        int j =0;
        for(auto y : ownings[i]){
            
            if(Academic *a = dynamic_cast<Academic *>(ownings[i][j])){
                
               a->clearImp();
            }
            
            if(y->mortagaged){
                y->changeMortagageStatus();
            }
            y->changeOwner(nullptr);
            m->auction(y);
            j++;
        }
        i++;
    }
    
}

void Player::bankruptcy(Player *p){//give everything to who you owe
    broken = true;
    p->getPaid(cash);
    for (auto x : ownings){
        
        for(auto y : x){
            if(Academic *a = dynamic_cast<Academic *>(y)){
                a->sellImp(p);
            }
            if(y->mortagaged){
                p->buyMort(y);
            }
            y->changeOwner(p);
            
        }
    }
    
}

const int bankR = 1;
const int raise = 2;

int Player::ifToBreak(){
    cin.exceptions(ios::eofbit|ios::failbit);
    string s;
    //display message
    cout<<"Sorry, you need more cash..."<<endl;
    cout<<"You may either choose to:"<<endl;
    cout<<"1. Declare bankruptcy"<<endl;
    cout<<"2. Sell or mortgage your ownings to raise money"<<endl;
    cout<<"Enter 3 to check your assets"<<endl;
    cout<<"Please enter 1 or 2 or 3 to choose"<<endl;
    
        while(1){
            try{
                cin>>s;
                if(s=="bankrupt"){
                    return  bankR;
                }else if(s=="R"){
                    return raise;
                }else if(s =="assets"){
                    asset();
                    cout<<"You may either choose to:"<<endl;
                    cout<<"1. Declare bankruptcy"<<endl;
                    cout<<"2. Sell your ownings to raise money"<<endl;
                    cout<<"If you would like to raise money, please enter R"<<endl;
                    cout<<"3. Check your assets"<<endl;
                }else{
                    cout<<"You may either choose to:"<<endl;
                    cout<<"1. Declare bankruptcy"<<endl;
                    cout<<"2. Sell your ownings to raise money"<<endl;
                    cout<<"If you would like to raise money, please enter R"<<endl;
                    cout<<"3. Check your assets"<<endl;
                }
            }
            catch(...){
                if(cin.eof()){
                    throw;
                }
                cout<<"You may either choose to:"<<endl;
                cout<<"1. Declare bankruptcy"<<endl;
                cout<<"Enter: bankrupt"<<endl;
                cout<<"2. Sell your ownings to raise money"<<endl;
                cout<<"Enter: R"<<endl;
                cout<<"3. Check your assets"<<endl;
                cout<<"Enter: assets"<<endl;
                cin.clear();
                cin.ignore();
            }
        }
    
    
}


void Player::pay(int n){//owe the bank
    
    if(n > cash){
        if(total()<n){
            cout<<"You have no way to pay for your debt :)"<<endl;
            bankruptcy();
            return;
        }
        int i = ifToBreak();
        if (i == bankR){
            cout<<"You have declared bankrupcty."<<endl;
            bankruptcy();
        }else if (i == raise){
            cout<<"You can either sell your improvements or mortgage your buildings"<<endl;
            cout<<"Note that you can only check assets, sell improvement, mortgage, or declare bankruptcy at this point."<<endl;
            cout<<"Enter command line to make action: "<<endl;
            
            while(1){
                while(1){//take in command to raise money
                    try{
                    string s;
                    cin>>s;
                    if(s == "improve"){
                        cin>>s;
                        bool find = false;
                        for (auto x : ownings) {
                            for (auto y : x) {//find property
                                if(s == y->name){//check name
                                    if(Academic *a = dynamic_cast<Academic *>(y)){//check type
                                        if(a->getImp()==0){//check imp
                                            cout<<"Cannot sell improvement if there is no improvement."<<endl;
                                        }else{//sell imp
                                            a->sellImp(this);
                                        }
                                        find = true;
                                        break;
                                    }
                                }
                            }
                            if(find)break;
                        }
                        if(!find){
                            cout<<"Cannot sell improvement if there is no improvement."<<endl;
                        }
                        break;
                    }else if(s=="mortgage"){
                        cin>>s;
                        bool find = false;
                        for (auto x : ownings) {
                            for (auto y : x) {//find property
                                if(s == y->name){
                                    mortgage(y);//check name
                                    find = true;
                                    break;
                                }
                            }
                            if(find)break;
                        }
                        if(!find){
                            cout<<"Cannot mortgage things you don't have."<<endl;
                        }
                        break;
                    }else if(s=="bankrupt"){
                        bankruptcy();
                        break;
                    }else if(s=="assets"){
                        asset();
                        break;
                    }else{
                        cout<<"You can only sell improvement, mortgage, or declare bankruptcy."<<endl;
                    }
                }
                    catch(...){
                        if(cin.eof()){
                            throw;
                        }else{
                            cout<<"You can only sell improvement, mortgage, or declare bankruptcy."<<endl;
                            cin.clear();
                            cin.ignore();
                        }
                        
                    }
                }
                if(cash >= n){//pay now
                    cash -= n;
                    cout<<"Payment completed"<<endl;
                    return;
                }
            }

        }else{
            cout<<"Payment of "<<name<<" failed, reached EOF"<<endl;
            return;
        }
    }else{
        cash -= n;
        cout<<"Payment completed"<<endl;
        cout<<"Your current cash: "<<cash<<endl;
    }
}

void Player::pay(int n, Player *p){//owe to player
    cout<<"You now owe "<<p->name<<" "<<n<<endl;
    
    asset();
    
    if(total()<n){
        cout<<"But you have no way to pay for your debt :)"<<endl;
        bankruptcy(p);
        return;
    }
    if (n > cash){
        int i = ifToBreak();
        if (i == bankR){
            cout<<"You have declared bankrupcty."<<endl;
            bankruptcy(p);
        }else if (i == raise){
            cout<<"You can either sell your improvements or mortgage your buildings"<<endl;
            cout<<"Note that you can only sell improvement, mortgage, or declare bankruptcy at this point."<<endl;
            cout<<"Enter command line to make action: "<<endl;
            
            while(1){
                while(1){//take in command to raise money
                    try{
                    string s;
                    cin>>s;
                    if(s == "improve"){
                        cin>>s;
                        bool find = false;
                        for (auto x : ownings) {
                            for (auto y : x) {//find property
                                if(s == y->name){//check name
                                    if(Academic *a = dynamic_cast<Academic *>(y)){//check type
                                        if(a->getImp()==0){//check imp
                                            cout<<"Cannot sell improvement if there is no improvement."<<endl;
                                        }else{//sell imp
                                            a->sellImp(this);
                                        }
                                        find = true;
                                        break;
                                    }
                                }
                            }
                            if(find)break;
                        }
                        if(!find){
                            cout<<"Cannot sell improvement if there is no improvement."<<endl;
                        }
                        break;
                    }else if(s=="mortgage"){
                        cin>>s;
                        bool find = false;
                        for (auto x : ownings) {
                            for (auto y : x) {//find property
                                if(s == y->name){
                                    mortgage(y);//check name
                                    find = true;
                                    break;
                                }
                            }
                            if(find)break;
                        }
                        if(!find){
                            cout<<"Cannot mortgage things you don't have."<<endl;
                        }
                        break;
                    }else if(s=="bankrupt"){
                        bankruptcy(p);
                        break;
                    }else if(s=="assets"){
                        asset();
                        break;
                    }else{
                        cout<<"You can only sell improvement, mortgage, or declare bankruptcy."<<endl;
                    }
                    }
                    catch(...){
                        if(cin.eof()){
                            throw;
                        }else{
                            cout<<"You can only sell improvement, mortgage, or declare bankruptcy."<<endl;
                            cin.clear();
                            cin.ignore();
                        }
                    }
                    
                }
                if(cash >= n){//pay now
                    cash -= n;
                    p->getPaid(n);
                    cout<<"Payment completed"<<endl;
                    cout<<"Your current cash: "<<cash<<endl;
                    return;
                }
            }

        }else{
            cout<<"Payment of "<<name<<" to "<< p->name<<" failed, reached EOF"<<endl;
            return;
        }
    }else{
        cash -= n;
        p->getPaid(n);
        cout<<"Payment completed"<<endl;
        cout<<"Your current cash: "<<cash<<endl;
    }
}



//assets
void Player::asset(){
    cout<<"Now displaying the assets of player: "<<name<<"."<<endl;
    
    //display cash
    cout<<"Money: "<<cash<<endl;
    
    //display propertysqr and price
    
    bool ifAca = false;
    for(auto x : ownings){

        if (dynamic_cast<Academic*>(x[0])){ifAca =true;}
        else{ifAca = false;}
        for(auto y : x){
            
            int price = y->getPrice();
            bool mort = y->mortagaged;
            
            cout<<y->name;
            if(ifAca)cout<<" "<<dynamic_cast<Academic *>(y)->getBlock()<<" ";
            cout<<" Price:"<<price;
            if(mort)cout<<" Mortgaged";
            cout<<endl;
            
            if(ifAca){
                int imp = dynamic_cast<Academic *>(y)->getImp();
                int impPrice = dynamic_cast<Academic *>(y)->getImpCost();
                cout<<"Improvements: "<<imp<<" Price of each:"<<impPrice<<endl;
            }
        }
       
    }
}




void Player::improve(Academic *b){
    bool belong = false;
    int payMono = b->getImpCost();
    int numMono = 0;
    string block = b->getBlock();
    int imp = b->getImp();
    
    //check ownings
    for(auto x : ownings){
        if (dynamic_cast<Academic*>(x[0])){
            for(auto y : x){
                if(y->name == b->name){
                    belong=true;
                    numMono = (int)x.size();
                    break;
                }
            }
        }
        if(belong)break;
    }
    if(!belong){
        cout<<"Cannot improve building not belonging to you."<<endl;
        return;
    }
    
    //check monopoly
    if ((block == A2||block == E||block == H||block == Ev||block == S1||block == S2)){
        if(numMono<mA2){
            cout<<"Need to collect all buildings in the monopoly to get improvement :)"<<endl;
            return;
        }
    }else{
        if(numMono<mA1){
            cout<<"Need to collect all buildings in the monopoly to get improvement :)"<<endl;
            return;
        }
    }
    
    //check imp
    const int maxImp = 5;
    if(imp >=maxImp){
        cout << "Reach the limit of Improvements!" << endl;
        return;
    }
    
    //check cash
    if(cash<payMono){
        cout<<payMono<<"needed! "<<"No enough money, please try again later."<<endl;
        return;
    }
    
    //pay
    cash -= payMono;
    //add imp
    b->addImp(this);
    cout<<"Improvement added successfully!"<<endl;
    
}



///////////////////////////////OWNINGS//////////////////////////////////////////////
void Player::addOwning(propertySqr *b){
    int i  = 0;
    for(auto x : ownings){
        //check if exist
        if(std::find(x.begin(), x.end(), b) != x.end()) {
            cout<<"Cannot add what you already include."<<endl;
            return;
        }
        Academic *a = dynamic_cast<Academic*>(b);
        Academic *c = dynamic_cast<Academic*>(x[0]);
        if(c&&a){//if this vector stores academic
            if (c->getBlock() == a->getBlock()){//check monopoly
                ownings[i].push_back(a);
                return;
            }
            
        }else if ((dynamic_cast<Gym*>(x[0]))&&(dynamic_cast<Gym*>(b))){//if this vector stores gym
            ownings[i].push_back(b);
            return;
        }else if((dynamic_cast<Residence *>(x[0]))&&(dynamic_cast<Residence *>(b))){
            
            ownings[i].push_back(b);
            return;
            
        }
        i++;
    }
    
    vector<propertySqr *> newB;
    newB.emplace_back(b);
    ownings.emplace_back(newB);
}

/////////remove////////
void Player::removeOwnings(propertySqr *b){
    
    //check belongings
    bool belong = false;
    int j = 0;
    for(auto x : ownings){
        int i = 0;
        for(auto y : x){
            if(y->name == b->name){
                belong=true;
                ownings[j].erase(ownings[j].begin()+i);
                break;
            }
            ++i;
        }
        
        if(ownings[j].empty()){
            ownings.erase(ownings.begin()+j);
        }
        ++j;
        if(belong)return;
    }
    if(!belong){
        cout<<"Cannot remove building not belonging to you."<<endl;
        return;
    }
    
}




////////////////////////////HELPER////////////////////////////////////////////
//accept of reject
bool aOrR(){
    string s;
    bool accept = true;
    while(1){
        try{
        if(cin>>s){
            if(s == "accept"){
                return accept;
            }else if(s == "reject"){
                accept = false;
                return accept;
            }else{
                cout<<"Please enter valid command: accept or reject"<<endl;
            }
        }
    }
    catch(...){
        if(cin.eof()){
            throw;
        }
    }
    }
}


///////////////////////////////TRADE/////////////////////////////////////////

//property with property
void Player::askedTrade(Player *p, propertySqr *otherB, propertySqr *myB){
    if(myB->mortagaged){
        cout<<"This building is currently mortgaged."<<endl;
        cout<<"You will have to pay "<<myB->getPrice()*interest<<" if you buy it"<<endl;
    }
    ////////check the belongings////////
    if(p != otherB->getOwner()){
        cout<<"Cannot trade things not belong to you."<<endl;
        return;
    }
    if(this != myB->getOwner()){
        cout<<"Can only trade things with its owner."<<endl;
        return;
    }
    /////////check improvement///////////
    if(Academic* a1 = dynamic_cast<Academic*>(myB)){
        if(p->blockImp(a1)){
            cout<< "Cannot trade property in improved block"<<endl;
            return;
        }
    }
    if(Academic* a2 = dynamic_cast<Academic*>(otherB)){
        if(p->blockImp(a2)){
            cout<< "Cannot trade property in improved block"<<endl;
            return;
        }
    }
    ////////////////////////////////////
    cout<<name<<", "<<endl;
    cout<<p->name<<" asked to exchange "<<otherB->name<<" with "<<myB->name<<"."<<endl;
    if(otherB->mortagaged){
        cout<<"This building is currently mortgaged."<<endl;
        cout<<"You will have to pay "<<otherB->getPrice()*interest<<" if you accept it"<<endl;
    }
    cout << "Please enter accept or reject: "<<endl;
    bool accept = aOrR();
    if(accept){
        otherB->changeOwner(this);
        
        if(myB->mortagaged)p->buyMort(myB);
        removeOwnings(myB);
        myB->changeOwner(p);
        
        if(otherB->mortagaged)buyMort(otherB);
        p->removeOwnings(otherB);
        cout<<"Trade completed."<<endl;
    }else{
        cout<<"Trade rejected."<<endl;
    }
}

//money with property///////////////////////////////////
void Player::askedTrade(Player *p, int oMoney, propertySqr *myB){
    if(myB->mortagaged){
        cout<<"This building is currently mortgaged."<<endl;
        cout<<"You will have to pay "<<myB->getPrice()*interest<<" if you buy it"<<endl;
    }
    ////////check the belongings////////
    if(p->getCash()<oMoney){
        cout<<"You don't have enough money to trade"<<endl;
        return;
    }
    if(this != myB->getOwner()){
        cout<<"Can only trade things with their owner."<<endl;
        return;
    }
    /////////check improvement///////////
    if(Academic* a = dynamic_cast<Academic*>(myB)){
        if(p->blockImp(a)){
            cout<< "Cannot trade property in improved block"<<endl;
            return;
        }
    }

    cout<<name<<", "<<endl;
    cout<<p->name<<" asked to exchange "<<oMoney<<" with "<<myB->name<<"."<<endl;
    cout << "Please enter accept or reject: "<<endl;
    bool accept = aOrR();
    if(accept){
        p->pay(oMoney);
        getPaid(oMoney);
        myB->changeOwner(p);
        removeOwnings(myB);
        
        if(myB->mortagaged)p->buyMort(myB);
        cout<<"Trade completed."<<endl;
    }else{
        cout<<"Trade rejected."<<endl;
    }
}

//property with money/////////////////////////////////////
void Player::askedTrade(Player *p, propertySqr *otherB, int myMoney){
    ////////check the belongings////////
    if(p != otherB->getOwner()){
        cout<<"Cannot trade things not belong to you."<<endl;
        return;
    }
    /////////check improvement///////////
    if(Academic* a = dynamic_cast<Academic*>(otherB)){
        if(p->blockImp(a)){
            cout<< "Cannot trade property in improved block"<<endl;
            return;
        }
    }
    ////////////////trade////////////////////
    cout<<name<<", "<<endl;
    cout<<p->name<<" asked to exchange "<<otherB->name<<" with "<<myMoney<<"."<<endl;
    if(otherB->mortagaged){
        cout<<"This building is currently mortgaged."<<endl;
        cout<<"You will have to pay "<<otherB->getPrice()*interest<<" if you accept it"<<endl;
    }
    cout << "Please enter accept or reject: "<<endl;
    bool accept = aOrR();
    if(accept){
        if(cash >= myMoney){
            pay(myMoney);
            p->getPaid(myMoney);
            p->removeOwnings(otherB);
            otherB->changeOwner(this);
            
            if(otherB->mortagaged)buyMort(otherB);
            cout<<"Trade completed."<<endl;
        }else{
            cout<<"No enough cash, come back later."<<endl;
        }
        
    }else{
        cout<<"Trade rejected."<<endl;
    }
}

//reject money with money
void Player::askedTrade(Player *p, int oMoney, int myMoney){
    cout<<"Cannot trade money with money"<<endl;
}
///////////////////////////////TRADE///OVER//////////////////////////////////////

void Player::mortgage(propertySqr *p){
    //check owner
    if(p->getOwner() != this){
        cout<<"Cannot mortgage building not belong to you!"<<endl;
        return;
    }
    
    //check improvement
    if(Academic* a = dynamic_cast<Academic*>(p)){
        if(a->getImp()){
            cout<<"Cannot mortgage building with improvement!"<<endl;
            return;
        }
    }
    
    //check mortgage status
    if(p->mortagaged){
        cout<<"Cannot mortgage building that has already been mortgaged!"<<endl;
        return;
    }
    
    p->changeMortagageStatus();
    getPaid(p->getPrice()/2);
    cout<<"Mortgage succeeded: "<< p->name<<endl;
    
}

void Player::unmortgage(propertySqr *p){
    const double unmortRate = 0.6;
    if (p->getOwner() != this) {
        cout << "You can not unmortagage other's building!" << endl;
        return;
    }
    
    if(!p->mortagaged){
        cout<<"Cannot unmortgage building that is not mortgaged!"<<endl;
        return;
    }
    
    int cashNeeded =p->getPrice()*unmortRate;
    if(cash < cashNeeded ){
        cout << "You don't have enough money to unmortgage! You need "<<cashNeeded << endl;
        return;
    }
    
    pay(cashNeeded);
    p->changeMortagageStatus();
    cout << "You successfully unmortagage building " << p->name  << "!" << endl;
    return;
    
}

void Player::sellImp(Academic *a){
    if(this != a->getOwner()){
        cout << "Cannot sell things do not belong to you!"<<endl;
        return;
    }
    
    a->sellImp(this);
}

void Player::buyMort(propertySqr *b){
    int payItst = interest * b->getPrice();
    pay(payItst);
    
    cout<<"Would you like to unmortgage the building now?(Y/N)"<<endl;
    
    string s;
    try{
    while(cin>>s){
        if(s=="Y"){
            if(cash < b->getPrice()/2){
                cout<<"Unmortgage failed"<<endl;
                cout<<"Do not have enough cash"<<endl;
                
            }else{
                pay(b->getPrice()/2);
                b->changeMortagageStatus();
                cout<<"Unmortgage succeeded."<<endl;
            }
            break;
        }else if(s=="N"){
            break;
        }else{
            cout<<"Please enter Y or N"<<endl;
        }
    }
    }
    catch(...){
        throw;
    }
}


Map* Player::getMap(){
    return m;
}



