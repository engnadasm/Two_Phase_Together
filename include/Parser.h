#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include <iostream>
#include <fstream>
#include "NFA.h"
#include "RDB.h"

using namespace std;


class Parser
{
    public:
        Parser(string path);
        void parse();
        NFA* getCombinedNFA();
        vector<string> segmentation(string line);
    protected:

    private:
        void parseSegs(vector<string> segs);
        void createExp(vector<string> segs);
        void createDef(vector<string> segs);
        void createKeywords(vector<string> segs);
        int getOperator(char c);
        string path;
        vector<NFA> exp;
        unordered_map<string ,NFA> defs;
        NFA* nfa;
        int order;

};

#endif // PARSER_H
