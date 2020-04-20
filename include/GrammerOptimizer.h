#ifndef GRAMMEROPTIMIZER_H
#define GRAMMEROPTIMIZER_H
#include <vector>
#include <string>
#include <iostream>
#include "Rule.h"

using namespace std;


class GrammerOptimizer
{
    public:
        GrammerOptimizer(vector<Rule*> grammer);
        void RemoveLeftRecusion();
        void leftFactorisation();
        vector<Rule*> getRules();

    protected:

    private:
        vector<Rule*> grammer;
        void eliminateLeftImmidiateRecursion(Rule* r);
        bool hasImmidiateLeftRecursion(Rule* r);
        void factorizeRule(Rule* r);
        void removeEmpty();
};


static inline bool isEpsilon(Rule* r){
    if(r->getProductions().size() == 1){
        if(r->getProductions()[0].size() == 1){
            if(r->getProductions()[0][0]->getName() == "\\L"){
                return true;
            }
        }
    }
    return false;
}
static inline bool isEpsilon(vector<Rule*> r){
    if(r.size() == 1){
        if(r[0]->getName() == "\\L"){
            return true;
        }
    }
    return false;
}
#endif // GRAMMEROPTIMIZER_H
