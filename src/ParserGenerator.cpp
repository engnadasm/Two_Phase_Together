#include "../include/ParserGenerator.h"
#include <fstream>
#include <iostream>
ParserGenerator::ParserGenerator(vector<Rule*> grammer)
{
    //ctor
    this->grammer = grammer;
}

void ParserGenerator::generateParsingTable()
{//1-compute first of all Rules
    //for each nonTerminal
    for (auto lhs : grammer)
    {  //compute lhs if not computed
       if(!lhs->firstComputed()){
        //cout <<"compute :" <<lhs->getName()<<"----------------------------" << endl;
            calcFirst(lhs);
       }
       if(!lhs->followComputed()){
            calcFollow(lhs);
       }
    }
    //2-compute follow of all nonTerminals
    for (auto lhs : grammer)
    {
       if(!lhs->followComputed()){
            calcFollow(lhs);
       }
    }
//3-construct parsing table
    addToTable();

}
void ParserGenerator::calcFirst(Rule* lhs){
        //mark lhs as computed
        //cout << "********************before mark " <<lhs->getName()<<" = "<< lhs->firstComputed()<<endl;
        lhs->markFirst();
        //cout << "********************mark " <<lhs->getName()<<" = "<< lhs->firstComputed()<<endl;
        //loop over its productions
        vector<vector<Rule*>>productions = lhs->getProductions();
        if(lhs->isTerminal()){
            //cout << lhs->getName() << "is terminal" <<endl;
                    set<string> s;
                    s.insert(lhs->getName());
                    lhs->addFirst(s);
                    if(lhs->getName()=="\\L"){
                        //cout << "is nullable" <<endl;
                       lhs->addNullable(true);
                    }else{
                       lhs->addNullable(false);
            }
        }
        else{for(auto p: productions){
             if(p[0]->isTerminal()){
                 //cout << p[0]->getName() << "is terminal" <<endl;
                    set<string> s;
                    s.insert(p[0]->getName());
                    lhs->addFirst(s);
                    if(p[0]->getName()=="\\L"){
                        //cout << "is nullable" <<endl;
                       lhs->addNullable(true);
                    }else{
                       lhs->addNullable(false);
                    }
             }else{// p[0] is nonTerminal
                eps = true;
                unsigned int i = 0;
                set<string> f1;
                //cout << p[i]->getName() << "is non terminal" <<endl;
                while(eps && i < p.size()){
                    if(!p[i]->firstComputed()){
                      //cout << "*compute "<<p[i]->getName()<< "----------------------------"<<endl;
                        calcFirst(p[i]);
                    }
                    //convert first from vector to set<string> and check if it is nullable
                    //cout << "finish" <<p[i]->getName()<< endl;
                    set<string> f2 = checkEps(p[i]->getFirst(), (i+1 == p.size()));

                    //add to f1
                    f1.insert(f2.begin(),f2.end());

                    i++;
                    //cout << "i = " <<i<< endl;
                }
                lhs->addNullable(eps);
                lhs->addFirst(f1);
            }
         }
        }
}
set<string> ParserGenerator::checkEps(vector<set<string>> f , bool lastSymbol){
    //cout << "check eps, last symbol = " << lastSymbol << endl;
    eps = false;
    set<string> first;
    for (auto s : f){
        //1- remove epsilon unless it comes from the last production
        set<string>::iterator it = s.find("\\L");
        if (it != s.end()) {
            //cout << "eps found"<<endl;
            eps = true;
            if(!lastSymbol){
             s.erase(*it);
            }
        }
        //2- add s to first
        first.insert(s.begin(), s.end());
    }

    return first;
}
void ParserGenerator::calcFollow(Rule* lhs)
{
    if(lhs == this->grammer.at(0)){
        lhs->addFollow("$");
    }
    for(int i = 0; i < this->grammer.size(); i++){
        vector<vector<Rule*>> productions = this->grammer.at(i)->getProductions();
        for(int j = 0; j < productions.size(); j++){
            vector<Rule*> prod = productions.at(j);
            std::vector<Rule*>::iterator it = find(prod.begin(), prod.end(), lhs);
            if(it != prod.end()){
                if(distance(prod.begin(), it) == prod.size() - 1){
                    if(!this->grammer.at(i)->followComputed() && this->grammer.at(i)->checkVisited() < this->grammer.size()){
                        this->grammer.at(i)->markVisited();
                        calcFollow(this->grammer.at(i));
                    }
                    lhs->addFollow(this->grammer.at(i)->getFollow());
                }else{
                    Rule* r = prod.at(distance(prod.begin(), it) + 1);
                    if(r->isTerminal() && r->getName() != "\\L"){
                        lhs->addFollow(r->getName());
                    }else if(!r->isTerminal()){
                        if(!r->firstComputed())
                            calcFirst(r);
                        for(int k = 0; k < r->getFirst().size(); k++){
                            lhs->addFollow(r->getFirst().at(k));
                        }
                        vector<bool> nullable = r->getNullable();
                        std::vector<bool>::iterator it = find(nullable.begin(), nullable.end(), true);
                        if(it != nullable.end()){
                            if(!this->grammer.at(i)->followComputed() && this->grammer.at(i)->checkVisited() < this->grammer.size()){
                                this->grammer.at(i)->markVisited();
                                calcFollow(this->grammer.at(i));
                            }
                            lhs->addFollow(this->grammer.at(i)->getFollow());
                        }
                    }
                }
            }
        }
    }
    lhs->markFollow();
}
void ParserGenerator::printParsingTable()
{

}
void ParserGenerator::printParsingTable(string outputFile)
{
    ofstream myfile (outputFile);
  if (myfile.is_open())
  {         int i = 1;
            for (auto& nonterminal : parsingTable)
            {
                myfile << i << "- " <<nonterminal.first;
                myfile << " :\n";
                i++;
                for(auto& terminal : nonterminal.second){
                    myfile << terminal.first << " :";
                    for(auto r : terminal.second){
                        myfile << " " << r->getName();
                    }
                    myfile << "  ";
                }
                myfile << "\n";
            }
    myfile.close();
  }
  else cout << "Unable to open file";

}
unordered_map<string,unordered_map<string, vector<Rule*>>>ParserGenerator::getParsingTable()
{
   return this->parsingTable;

}
void::ParserGenerator::addToTable(){
    for(auto g : grammer){
        vector<vector<Rule*>>productions = g->getProductions();
        vector<vector<Rule*>>::iterator pitr = productions.begin();
        vector<bool>nullable = g->getNullable();
        vector<bool>::iterator nitr = nullable.begin();
        Rule* synch = new Rule("synch");
        vector<Rule*> synchProd = {synch};
        //f = set<string>
        for(auto f : g->getFirst()){
            // t = string -> terminal name
            for(auto t : f){
                if(t != "\\L"){
                    parsingTable[g->getName()][t] = *pitr;
                }
            }
            if(*nitr == true){
                for(string s : g->getFollow()){
                    parsingTable[g->getName()][s] = *pitr;
                }
            }
            pitr++;
            nitr++;
        }
        unordered_map<string,unordered_map<string, vector<Rule*>>>::iterator it = parsingTable.find(g->getName());
        if(it != parsingTable.end()){
            for(string s : g->getFollow()){
                unordered_map<string, vector<Rule*>>::iterator it2 = it->second.find(s);
                if(it2 == it->second.end()){
                    parsingTable[g->getName()][s] = synchProd;
                }
            }
        }else{
            for(string s : g->getFollow()){
                parsingTable[g->getName()][s] = synchProd;
            }
        }
    }
}
