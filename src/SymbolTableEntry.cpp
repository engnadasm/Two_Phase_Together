#include "../include/SymbolTableEntry.h"
#include <string>
SymbolTableEntry::SymbolTableEntry()
{
    //ctor
    className = "";
}

void SymbolTableEntry::setClassName(string type){

    className = type;
}
