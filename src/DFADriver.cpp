#include "../include/DFADriver.h"
#include <fstream>
#include "../include/Derivation.h"
#include "../include/ErrorMessages.h"
DFADriver::DFADriver(DFA* dfa)
{
    this->dfa = dfa;
    this->symbolTable = new SymbolTable();
    inputBuf = "";
}
void DFADriver::setInputFile(string path)
{
    string line;
    ifstream file(path);
    if (file.is_open())
    {
        // cout <<"enter\n";
        while(getline(file, line))
        {
            //  cout << line << "\n";
            inputBuf += line;
        }
        file.close();
    }
}
void DFADriver::start()
{
    unsigned int i = 0;
    unsigned int j = 0;
    while( j < inputBuf.size())
    {
        while((!dfa->isDead())&& (j+i < inputBuf.size()))
        {
            // cout<< dfa->getCurrentState();
            dfa->move(inputBuf[j+i]);
            i++;
        }
        string token = dfa->getToken();
        string lexeme = dfa->getLexeme();
        /* cout<<"input sequence: "<<dfa->getInputSequence()<<endl;
         cout<<"token: "<<token<<endl;
         cout<<"lexeme: "<<lexeme<<endl;*/
        //no match exists
        if(token == "")
        {
            if(dfa->getInputSequence()== " " || dfa->getInputSequence()== "\n") // No Error
            {
                j++;
            }
            else
            {
                //call error recovery routine to produce an error message
                produceLexicalError(dfa->getInputSequence());
                j++;
            }

        }//match exist
        else
        {
            //cout<<"match"<<endl;
            tokens.push_back(token);

            //for phase_2
            Derivation* der;
            der = Derivation::GetInstance();
            if(token[0] != ',')
            {
                cout<< token << "\n";
                der->enterToken(token);
            }
            //insert identifiers into symbol table
            if(token == "id")
            {
                symbolTable->insert(lexeme, token);
                // cout<<"symtab"<<endl;
            }
            j = j + lexeme.size();
        }

        i = 0;
        dfa->reset();
        // cout<< "j:"<<j<<endl;
    }
}
vector<string> DFADriver::getTokens()
{
    return tokens;
}
SymbolTable* DFADriver::getSymbolTable()
{
    return symbolTable;
}
void DFADriver::produceOutputFile()
{
    //for phase_2
    Derivation* der;
    der = Derivation::GetInstance();
    der->endDerivation();

    ofstream myfile ("lexical file.txt");
    if (myfile.is_open())
    {
        vector< string >::iterator i = tokens.begin();
        while (i != tokens.end())
        {
            myfile << *i;
            myfile << "\n";
            i++;
        }
        myfile.close();
    }
    else
        cout << "Unable to open file";
}
