#include "../include/NDConverter.h"
#include <iostream>
#include <Vector>
#include <queue>
#include<bits/stdc++.h>
NDConverter::NDConverter(NFA* nfa)
{
    //ctor
    this->nfa = nfa;

}
void printTokens( unordered_map<int, pair<int, string>>tokens){
    cout<<"tokens--------------"<<endl;
   for(auto& s: tokens){
        cout << s.first << " : ";
        cout << "order " <<s.second.first;
        cout << "token "<<s.second.second;
        cout << endl;
      }
}
void printSymbols(set<int> inputSymbols){
    cout<< "accept----------------"<<endl;
    set<int> :: iterator it = inputSymbols.begin();
    while (it != inputSymbols.end())
        {
            cout<< *it<<" ";
            it++;
        }
}
void NDConverter::convert()
{   //printTokens(nfa->getAcceptedTokens());
    acceptStates = nfa->getAcceptStates();
    //printSymbols(acceptStates);
    inputSymbols = nfa->getSymbols();
    //printSymbols(inputSymbols);-----------------------------------------
    int TNum = 0;
    set<int> T = nfa->epsloneClosure(nfa->getStartState());
   /* cout<< "initial:";
            for(int c : T)
                cout<< c << " ";
            cout<<endl;*/
    vector< set<int> > Dstates;
    set<char> :: iterator it = inputSymbols.begin();
    queue< set<int> > unmarkedStates;
    Dstates.push_back(T);
    unmarkedStates.push(T);
    dfa = new DFA(inputSymbols, TNum);
    checkAccept(T, TNum);
    // subset construction algorithm
    while(!unmarkedStates.empty()){
            // mark state T by removing it from unmarked states
            T = unmarkedStates.front();
            unmarkedStates.pop();
            // find its number in the dfa states
            vector< set<int> >::iterator itr = find(Dstates.begin(), Dstates.end(), T);
            TNum = distance(Dstates.begin(), itr);
           /*cout<< "unmark:";
            for(int c : T)
                cout<< c << " ";
            cout<< "->" << TNum;
            cout<<endl;*/
            it = inputSymbols.begin();

        // Iterate over the input symbols
        while (it != inputSymbols.end())
        {
            if(*it != '\0'){
            /*cout<< *it <<" inputMove:";
            for(int c : nfa->inputMove(T,*it))
                cout<< c << " ";
            cout<<endl;*/
            set<int> U = nfa->epsloneClosure(nfa->inputMove(T,*it));
            /*cout<<"closure:-------------------------------------------"<<endl;
            printSymbols(U);*/
            if(!U.empty()){
            vector< set<int> >::iterator i = Dstates.begin();
            while (i != Dstates.end())
            {
                if(U == *i){
                //found -> add transition from T to U under char *it
                int UNum = distance(Dstates.begin(), i);
               // cout<<"found: "<< UNum<<"  "<<*it<<endl;
                dfa->addTransition(TNum, UNum, *it);
                break;
                }
                i++;
            }
            if(i == Dstates.end()){
                //not found -> add U as a new state to Dstates & add transition from T to U under char *it
                Dstates.push_back(U);
                unmarkedStates.push(U);
                int UNum = Dstates.size()- 1;
                dfa->addTransition(TNum, UNum, *it);
                checkAccept(U, UNum);
                   /* cout<< "add:";
                    for(int c : U)
                        cout<< c << " ";
                    cout<< "->" << UNum;
                    cout<<endl;*/
            }
            }
            }
         it++;
        }
    }
   // cout << "converter finish----------------------------------------";
}
DFA* NDConverter::getDFA()
{
    return dfa;
}
void NDConverter::checkAccept(set<int> U, int num){
    int order = -1;
    string token;
    set<int>::iterator  itr1 = acceptStates.begin();
        while (itr1 != acceptStates.end())
        {
            set<int>::iterator itr2 = U.find(*itr1);
            if (itr2 != U.end()){
                if(nfa->getAcceptedTokens()[*itr2].first > order){
                    order = nfa->getAcceptedTokens()[*itr2].first;
                    token = nfa->getAcceptedTokens()[*itr2].second;
                //cout << "order:" << order << " ,token:"<<token<<endl;
                }
            }
            itr1++;
        }
        if(order != -1){
            dfa->accept(num,token);
        }
}

