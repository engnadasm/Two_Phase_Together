#include "../include/RDB.h"

RDP::RDP(vector<string> tokens, unordered_map<string ,NFA> defs)
{
    this->tokens = tokens;
    this->defs = defs;
    index = this->tokens.begin();

}

NFA RDP::toNFA(){
    NFA nfa = concat();
    while(match("|")){
        NFA temp;
        NFA right = concat();
        temp.orOperator(nfa, right);
        nfa = temp;
    }
    return nfa;
}

NFA RDP::concat(){
    NFA nfa = unary();
    while(index != tokens.end() && (*index) != "|" && (*index) != ")"){
        NFA temp;
        NFA right = unary();
       // cout << "concatenating with a new unary" <<endl;
        temp.concatenateOperator(nfa, right);
       // cout << "concatenation success" <<endl;
        nfa = temp;
    }
    return nfa;
}

NFA RDP::unary(){
    NFA p = primary();
    NFA nfa;
    if(match("+")){
      //  cout << "plus operator" << endl;
        nfa.plusOperator(p);
      //  cout << "ended plus operator" << endl;
    } else if(match("*")){
        nfa.starOperator(p);
    }else {
        return p;
    }
    return nfa;
}

NFA RDP::primary(){
    NFA nfa;
    if(match("\\L")){
        nfa.concatenateOperator('\0', '\0');
        return nfa;
    } else if(isChar()){
        cout <<"returning a char" <<endl;
        return charSet();
    } else if(match("(")){
        cout << "a ( is found" << endl;
        nfa = toNFA();
        if(!match(")")){
            cout << "error parentheses" <<endl;
        }
        return nfa;
    } else {
        cout << "returning a word" << endl;
        return word();
    }

}

NFA RDP::charSet(){
    string w = current();
    w.erase(remove(w.begin(), w.end(), '\\'), w.end());
    NFA nfa;
    char left = w[0];
    cout << "the char is  " << left << endl;
    if(match("-")){
        w = current();
        w.erase(remove(w.begin(), w.end(), '\\'), w.end());
        char right = w[0];
        nfa.dashOperator(left, right);
    } else {
        nfa.concatenateOperator(left, '\0');
    }
    return nfa;
}

NFA RDP::word(){
    string w = current();
    cout << "the word is: " << w << endl;
    unordered_map<string, NFA>::const_iterator it = defs.find(w);
    if(it == defs.end()){
        NFA nfa;
        w.erase(remove(w.begin(), w.end(), '\\'), w.end());
        nfa.concatenateOperator(w[0], w[1]);
        for(int i = 2; i < w.size(); i++){
            nfa.concatenateOperator(nfa, w[i]);
        }
        return nfa;
    } else {
        cout << "found a definition!" << endl;
        NFA* nfa;
        nfa = new NFA(it->second);
        return *nfa;
    }
}

bool RDP::match(string token){
    if(index == tokens.end()) return false;
    if(token == *index){
        index++;
        return true;
    } else {
        return false;
    }
}

bool RDP::isChar(){
    if(isOperator()) return false;
    string w = *index;
    w.erase(remove(w.begin(), w.end(), '\\'), w.end());
    if(w.size() > 1) return false;
    return true;
}

string RDP::current(){
    string token = *index;
    index++;
    return token;
}

bool RDP::isOperator(){
    if(index == tokens.end()) return false;
    if((*index) == "-"){
        return true;
    } else if((*index) == "|"){
        return true;
    } else if((*index) == "+"){
        return true;
    } else if((*index) == "*"){
        return true;
    } else if((*index) == "("){
        return true;
    } else if((*index) == ")"){
        return true;
    }
    return false;

}
