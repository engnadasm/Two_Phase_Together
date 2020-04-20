#include "../include/NFA.h"
#include <iostream>
NFA::NFA() {
	this->startState = 0;
}

void NFA::dashOperator(char a, char b) {
	map<char, set<int>> stateOne;
	map<char, set<int>> stateTwo;
	for (char i = a; i <= b; i++) {
		this->symbols.insert(i);
		stateOne.insert( { i, { 1 } });
	}
	this->transitionTable.push_back(stateOne);
	this->transitionTable.push_back(stateTwo);
	this->startState = 0;
	this->acceptedStates.insert(1);
	this->states.insert( { 0, 1 });
}
void NFA::orOperator(char a, char b) {
	for (int i = 0; i < 6; i++) {
		this->states.insert(i);
	}
	this->symbols.insert( { '\0', a, b });
	map<char, set<int>> states[6];
	states[0].insert( { '\0', { 1, 3 } });
	states[1].insert( { a, { 2 } });
	states[3].insert( { b, { 4 } });
	states[2].insert( { '\0', { 5 } });
	states[4].insert( { '\0', { 5 } });
	for (int i = 0; i < 6; i++) {
		this->transitionTable.push_back(states[i]);
	}
	this->acceptedStates.insert(5);
	this->startState = 0;
}
void NFA::orOperator(NFA nfa, char c) {
	NFA* pnfa;
	pnfa = &nfa;
	nfa.updateStates(pnfa, 1);
	this->startState = 0;
	map<char, set<int>> stateOne;
	stateOne.insert(
			{ '\0', { nfa.getStartState(), nfa.getStates().size() + 1 } });
	this->transitionTable.push_back(stateOne);
	for (map<char, set<int>> i : nfa.getTransitionTable()) {
		this->transitionTable.push_back(i);
	}
	map<char, set<int>> stateTwo;
	map<char, set<int>> stateThree;
	map<char, set<int>> stateFour;
	stateTwo.insert( { c, { nfa.getStates().size() + 2 } });
	stateThree.insert( { '\0', { nfa.getStates().size() + 3 } });
	this->transitionTable.push_back(stateTwo);
	this->transitionTable.push_back(stateThree);
	this->transitionTable.push_back(stateFour);
	set<int> indexOfAccepted = nfa.getAcceptStates();
	for (int i : indexOfAccepted) {
		map<char, set<int>> temp = this->transitionTable.at(i);
		if (temp.find('\0') == temp.end()) {
			temp.insert( { '\0', { this->transitionTable.size() - 1 } });
		} else {
			temp.find('\0')->second.insert(this->transitionTable.size() - 1);
		}
		this->transitionTable.at(i) = temp;
	}
	this->acceptedStates.insert(this->transitionTable.size() - 1);
	this->symbols = nfa.getSymbols();
	this->symbols.insert(c);
	this->symbols.insert('\0');
	for (int i = 0; i < this->transitionTable.size(); i++) {
		this->states.insert(i);
	}
}
void NFA::orOperator(NFA nfa1, NFA nfa2) {
	NFA* pnfa1 = &nfa1;
	NFA* pnfa2 = &nfa2;
	nfa1.updateStates(pnfa1, 1);
	nfa2.updateStates(pnfa2, nfa1.getStates().size() + 1);
	map<char, set<int>> initialState;
	map<char, set<int>> lastState;
	initialState.insert(
			{ '\0', { nfa1.getStartState(), nfa2.getStartState() } });
	this->transitionTable.push_back(initialState);
	for (map<char, set<int>> m : nfa1.getTransitionTable()) {
		this->transitionTable.push_back(m);
	}
	for (map<char, set<int>> m : nfa2.getTransitionTable()) {
		this->transitionTable.push_back(m);
	}
	this->transitionTable.push_back(lastState);
	set<int> indexOfAccepted = nfa1.getAcceptStates();
	for (int i : indexOfAccepted) {
		map<char, set<int>> temp = this->transitionTable.at(i);
		if (temp.find('\0') == temp.end()) {
			temp.insert( { '\0', { this->transitionTable.size() - 1 } });
			this->symbols.insert('\0');
		} else {
			temp.find('\0')->second.insert(this->transitionTable.size() - 1);
		}
		this->transitionTable.at(i) = temp;
	}
	indexOfAccepted = nfa2.getAcceptStates();
	for (int i : indexOfAccepted) {
		map<char, set<int>> temp = this->transitionTable.at(i);
		if (temp.find('\0') == temp.end()) {
			temp.insert( { '\0', { this->transitionTable.size() - 1 } });
			this->symbols.insert('\0');
		} else {
			temp.find('\0')->second.insert(this->transitionTable.size() - 1);
		}
		this->transitionTable.at(i) = temp;
	}
	this->acceptedStates.insert(this->transitionTable.size() - 1);
	this->startState = 0;
	for (int i = 0; i < transitionTable.size(); i++) {
		this->states.insert(i);
	}
	this->symbols = nfa1.getSymbols();
	this->symbols.insert('\0');
	for (char c : nfa2.getSymbols()) {
		this->symbols.insert(c);
	}
}
void NFA::concatenateOperator(char a, char b) {
	map<char, set<int>> states[3];
	states[0].insert({a, { 1 }});
	states[1].insert({b, { 2 }});
	this->symbols.insert( { a, b });
	for (int i = 0; i < 3; i++)
		this->transitionTable.push_back(states[i]);
	this->startState = 0;
	this->acceptedStates.insert(2);
	this->states.insert( { 0, 1, 2 });
}
void NFA::concatenateOperator(NFA nfa, char c) {
	set<int> indexOfStates = nfa.getStates();
	set<char> inputChars = nfa.getSymbols();
	vector<map<char, set<int>>> transition = nfa.getTransitionTable();
	set<int> indexOfLastStates = nfa.getAcceptStates();
	map<char, set<int>> newState;
	transition.push_back(newState);
	for (int i : indexOfLastStates) {
		map<char, set<int>> temp = transition.at(i);
		if (temp.find(c) == temp.end()) {
			temp.insert( { c, { transition.size() - 1 } });
			inputChars.insert(c);
		} else {
			temp.find(c)->second.insert(transition.size() - 1);
		}
		transition.at(i) = temp;
	}
	indexOfLastStates.clear();
	indexOfLastStates.insert(transition.size() - 1);
	indexOfStates.insert(transition.size() - 1);
	this->acceptedStates = indexOfLastStates;
	this->startState = nfa.getStartState();
	this->states = indexOfStates;
	this->symbols = inputChars;
	this->transitionTable = transition;
}
void NFA::concatenateOperator(NFA nfa1, NFA nfa2) {
	int num = nfa1.getStates().size() - 1;
	NFA* pnfa2 = &nfa2;
	nfa2.updateStates(pnfa2, num);
	this->acceptedStates = nfa2.getAcceptStates();
	this->startState = nfa1.getStartState();
	this->symbols = nfa1.getSymbols();
	for (char c : nfa2.getSymbols())
		this->symbols.insert(c);
	this->transitionTable = nfa1.getTransitionTable();
	this->transitionTable.at(num) = nfa2.getTransitionTable().at(0);
	for (int i = 1; i < nfa2.getStates().size(); i++) {
		this->transitionTable.push_back(nfa2.getTransitionTable().at(i));
	}
	for(int i = 0; i < this->transitionTable.size(); i++){
        this->states.insert(i);
	}
}
void NFA::plusOperator(char c) {
	this->starOperator(c);
	set<int>::iterator itBegin =
			this->transitionTable.at(0).find('\0')->second.begin();
	set<int>::iterator itEnd =
			this->transitionTable.at(0).find('\0')->second.begin();
	for (int i : this->transitionTable.at(0).find('\0')->second) {
		if (i == this->transitionTable.size() - 1) {
			break;
		} else {
			itBegin++;
			itEnd++;
		}
	}
	itEnd++;
	this->transitionTable.at(0).find('\0')->second.erase(itBegin, itEnd);
}
void NFA::plusOperator(NFA nfa) {
	this->starOperator(nfa);
	set<int>::iterator itBegin =
			this->transitionTable.at(0).find('\0')->second.begin();
	set<int>::iterator itEnd =
			this->transitionTable.at(0).find('\0')->second.begin();
	for (int i : this->transitionTable.at(0).find('\0')->second) {
		if (i == this->transitionTable.size() - 1) {
			break;
		} else {
			itBegin++;
			itEnd++;
		}
	}
	itEnd++;
	this->transitionTable.at(0).find('\0')->second.erase(itBegin, itEnd);
}
void NFA::starOperator(char c) {
	map<char, set<int>> states[4];
	states[0].insert( { '\0', { 1, 3 } });
	states[1].insert( { c, { 2 } });
	states[2].insert( { '\0', { 1, 3 } });
	this->acceptedStates.insert(3);
	this->startState = 0;
	this->states.insert( { 0, 1, 2, 3 });
	this->symbols.insert(c);
	for (int i = 0; i < 4; i++) {
		this->transitionTable.push_back(states[i]);
	}
}
void NFA::starOperator(NFA nfa) {
	NFA* pnfa = &nfa;
	nfa.updateStates(pnfa, 1);
	map<char, set<int>> states[2];
	states[0].insert( { '\0', { 1, nfa.getStates().size() + 1 } });
	this->transitionTable.push_back(states[0]);
	for (map<char, set<int>> i : nfa.getTransitionTable()) {
		this->transitionTable.push_back(i);
	}
	this->transitionTable.at(this->transitionTable.size() - 1).insert( { '\0', { nfa.getStartState(), nfa.getStates().size() + 1 } });
	this->transitionTable.push_back(states[1]);
	this->acceptedStates.insert(this->transitionTable.size() - 1);
	this->startState = 0;
	for (int i = 0; i < this->transitionTable.size(); i++) {
		this->states.insert(i);
	}
	this->symbols = nfa.getSymbols();
	this->symbols.insert('\0');
}
void NFA::keyword(string word) {
	map<char, set<int>> wordStates[word.size() + 1];
	for (int i = 0; i < word.size(); i++) {
		wordStates[i].insert( { word.at(i), { i + 1 } });
		this->symbols.insert(word.at(i));
	}
	for(int i = 0; i< word.size() + 1; i++){
        this->transitionTable.push_back(wordStates[i]);
        this->states.insert(i);
	}
	this->acceptedStates.insert(word.size());
	this->startState = 0;
}
void NFA::keyword(char word[], int len) {
	map<char, set<int>> wordStates[len + 1];
	for (int i = 0; i < len; i++) {
		wordStates[i].insert( { word[i], { i + 1 } });
		this->symbols.insert(word[i]);
	}
	for(int i = 0; i< len + 1; i++){
        this->transitionTable.push_back(wordStates[i]);
        this->states.insert(i);
	}
	this->acceptedStates.insert(len);
	this->startState = 0;
}
void NFA::combine(NFA nfa[], int n) {
	map<char, set<int>> stateZero;
	int num = 1;
	stateZero.insert( { '\0', { 1 } });
	this->transitionTable.push_back(stateZero);
	this->states.insert(0);
	for (int i = 0; i < n; i++) {
		if(i == 0)
            num = 1;
        else
            num = num + nfa[i-1].getStates().size();
		NFA* pnfa = &nfa[i];
		nfa[i].updateStates(pnfa, num);
		for (int j = 0; j < nfa[i].getStates().size(); j++) {
			this->transitionTable.push_back(nfa[i].getTransitionTable().at(j));
		}
		for (int j : nfa[i].getAcceptStates()) {
            pair<int, string> order_name(nfa[i].getOrder(), nfa[i].getName());
			this->acceptedOfAllNFA.insert( { j, order_name });
			this->acceptedStates.insert(j);
		}
		for (char c : nfa[i].getSymbols()) {
			this->symbols.insert(c);
		}
		this->transitionTable.at(0).find('\0')->second.insert(
				nfa[i].getStartState());
		for (int j : nfa[i].getStates()) {
			this->states.insert(j);
		}
	}
}
void NFA::setName(string name) {
	this->tokenName = name;
}
set<int> NFA::epsloneClosure(int s) {
	map<char, set<int>> state = this->transitionTable.at(s);
	map<char, set<int>>::iterator it = state.find('\0');
	set<int> resultedStates;
	if (it == state.end()) {
        resultedStates.insert(s);
		return resultedStates;
	} else {
	    resultedStates.insert(s);
	    for(int i : it->second){
            resultedStates.insert(i);
            set<int> temp = epsloneClosure(i);
            for(int j : temp){
                resultedStates.insert(j);
            }
	    }
		return resultedStates;
	}
}
set<int> NFA::epsloneClosure(set<int> T) {
	set<int> resultedStates;
	for (int i : T) {
		set<int> temp = this->epsloneClosure(i);
        for (int j : temp) {
            resultedStates.insert(j);
        }
	}
	return resultedStates;
}
set<int> NFA::inputMove(set<int> T, char c) {
	set<int> resultedStates;
	for (int i : T) {
		map<char, set<int>> m = this->transitionTable.at(i);
		if (m.find(c) != m.end()) {
			for (int j : m.find(c)->second) {
				resultedStates.insert(j);
			}
		}
	}
	return resultedStates;
}
void NFA::updateStates(NFA* nfa, int num) {
	/*update the index of accepted states*/
	set<int> final = nfa->getAcceptStates();
	int n = final.size();
	int arrFinal[n];
	int j = 0;
	for (int i : final) {
		arrFinal[j] = i + num;
		j++;
	}
	final.clear();
	final.insert(arrFinal, arrFinal + n);
	nfa->setAcceptStates(final);
	/*update the index of start state*/
	nfa->setStartState(nfa->getStartState() + num);
	/*update the index of the states*/
	set<int> allStates = nfa->getStates();
	n = allStates.size();
	int arr[n];
	j = 0;
	for (int i : allStates) {
		arr[j] = i + num;
		j++;
	}
	allStates.clear();
	allStates.insert(arr, arr + n);
	nfa->setStates(allStates);
	/*update the transition states*/
	vector<map<char, set<int>>> transition = nfa->getTransitionTable();
	for (int k = 0; k < transition.size(); k++) {
		map<char, set<int>> m = transition.at(k);
		for (map<char, set<int>>::iterator it = m.begin(); it != m.end();
				it++) {
			set<int> temp = it->second;
			n = temp.size();
			int arrTemp[n];
			j = 0;
			for (int i : temp) {
				arrTemp[j] = i + num;
				j++;
			}
			temp.clear();
			temp.insert(arrTemp, arrTemp + n);
			it->second = temp;
		}
		transition.at(k) = m;
	}
	nfa->setTransitionTable(transition);
}
set<char> NFA::getSymbols() {
	return this->symbols;
}
set<int> NFA::getStates() {
	return this->states;
}
int NFA::getStartState() {
	return this->startState;
}
set<int> NFA::getAcceptStates() {
	return this->acceptedStates;
}
vector<map<char, set<int>>> NFA::getTransitionTable()
{
	return this->transitionTable;
}
string NFA::getName() {
	return this->tokenName;
}
unordered_map<int, pair<int, string>> NFA::getAcceptedTokens() {
	return this->acceptedOfAllNFA;
}
void NFA::setSymbols(set<char> symb) {
	this->symbols = symb;
}
void NFA::setStates(set<int> updateStates) {
	this->states = updateStates;
}
void NFA::setStartState(int start) {
	this->startState = start;
}
void NFA::setAcceptStates(set<int> finalStates) {
	this->acceptedStates = finalStates;
}
void NFA::setTransitionTable(vector<map<char, set<int>>> transition) {
	this->transitionTable = transition;
}

void NFA::setOrder(int order){
    this->order = order;
}

int NFA::getOrder(){
    return order;
}
