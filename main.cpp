#include <iostream>
#include "include/Parser.h"
#include "include/NDConverter.h"
#include "include/DFAMinimzer.h"
#include "include/DFADriver.h"
#include <set>
#include <vector>
#include <map>
#include <string>
#include "include/GrammerParser.h"
#include "include/GrammerOptimizer.h"
#include "include/ParserGenerator.h"
#include "include/Derivation.h"

using namespace std;

int main()
{
    //phase_1
	string rulesPath = "rules1.txt";
	string programPath = "program.txt";
	Parser p(rulesPath);
	p.parse();
	NFA* nfaCombined = p.getCombinedNFA();
	int startState = nfaCombined->getStartState();
    set<int> acceptedStates = nfaCombined->getAcceptStates();
    set<char> symbols = nfaCombined->getSymbols();
    set<int> states = nfaCombined->getStates();
    vector<map<char,set<int>>> transitionTable = nfaCombined->getTransitionTable();
    string tokenName = nfaCombined->getName();
    unordered_map<int, pair<int, string>> acceptedOfAllNFA = nfaCombined->getAcceptedTokens();
	NDConverter converter(nfaCombined);
	converter.convert();
	DFA* dfaConverted = converter.getDFA();
	DFAMinimzer minimizer(dfaConverted);
	minimizer.minimize();
    DFA* dfaMinimized = minimizer.getMinimizedDFA();
	dfaMinimized->printTransitionTable();
    //phase_2
    GrammerParser gp("rules2.txt");
    gp.parseRules();
    GrammerOptimizer go(gp.getRules());
    go.RemoveLeftRecusion();
    go.leftFactorisation();
    vector<Rule*> rules = go.getRules();
    for(int i = 0; i < rules.size(); i++){
        cout << i << ": " << rules[i]->getName() << " :" << endl;
        vector<vector<Rule*>> prod = rules[i]->getProductions();
        for(int j = 0; j < prod.size(); j++){
            cout << "production " << j << " :";
            for(int k = 0; k < prod[j].size(); k++){
                cout << " " <<prod[j][k]->getName();
            }
            cout << endl;
        }
    }
    cout<<"First"<<endl;
    ParserGenerator pg(go.getRules());
    pg.generateParsingTable();
    for(auto g : rules){
            cout << "-- "<<g->getName() << " :" << endl;
            int i = 0;
        for(auto f : g->getFirst()){
            cout<< "p"<<i++<<": ";
            for(auto t : f){
             cout << t << " ";
            }
            cout << " ";
        }
    cout << endl;
    }
    cout<<"FOLLOW"<<endl;
    for(Rule* g : rules){
        cout<<"Follow("<<g->getName()<<") : "<<endl;
        for(auto f : g->getFollow()){
            cout<<" "<<f;
        }
        cout<<endl;
    }
    pg.printParsingTable("parsingTable.txt");

    //last_step_together
    DFADriver driver(dfaMinimized);
	driver.setInputFile(programPath);

	Derivation* der;
    der = Derivation::GetInstance();
    der->setParsingTable(pg.getParsingTable());
    der->setStart(go.getRules()[0]);

	driver.start();
	driver.produceOutputFile();
    return 0;
}
