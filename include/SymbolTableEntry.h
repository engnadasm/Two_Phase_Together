#ifndef SYMBOLTABLEENTRY_H
#define SYMBOLTABLEENTRY_H
#include<string>
using namespace std;
class SymbolTableEntry
{
    public:
        SymbolTableEntry();
        void setClassName(string type);
    protected:

    private:
        string className;

};

#endif // SYMBOLTABLEENTRY_H
