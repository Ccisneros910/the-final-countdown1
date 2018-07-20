////
//  Analytical.h
//  Sigma
//
//  Created 12/6/17.
//  Copyright © 2017.
//

#ifndef Anylitical_h
#define Anylitical_h


#include <iostream>
#include <string>

using namespace std;

class Analytical{
public:
    Analytical ();
    
    string getInstru();
    string getRegOr1();
    string getRegOr2();
    string getAction();
    string getRegizter();
    
    void setInstru(string nuInstru);
    void setRegOr1(string nuRegOr1);
    void setRegOr2(string nuRegOr2);
    void setAction(string nuAction);
    void setRegizter(string nuRegizter);
    
    
private:
    string instru;
    string regOrInstru1; //instru actually means literal
    string regOrInstru2;
    string actionBits;
    string regizter;
};

Analytical::Analytical()
{
    instru = " ";
    regOrInstru1= " ";
    regOrInstru2=" ";
    actionBits= " ";
    regizter= " ";
}
string Analytical :: getInstru()
{
    return instru;
}
string Analytical :: getRegOr1()
{
    return regOrInstru1;
}
string Analytical :: getRegOr2()
{
    return regOrInstru2;
}

string Analytical :: getAction()
{
    return actionBits;
}

string Analytical :: getRegizter()
{
    return regizter;
}

void Analytical :: setInstru(string nuInstru)
{
    instru = nuInstru;
}
void Analytical :: setRegOr1(string nuRegOr1)
{
    regOrInstru1 = nuRegOr1;
}
void Analytical :: setRegOr2(string nuRegOr2)
{
    regOrInstru2 = nuRegOr2;
}
void Analytical :: setAction(string nuAction)
{
    actionBits = nuAction;
}
void Analytical :: setRegizter(string nuRegizter)
{
    regizter= nuRegizter;
}
#endif /* Analytical_h */