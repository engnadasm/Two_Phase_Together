#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <string>
#include "SymbolTableEntry.h"
#include <string>
#include <unordered_map>
using namespace std;


class SymbolTable
{
    public:
        SymbolTable();
        void free();
        SymbolTableEntry* lookup(string symbol);
        SymbolTableEntry* insert(string symbol, string className);
        //set attributes
        //get attributes

    protected:

    private:
        unordered_map< string, SymbolTableEntry* >symtab;
};

#endif // SYMBOLTABLE_H
