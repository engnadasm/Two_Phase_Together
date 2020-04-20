#ifndef GRAMMERPARSER_H
#define GRAMMERPARSER_H
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "Rule.h"

using namespace std;


class GrammerParser
{
    public:
        GrammerParser(string inputFile);
        void parseRules();
        vector<Rule*> getRules();

    protected:

    private:
        string path;
        unordered_map<string, vector<string>> passOne;
        void addRule(string r);
        vector<string> segmentation(string line);
        vector<Rule*> rules;
        unordered_map<string, int> indexes;
        unordered_map<string, Rule*> terminals;
};


// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static inline void printRule(Rule r){
    cout << r.getName() << endl;
    vector<vector<Rule*>> p = r.getProductions();
    for(int j = 0; j < p.size(); j++){
        cout << "production " << j << " :";
        for(int k = 0; k < p[j].size(); k++){
            cout << " " <<p[j][k]->getName();
        }
        cout << endl;
    }
}
#endif // GRAMMERPARSER_H
