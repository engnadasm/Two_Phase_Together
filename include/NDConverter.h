#ifndef NDCONVERTER_H
#define NDCONVERTER_H
#include "../include/NFA.h"
#include "../include/DFA.h"
#include <iostream>

using namespace std;

class NDConverter
{
    public:
        NDConverter(NFA* n);
        void convert();
        DFA* getDFA();

    protected:

    private:
        NFA* nfa;
        DFA* dfa;
        set<int> acceptStates;
        set<char> inputSymbols ;
        void checkAccept(set<int> U, int num);
};

#endif // NDCONVERTER_H
