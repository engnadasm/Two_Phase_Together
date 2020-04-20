#include "../include/SymbolTable.h"

SymbolTable::SymbolTable()
{
    //ctor
}
void SymbolTable::free(){
}
SymbolTableEntry* SymbolTable::lookup(string symbol){

}
SymbolTableEntry* SymbolTable::insert(string symbol, string className){
    symtab[symbol] = new SymbolTableEntry();
    symtab[symbol]->setClassName(className);
}
