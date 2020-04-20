#ifndef NFA_H
#define NFA_H
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <utility>

using namespace std;

class NFA
{
    public:
        NFA(); //constructor initializations goes here
        /* NFA construction methods this methods will be used by the parser */
        void dashOperator(char a, char b); // a-b ex: 0-9 or a-z or A-Z would call dashOperator('a', 'z');
        void orOperator(char a, char b); // a | b ex: 0 | 1 would call orOperator('0', '1');
        void orOperator(NFA nfa, char c); // nfa | c ex: letter|0 would call orOperator(letter, '0');
        void orOperator(NFA nfa1, NFA nfa2); // nfa1 | nfa2 ex letter|digit would call orOperator(letter,digit);
        void concatenateOperator(char a, char b); // ab ex 00 or 11 would call concatenateOperator('0','1');
        void concatenateOperator(NFA nfa, char c);
        void concatenateOperator(NFA nfa1, NFA nfa2);
        void plusOperator(char c);
        void plusOperator(NFA nfa);
        void starOperator(char c);
        void starOperator(NFA nfa);
        void keyword(string word); // ex: if else etc.
        void keyword(char word[], int len); // same as above for flexibility
        void combine(NFA nfa[], int n); // combines n nfa's into one nfa keeping all the accept states.
        void setName(string name);
        void setOrder(int order);
        int getOrder();

        /* operations on NFA states */
        set<int> epsloneClosure(int s);
        set<int> epsloneClosure(set<int> T);
        set<int> inputMove(set<int> T, char c);
        void updateStates(NFA* nfa, int num);

        /* NFA Definition functions */

        set<char> getSymbols();
        set<int> getStates();
        int getStartState();
        set<int> getAcceptStates();
        vector<map<char,set<int>>> getTransitionTable();
        string getName();
        unordered_map<int, pair<int, string>> getAcceptedTokens();

        void setSymbols(set<char> symb);
		void setStates(set<int> updateStates);
		void setStartState(int start);
		void setAcceptStates(set<int> finalStates);
		void setTransitionTable(vector<map<char,set<int>>> transition);

    protected:

    private:
        int startState;
        set<int> acceptedStates;
        set<char> symbols;
        set<int> states;
        vector<map<char,set<int>>> transitionTable;
        string tokenName;
        int order;
        unordered_map<int, pair<int, string>> acceptedOfAllNFA;
};

#endif // NFA_H
