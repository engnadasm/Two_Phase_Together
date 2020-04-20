#ifndef DFAMINIMZER_H
#define DFAMINIMZER_H
#include "DFA.h"
#include <set>
#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

class DFAMinimzer
{
public:
    DFAMinimzer(DFA* dfa);
    void minimize();
    DFA* getMinimizedDFA();
protected:

private:
    vector<unordered_map<int, string>> doPartition();//Construct New Partition
    void oneStateOfEachGroup();
    DFA* dfa;
    DFA* newDfa;
    bool flag = true;
    int counter = 2;
    map<int, int> groupStates;
    set<char> inputs ;
    int initState;
    int numStates;
    unordered_map<int, string> acceptStates;
    vector <unordered_map<int, string>> partitions;
};

#endif // DFAMINIMZER_H
