#include "../include/DFA.h"
#include <fstream>
#include <iostream>

DFA::DFA(set<char> inputs, int initState)
{
    this->inputs = inputs;
    this->initState = initState;
    this->currentState = initState;
    lastValidToken = "";
    lexeme = "";
    inputSequence = "";
    initTransitionTableEntry(initState);
}

void DFA::initTransitionTableEntry(int state)
{
    unordered_map<char, int> trans;
    transitionTable[state] = trans;
    ntransitionTable[state] = trans;
    for(auto c: inputs)
    {
        transitionTable[state][(char)c] = -1;
        ntransitionTable[state][(char)c] = -1;
    }
}

void DFA::addTransition(int s1, int s2, char input)
{
    if(transitionTable.find(s1) == transitionTable.end())
    {
        initTransitionTableEntry(s1);
    }
    if(transitionTable.find(s2) == transitionTable.end())
    {
        initTransitionTableEntry(s2);
    }
    transitionTable[s1][input] = s2;
    ntransitionTable[s1][input] = s2;
}
void DFA::replaceTransition(int o1, int o2, int s1, int s2, char input)
{
    if(transitionTable.find(o1) != transitionTable.end() && !checkPass[o1])
    {
        ntransitionTable.erase(o1);
        //cout << "erase : " << o1 << "\n";
        checkPass[o1] = true;
    }
    if(transitionTable.find(o2) != transitionTable.end() && !checkPass[o2])
    {
        ntransitionTable.erase(o2);
        //cout << "erase : " << o2 << "\n";
        checkPass[o2] = true;
    }
    if(transitionTable.find(s1) != transitionTable.end() && !checkPass[s1])
    {
        ntransitionTable.erase(s1);
        //cout << "erase : " << s1 << "\n";
        checkPass[s1] = true;
    }
    if(transitionTable.find(s2) != transitionTable.end() && !checkPass[s2])
    {
        ntransitionTable.erase(s2);
        //cout << "erase : " << s2 << "\n";
        checkPass[s2] = true;
    }
    if(ntransitionTable.find(s1) == ntransitionTable.end())
    {
        unordered_map<char, int> trans;
        ntransitionTable[s1] = trans;
        for(auto c: inputs)
        {
            ntransitionTable[s1][(char)c] = -1;
        }
        //cout << "insert : " << s1 << "\n";
    }
    if(ntransitionTable.find(s2) == ntransitionTable.end())
    {
        unordered_map<char, int> trans;
        ntransitionTable[s2] = trans;
        for(auto c: inputs)
        {
            ntransitionTable[s2][(char)c] = -1;
        }
        //cout << "insert : " << s2 << "\n";

    }
    // cout <<"-------currentState = "<<s1 << "-->"   << s2<< "char : "<< input <<endl;
    ntransitionTable[s1][input] = s2;
}
void DFA::accept(int state, string className)
{
    if(transitionTable.find(state) != transitionTable.end())
    {
        acceptStates[state] = className;
    }
}
void DFA::newaccept(int state, string className)
{
    if(ntransitionTable.find(state) != ntransitionTable.end())
    {
        nacceptStates[state] = className;
    }
}
unordered_map<int, string> DFA::getAcceptStates()
{
    return acceptStates;
}
set<char> DFA::getInputSymbols()
{
    return inputs;
}
int DFA::getInitState()
{
    return initState;
}
int DFA::getNumStates()
{
    return transitionTable.size();
}
int DFA::getNextState(int state, char input)
{
    return transitionTable[state][input];
}
int DFA::getCurrentState()
{
    return currentState;
}
bool DFA::isAccept(int state)
{
    return (acceptStates.find(state) != acceptStates.end());
}
string DFA::getToken()
{
    return lastValidToken;
}
void DFA::reset()
{
    lastValidToken = "";
    inputSequence = "";
    lexeme = "";
    currentState = initState;
}

bool DFA::isDead()
{
    return currentState == -1;
}

int DFA::move(char input)
{
    if(inputs.find(input) == inputs.end())
    {
        inputSequence += input;
        currentState = -1;
        return -1;
    }
    currentState = ntransitionTable[currentState][input];
    inputSequence += input;
    //cout<< "currentState : "<< currentState <<"\n";
    if(isAccept(currentState))
    {
        lexeme = inputSequence;
        lastValidToken = acceptStates[currentState];
    }
    return currentState;
}
string DFA::getLexeme()
{
    return lexeme;
}
string DFA::getInputSequence()
{
    return inputSequence;
}
void DFA::printTransitionTable()
{
    ofstream myfile ("transition table.txt");
    //acceptStates = nacceptStates;
    //cout << "done1\n";
    if (myfile.is_open())
    {
        for (auto& state: ntransitionTable)
        {
            //cout << state.first << " : ";
            myfile << state.first << " : ";
            for (auto& ntransition: state.second)
            {
                //cout << ntransition.first << " -> " << ntransition.second << ", \n";
                myfile << ntransition.first << " -> " << ntransition.second << ",";
            }
            myfile << "\n";
        }
        myfile << "Accept states \n";
        for(auto& s: nacceptStates)
        {
            myfile << s.first << " : ";
            myfile << s.second;
            myfile << "\n";
        }
        myfile.close();
    }
    //cout << "done2\n";
    acceptStates = nacceptStates;
    /*for (auto& firstState: this->acceptStates)
    {
        cout << "state : " << firstState.first << "acc : " << firstState.second << "\n";
    }*/
}
