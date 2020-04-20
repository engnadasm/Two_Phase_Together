#include "../include/Parser.h"
#include <iostream>
Parser::Parser(string path)
{
    this->path = path;
    order = 0;
}
void Parser::parse(){
    string line;
    ifstream file(path);
    while(getline(file, line)){
        vector<string> segs = segmentation(line);
        //cout << "parsing line:  " << line << endl;
        parseSegs(segs);
        //cout << "finished-------------------------------" << endl;
    }
    file.close();
    NFA arr[exp.size()];
    std::copy(exp.begin(), exp.end(), arr);
    nfa = new NFA();
    nfa->combine(arr, exp.size());
}

NFA* Parser::getCombinedNFA(){
    return this->nfa;
}
vector<string> Parser::segmentation(string line){
    vector<string> segs;
    int i = 0;
    string seg;
    while(i < line.size()){
        if(getOperator(line[i])==0){
            seg.append(string(1,line[i]));
            if(line[i] == '\\'){
                seg += string(1,line[i+1]);
                i++;
            }


        } else if(getOperator(line[i]) == 13){
            if(seg.size() > 0){
                segs.push_back(seg);

                //it++;
                seg.clear();
            }
        } else {
            if(seg.size() > 0){
                segs.push_back(seg);
                //it++;
            }
            seg = string(1, line[i]);
            segs.push_back(seg);
            //it++;
            seg.clear();
        }
        i++;
    }
    if(seg.size() > 0){
        segs.push_back(seg);
    }
    return segs;
}


void Parser::parseSegs(vector<string> segs){
    if(segs[0] == "{"){
        segs.erase(segs.begin());
        segs.erase(segs.end()-1);
        createKeywords(segs);
    } else if(segs[0] == "["){
        segs.erase(segs.begin());
        segs.erase(segs.end()-1);
        createKeywords(segs);
    } else if (segs[1] == ":"){
      //  cout << "parsing an expression" << endl;
        createExp(segs);
    } else if (segs[1] == "="){
       // cout << "parsing a definition" << endl;
        createDef(segs);
    } else {
        //error
    }
}
void Parser::createExp(vector<string> segs){
    string className = segs[0];
  //  cout << "expression name = " << className << endl;
    vector<string> exp;
    copy(segs.begin()+2, segs.end(), back_inserter(exp));
   // cout << "# tokens = " << exp.size() << endl;
    RDP rdp(exp, defs);
    NFA expression = rdp.toNFA();
    expression.setName(className);
    expression.setOrder(order);
    order++;
     int startState = expression.getStartState();
    set<int> acceptedStates = expression.getAcceptStates();
    set<char> symbols = expression.getSymbols();
    set<int> states = expression.getStates();
    vector<map<char,set<int>>> transitionTable = expression.getTransitionTable();
    string tokenName = expression.getName();
    unordered_map<int, pair<int, string>> acceptedOfAllNFA = expression.getAcceptedTokens();
   /* cout<<"start state: "<<startState<<endl;
    cout<<"accepted states: ";
    for(int i : acceptedStates)
        cout<< i << " ";
    cout<<endl;
    cout<< "symbols: ";
    for(char c : symbols)
        cout<< c << " ";
    cout<<endl;
    cout<<"states: ";
    for(int i : states)
        cout<< i << " ";
    cout<<endl;
    cout<<"transition Table: "<<endl;
    for(int i = 0; i < transitionTable.size(); i++){
        map<char, set<int>> m = transitionTable.at(i);
        for(map<char, set<int>>::iterator it = m.begin(); it!=m.end(); it++){
            cout<<"state: "<< i <<" takes input: "<<it->first<< " and go to state: ";
            for(int j : it->second){
                cout<<j<< " ";
            }
            cout<<endl;
        }
    }*/
    this->exp.push_back(expression);
}
void Parser::createDef(vector<string> segs){
    string className = segs[0];
    //cout << "definition name = " << className << endl;
    vector<string> exp;
    copy(segs.begin()+2, segs.end(), back_inserter(exp));
   // cout << "# tokens = " << exp.size() << endl;
    RDP rdp(exp, defs);
    NFA expression = rdp.toNFA();
    expression.setOrder(order);
    order++;
    int startState = expression.getStartState();
    set<int> acceptedStates = expression.getAcceptStates();
    set<char> symbols = expression.getSymbols();
    set<int> states = expression.getStates();
    vector<map<char,set<int>>> transitionTable = expression.getTransitionTable();
    string tokenName = expression.getName();
    unordered_map<int, pair<int, string>> acceptedOfAllNFA = expression.getAcceptedTokens();
  /*  cout<<"start state: "<<startState<<endl;
    cout<<"accepted states: ";
    for(int i : acceptedStates)
        cout<< i << " ";
    cout<<endl;
    cout<< "symbols: ";
    for(char c : symbols)
        cout<< c << " ";
    cout<<endl;
    cout<<"states: ";
    for(int i : states)
        cout<< i << " ";
    cout<<endl;
    cout<<"transition Table: "<<endl;
    for(int i = 0; i < transitionTable.size(); i++){
        map<char, set<int>> m = transitionTable.at(i);
        for(map<char, set<int>>::iterator it = m.begin(); it!=m.end(); it++){
            cout<<"state: "<< i <<" takes input: "<<it->first<< " and go to state: ";
            for(int j : it->second){
                cout<<j<< " ";
            }
            cout<<endl;
        }
    }*/
    //expression.setName(className);
    defs[className] = expression;
}
void Parser::createKeywords(vector<string> segs){
    for(auto s: segs){
        cout << s << endl;
        vector<string> temp;
        temp.push_back(s);
        RDP rdp(temp, defs);
        NFA kwNFA = rdp.toNFA();
        kwNFA.setOrder(order);
        order++;
        //save it
        s.erase(remove(s.begin(), s.end(), '\\'), s.end());
        kwNFA.setName(s);
        int startState = kwNFA.getStartState();
        set<int> acceptedStates = kwNFA.getAcceptStates();
    set<char> symbols = kwNFA.getSymbols();
    set<int> states = kwNFA.getStates();
    vector<map<char,set<int>>> transitionTable = kwNFA.getTransitionTable();
    string tokenName = kwNFA.getName();
    unordered_map<int, pair<int, string>> acceptedOfAllNFA = kwNFA.getAcceptedTokens();
    /*cout<<"start state: "<<startState<<endl;
    cout<<"accepted states: ";
    for(int i : acceptedStates)
        cout<< i << " ";
    cout<<endl;
    cout<< "symbols: ";
    for(char c : symbols)
        cout<< c << " ";
    cout<<endl;
    cout<<"states: ";
    for(int i : states)
        cout<< i << " ";
    cout<<endl;
    cout<<"transition Table: "<<endl;
    for(int i = 0; i < transitionTable.size(); i++){
        map<char, set<int>> m = transitionTable.at(i);
        for(map<char, set<int>>::iterator it = m.begin(); it!=m.end(); it++){
            cout<<"state: "<< i <<" takes input: "<<it->first<< " and go to state: ";
            for(int j : it->second){
                cout<<j<< " ";
            }
            cout<<endl;
        }
    }*/
        exp.push_back(kwNFA);
    }
}

int Parser::getOperator(char c){
    switch(c){
    case '=':
        return 1;
        break;
    case ':':
        return 2;
        break;
    case '{':
        return 3;
        break;
    case '}':
        return 4;
        break;
    case '[':
        return 5;
        break;
    case ']':
        return 6;
        break;
    case '-':
        return 7;
        break;
    case '|':
        return 8;
        break;
    case '+':
        return 9;
        break;
    case '*':
        return 10;
        break;
    case '(':
        return 11;
        break;
    case ')':
        return 12;
        break;
    case ' ':
        return 13;
        break;
    default:
        return 0;
    }
}

