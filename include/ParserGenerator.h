#ifndef PARSERGENERATOR_H
#define PARSERGENERATOR_H
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "../include/Rule.h"
using namespace std;

class ParserGenerator
{
    public:
        ParserGenerator(vector<Rule*> grammer);
        void generateParsingTable();
        void printParsingTable();
        void printParsingTable(string outputFile);
        unordered_map<string,unordered_map<string, vector<Rule*>>>getParsingTable();
        // parse(next token)

    protected:

    private:
        vector<Rule*> grammer;
        //string:nonTerminal , map:terminal, table entry = rule
        unordered_map<string,unordered_map<string, vector<Rule*>>> parsingTable;
        bool eps;
        void calcFirst(Rule* lhs);
        void calcFollow(Rule* lhs);
        void calcNullable();
        void addToTable();
        set<string>checkEps(vector<set<string>> f , bool lastSymbol);
};

#endif // PARSERGENERATOR_H
