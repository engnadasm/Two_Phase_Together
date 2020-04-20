#include "../include/GrammerOptimizer.h"

GrammerOptimizer::GrammerOptimizer(vector<Rule*> grammer){
    this->grammer = grammer;
}

void GrammerOptimizer::RemoveLeftRecusion(){
    int len = grammer.size();
    for(int i = 0; i < len; i++){
        for(int j = 0; j < i; j++){
            vector<vector<Rule*>> p = grammer[i]->getProductions();
            grammer[i]->clearProductions();
            for(auto v: p){
                if(v[0] == grammer[j]){
                    vector<vector<Rule*>> alphas = grammer[j]->getProductions();
                    vector<Rule*> n = v;
                    n.erase(n.begin());
                    for(auto alpha: alphas){
                        vector<Rule*> x = alpha;
                        x.insert(x.end(), n.begin(), n.end());
                        grammer[i]->addProduction(x);
                    }
                } else {
                    grammer[i]->addProduction(v);
                }
            }
        }
        eliminateLeftImmidiateRecursion(grammer[i]);
    }
}
void GrammerOptimizer::leftFactorisation(){
    int len = grammer.size();
    for(int i = 0; i < len; i++){
        factorizeRule(grammer[i]);
    }
    removeEmpty();
}

void GrammerOptimizer::factorizeRule(Rule* r){
    if(isEpsilon(r)) return;
    vector<vector<Rule*>> p = r->getProductions();
    vector<bool> removed;
    removed.resize(p.size(), false);
    r->clearProductions();
    int counter = 1;
    for(int i = 0; i < p.size(); i++){
        if(removed[i]) continue;
        if(isEpsilon(p[i])){
            vector<Rule*> e;
            Rule* ee = new Rule("\\L");
            ee->setTerminal();
            e.push_back(ee);
            r->addProduction(e);
            continue;
        }
        vector<Rule*> n;
        n.push_back(p[i][0]);
        Rule* B = new Rule(r->getName() + "_" + to_string(counter));
        for(int j = 0; j < p.size(); j++){
            if(removed[j]) continue;
            if(p[i][0] != p[j][0]) continue;
            vector<Rule*> x(p[j].begin()+1, p[j].end());
            if(x.empty()){
                Rule* eps = new Rule("\\L");
                eps->setTerminal();
                x.push_back(eps);
            }
            B->addProduction(x);
            removed[j] = true;
        }
        n.push_back(B);
        factorizeRule(B);
        r->addProduction(n);
        grammer.push_back(B);
        counter++;
    }
}

void GrammerOptimizer::removeEmpty(){
    auto it = grammer.begin();
    while(it != grammer.end()){
        if(isEpsilon(*it)){
            auto it2 = grammer.begin();
            while(it2 != grammer.end()){
                (*it2)->removeProduction((*it)->getName());
                ++it2;
            }
            it = grammer.erase(it);
        } else {
            (*it)->removeRedundantNulls();
            ++it;
        }

    }
}
//{(abd), (a), (bda), (bdd), (da)}
vector<Rule*> GrammerOptimizer::getRules(){
    return this->grammer;
}

void GrammerOptimizer::eliminateLeftImmidiateRecursion(Rule* r){
    if(!hasImmidiateLeftRecursion(r)){
        return;
    }
    Rule* B = new Rule(r->getName() + "_dash");
    vector<vector<Rule*>> p = r->getProductions();
    r->clearProductions();
    for(auto v: p){
        if(v[0] == r){
            vector<Rule*> n = v;
            n.erase(n.begin());
            n.push_back(B);
            B->addProduction(n);
        } else {
            vector<Rule*> n = v;
            n.push_back(B);
            r->addProduction(n);
        }
    }
    Rule* eps = new Rule("\\L");
    eps->setTerminal();
    vector<Rule*> e;
    e.push_back(eps);
    B->addProduction(e);
    grammer.push_back(B);
}

bool GrammerOptimizer::hasImmidiateLeftRecursion(Rule* r){
    for(auto v: r->getProductions()){
        if(v[0] == r) return true;
    }
    return false;
}
