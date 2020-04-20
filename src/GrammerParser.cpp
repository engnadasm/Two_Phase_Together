#include "../include/GrammerParser.h"

GrammerParser::GrammerParser(string inputFile)
{
    this->path = inputFile;
}

void GrammerParser::parseRules(){
    //first pass
    string line;
    string r;
    ifstream file(path);
    while(getline(file, line)){
        trim(line);
        if(line[0] == '#'){
            if(!r.empty()){
                addRule(r);
            }
            r = line;
        } else {
            r += " " + line;
        }
    }
    addRule(r);
    file.close();

    // second pass

    for(auto x: passOne){
        //cout << "rule: " << x.first << " :" <<endl;
        vector<Rule*> p;
        for(auto seg: x.second){
            //cout << " " << seg << endl;
            //string seg = s;
            if(seg == "|"){
                if(p.empty()){
                    //error
                } else {
                    vector<Rule*> temp(p.begin(), p.end());
                    int idx = indexes[x.first];
                    rules[idx]->addProduction(temp);
                    p.clear();
                }
            } else if (seg[0] == '\''){
                //terminal
                //cout<<"pefore  " << seg << endl;
                seg = seg.substr(1, seg.size() - 2);
                //cout << seg << endl;
                if(terminals.find(seg) == terminals.end()){
                    Rule* rule = new Rule(seg);
                    rule->setTerminal();
                    terminals[seg] = rule;
                    p.push_back(rule);
                } else {
                    p.push_back(terminals[seg]);
                }
            } else if (seg[0] == '\\'){
                if(seg == "\\L"){
                    if(terminals.find(seg) == terminals.end()){
                        Rule* rule = new Rule(seg);
                        rule->setTerminal();
                        terminals[seg] = rule;
                        p.push_back(rule);
                    } else {
                        p.push_back(terminals[seg]);
                    }
                } else {
                    //mfesh case kda a3tked
                }
            } else{
                if(indexes.find(seg) == indexes.end()){
                    //error
                } else {
                     p.push_back(rules[indexes[seg]]);
                }
            }
        }
        if(!p.empty()){
            vector<Rule*> temp(p.begin(), p.end());
            int idx = indexes[x.first];
            rules[idx]->addProduction(temp);
            p.clear();
        }
        //printRule(rules[indexes.find(x.first)->second]);

    }
    //cout << "================================="<<endl;
}

vector<string> GrammerParser::segmentation(string line){
    line.erase(0,1);
    vector<string> segs;
    int i = 0;
    string seg;
    while(i < line.size()){
        if(line[i] == '\''){
            seg.append(string(1,line[i]));
            i++;
            while(line[i] != '\''){
                seg.append(string(1,line[i]));
                i++;
            }
            seg.append(string(1,line[i]));
        } else if (line[i] == '=' || line[i] == '|') {
            if(seg.size() > 0){
                segs.push_back(seg);
            }
            seg = string(1, line[i]);
            segs.push_back(seg);
            seg.clear();
        } else if(line[i] == ' '){
            if(seg.size() > 0){
                segs.push_back(seg);
                seg.clear();
            }
        } else {
            seg.append(string(1,line[i]));
            if(line[i] == '\\'){
                seg += string(1,line[i+1]);
                i++;
            }
        }
        i++;
    }
    if(seg.size() > 0){
        segs.push_back(seg);
    }
    return segs;
}

void GrammerParser::addRule(string r){
    vector<string> segs = segmentation(r);
    vector<string> rhs(segs.begin()+2, segs.end());
    if(passOne.find(segs[0]) != passOne.end()){
        rhs.insert(rhs.begin(), "|");
        passOne[segs[0]].insert(passOne[segs[0]].end(), rhs.begin(), rhs.end());
    } else {
        passOne[segs[0]] = rhs;
        Rule* rule = new Rule(segs[0]);
        rules.push_back(rule);
        indexes.insert({segs[0], rules.size()-1});
    }

}

vector<Rule*> GrammerParser::getRules(){
    return rules;
}
