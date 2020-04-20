#ifndef DERIVATION_H
#define DERIVATION_H
#include <iostream>
#include <queue>          // std::queue
#include <stack> // std::stack
#include<set>
#include <string>
#include <vector>
#include <unordered_map>
#include "../include/Rule.h"
using namespace std;

class Derivation
{

    public:
        static Derivation* GetInstance();
        void generateDerivations();
        void enterToken(string token);
        void setParsingTable(unordered_map<string,unordered_map<string, vector<Rule*>>> parsingTable);
        void setStart(Rule* start);
        bool getAccept();
        void endDerivation();

    protected:

    private:
        Derivation();
        void PrintDerivations();
        string PrintStack(stack<Rule*> s);
        static Derivation* derivationInstance;		// singleton instance
        queue<string> tokens;
        string outToAdd = "";
        stack<Rule*> ourStack;
        vector<string> outToPrint;
        int currentPointer = 0;
        unordered_map<string,unordered_map<string, vector<Rule*>>> ourParsingTable;
        bool accept = true;
};

#endif // DERIVATION_H
