#ifndef DFADRIVER_H
#define DFADRIVER_H
#include <string>
#include <vector>
#include "DFA.h"
#include "SymbolTable.h"
using namespace std;

class DFADriver
{
    public:
        DFADriver(DFA* dfa);
        void setInputFile(string path);
        void start();
        vector<string> getTokens();
        SymbolTable* getSymbolTable();
        void produceOutputFile();

    protected:

    private:
        DFA* dfa;
        vector<string> tokens;
        string inputBuf;
        SymbolTable* symbolTable;
};

#endif // DFADRIVER_H
