#include "../include/Rule.h"

Rule::Rule(string name)
{
    this->name = name;
    this->terminal = false;
    this->visited = 0;
}

void Rule::clearProductions(){
    this->productions.clear();
}

void Rule::removeProduction(string name){
    for(int i = 0; i < productions.size(); i++){
        for(int j = 0; j < productions[i].size(); j++){
            if(productions[i][j]->getName() == name){
                productions[i].erase(productions[i].begin()+j);
                if(productions[i].empty()){
                    Rule* eps = new Rule("\\L");
                    eps->setTerminal();
                    productions[i].push_back(eps);
                }
            }
        }
    }
}

void Rule::removeRedundantNulls(){
    auto it1 = productions.begin();
    while(it1 != productions.end()){
        if((*it1).size() == 1){
            it1++;
            continue;
        }
        auto it2 = (*it1).begin();
        while(it2 != (*it1).end()){
            if((*it2)->getName() == "\\L"){
                it2 = (*it1).erase(it2);
            } else {
                it2++;
            }
        }
        it1++;
    }
}
bool Rule::isTerminal(){
    return this->terminal;
}
vector<bool> Rule::getNullable(){
    return this->nullable;
}
void Rule::addNullable(bool b){
    nullable.push_back(b);
}
vector<set<string>>Rule::getFirst(){
    return this->first;
}
set<string> Rule::getFollow(){
    return this->follow;
}
string Rule::getName(){
    return this->name;
}
void Rule::addProduction(vector<Rule*> r){
    /*cout << "adding production in rule "<<name <<":" ;
    for(auto x: r){
        cout << " " << x->getName();
    }
    cout << endl;*/


    this->productions.push_back(r);
    //cout << "productions so far" << endl;
    //printProductions(productions);
}

void Rule::setTerminal(){
    terminal = true;
}

vector<vector<Rule*>> Rule::getProductions(){
    return productions;
}
void Rule::addFollow(string f){
    this->follow.insert(f);
}
void Rule::addFirst(set<string> f){
    first.push_back(f);
}
void Rule::addFollow(set<string> f){
    for(string s: f){
        if(s != "\\L")
            follow.insert(s);
    }
}
bool Rule::firstComputed(){
    return calcFirst;
}
void Rule::markFirst(){
    calcFirst = true;
}
bool Rule::followComputed(){
    return calcFollow;
}
void Rule::markFollow(){
    calcFollow = true;
}
int Rule::checkVisited(){
    return visited;
}
void Rule::markVisited(){
    visited++;
}
