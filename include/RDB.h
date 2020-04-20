#ifndef RDB_H
#define RDB_H
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include "NFA.h"
using namespace std;

class RDP
{
    public:
        RDP(vector<string> tokens, unordered_map<string ,NFA> defs);
        NFA toNFA();
    protected:

    private:
        vector<string> tokens;
        NFA concat();
        NFA unary();
        NFA primary();
        NFA charSet();
        NFA word();
        bool match(string token);
        bool isOperator();
        bool isChar();
        string current();
        vector<string>::iterator index;
        unordered_map<string ,NFA> defs;
};

#endif // RDB_H
