#include "../include/DFAMinimzer.h"
#include "../include/DFA.h"
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>

DFAMinimzer::DFAMinimzer(DFA* dfa)
{
    this->dfa = dfa;
    this->numStates = dfa->getNumStates();
}
void DFAMinimzer::minimize()
{
    this->inputs = dfa->getInputSymbols();
    this->initState = dfa->getInitState();
    this->numStates = dfa->getNumStates();
    this->acceptStates = dfa->getAcceptStates();
    unordered_map<int, string> nonAcceptStates;
    for (int i =0 ; i< this->numStates; i++)
    {
        if (!this->dfa->isAccept(i))
        {
            nonAcceptStates.insert(pair<int,string>(i, ""));
            groupStates.insert(pair<int,int>(i,0));
        }
        else
        {
            groupStates.insert(pair<int,int>(i, 1));
        }
    }
    this->partitions.push_back(acceptStates);
    this->partitions.push_back(nonAcceptStates);

    this->flag = true;

    while(flag)
    {
        this->flag = false;
        int partitionCount = partitions.size();
        for(int i = 0 ; i < partitionCount; i++)
        {
            this->partitions = doPartition();
        }
    }
    oneStateOfEachGroup();

}

vector<unordered_map<int, string>> DFAMinimzer::doPartition()
{
    int j = 0;
    for (auto& firstState: this->partitions[0])
    {
        unordered_map<int, string> currentPartition;
        currentPartition.insert(pair<int,string>(firstState.first,firstState.second));
        //for(int k = j+1 ; k < partitions[i].size() ; k++)
        int k = 0;
        for(unordered_map<int, string>::iterator secondState = this->partitions[0].begin();
                secondState != partitions[0].end(); ++secondState)
        {
            if(k < (j + 1) )
            {
                k++;
                continue;
            }
            else
            {
                bool check = true;
                for(char charInput : this->inputs)
                {
                    int nextFirstState = this->dfa->getNextState(firstState.first,charInput);
                    int nextSecondState = this->dfa->getNextState(secondState->first,charInput);
                    if ((nextFirstState != nextSecondState))
                    {
                        check =false;
                        break;
                    }
                    else if ((nextFirstState == nextSecondState)&& dfa->isAccept(firstState.first)
                             && dfa->isAccept(secondState->first)
                            )
                    {
                        if(acceptStates.at(firstState.first)!= acceptStates.at(secondState->first))
                        {
                            check =false;
                            break;
                        }
                    }

                }
                if (check)
                {
                    currentPartition.insert(pair<int,string>(secondState->first,secondState->second));
                    this->partitions[0].erase( secondState->first);
                    k--;
                }
                else
                {
                    this->flag = true;
                }
                k++;
            }
        }
        if(partitions[0].size() != 1)
        {
            for (auto& state: currentPartition)
            {
                groupStates[state.first] = this->counter;
            }
        }
        this->partitions.push_back(currentPartition);
        this->partitions[0].erase(firstState.first);
        this->counter++;
    }
    if(flag)
    {
        this->partitions.erase(partitions.begin());
    }

    return this->partitions;
}

DFA* DFAMinimzer::getMinimizedDFA()
{
    return this->dfa;
}

void DFAMinimzer::oneStateOfEachGroup()
{
    unordered_map<int, string> mainStates;
    unordered_map<int, string> currentPartition;

    int currentState = 0;//initState
    int nextState ;

    for(int i = 0 ; i < this->numStates; i++)
    {
        currentState = i;
        for(auto& charInput : this->inputs)
        {
            nextState = this->dfa->getNextState(currentState, charInput); // return -1 if dead state
            if(nextState != -1)
            {
                this->dfa->replaceTransition(currentState,nextState,this->groupStates[currentState],
                                             this->groupStates[nextState], charInput);
            }
        }
        if(dfa->isAccept(currentState))
        {
            this->dfa->newaccept(this->groupStates[currentState], acceptStates.at(currentState));
        }
    }
}
