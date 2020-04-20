#include "../include/Derivation.h"
#include <fstream>
Derivation* Derivation::derivationInstance = NULL;

Derivation::Derivation()
{
    // do init stuff
}

Derivation* Derivation::GetInstance()
{
    if (derivationInstance== NULL)
    {
        derivationInstance = new Derivation();
    }
    return derivationInstance;
}

void Derivation::generateDerivations()
{
    string input = tokens.front();
    Rule* currentTopStack = ourStack.top();
    string nameTopStack = (*currentTopStack).getName();
    bool flag = true;
    bool error = false;
    if(!currentTopStack->isTerminal())
    {
        unordered_map<string, vector<Rule*>> currentRow = ourParsingTable[nameTopStack];
        vector<Rule*> nextTran = currentRow[input];
       // cout << "*****\n";
        if(nextTran.empty() == false)
        {
            ourStack.pop();
            string out = "";
            for (auto i = nextTran.rbegin(); i != nextTran.rend(); ++i)
            {
                if((*i)->getName().compare("synch") == 0 || (*i)->getName().compare("\\L") == 0)
                {
                    outToPrint.push_back("reach " + (*i)->getName());
                    break;
                }
                else
                {
                   // cout << (*i)->getName() << " ";
                    ourStack.push(*i);
                    out = out + (*i)->getName() + " ";
                }
            }
          //  cout<<"\n";
            currentTopStack = ourStack.top();
            string out2 = PrintStack(ourStack);
            outToAdd = "";
            outToPrint.push_back(out2);
        }
        else
        {
            error = true;
            //accept = false;
        }
    }
    //cout << "stack is \n";
    //cout<<"\n";
    while(flag)
    {
        currentTopStack = ourStack.top();

        if(error)
        {
            flag = false;
            cout << "there is error where can't reach to " + input + " under non terminal "
                 + currentTopStack->getName() + "\n";
            outToPrint.push_back("there is error where can't reach to " + input
                                 + " under non terminal " + currentTopStack->getName());
            tokens.pop();
            error = false;
        }
        /*else if (currentTopStack->getName().compare("synch") == 0)
        {
            cout << "we reach to sync under terminal " +input + " in non terminal"
                 + currentTopStack->getName() + "\n";
            outToPrint.push_back("we reach to sync under terminal " +input + " in non terminal"
                                 + currentTopStack->getName() + ", So it be :");
            ourStack.pop();
            cout << "stack is \n";
            string out2 = PrintStack(ourStack);
            outToAdd = "";
            outToPrint.push_back(out2);
            cout<<"\n";

        }
        else if(currentTopStack->getName().compare("\\L") == 0)
        {
            //for epsilon
            cout<< "compare \\L with " + currentTopStack->getName() +"\n";
            ourStack.pop();
            cout << "stack is \n";
            string out2 = PrintStack(ourStack);
            outToAdd = "";
            outToPrint.push_back(out2);
            cout<<"\n";

        }*/
        else if(ourStack.empty())   //case if ourstack finish but still we have tokens
        {
            flag = false;
            cout << "error,we can't complete parser and not accept this program\n";
            outToPrint.push_back("error,we can't complete parser\n");
            tokens.pop();
            accept = false;
        }
        else if(currentTopStack->isTerminal())
        {
            if(input.compare(currentTopStack->getName()) == 0)
            {
                cout<< "compare " << input <<" with " << currentTopStack->getName()<<"\n";
                tokens.pop();
                ourStack.pop();
                //cout << "stack is \n";
                string out2 = PrintStack(ourStack);
                outToAdd = "";
                outToPrint.push_back(out2);
                //cout<<"\n";
                flag = false;
            }
            else
            {
                flag = false;
                cout << "there is error in matching " + input +"\n";
                outToPrint.push_back("there is error in matching " + input );
                tokens.pop();
                // accept = false;
            }
        }
        else
            while(!currentTopStack->isTerminal())
            {
                flag = false;
                currentTopStack = ourStack.top();
                nameTopStack = (*currentTopStack).getName();
                //cout<< "Top " << nameTopStack << "\n";
                unordered_map<string, vector<Rule*>> currentRow = ourParsingTable[nameTopStack];
                vector<Rule*> nextTran = currentRow[input];
                if(nextTran.empty() == false)
                {
                    ourStack.pop();
                    string out = "";
                  //  cout << ".........\n";
                    for (auto i = nextTran.rbegin(); i != nextTran.rend(); ++i)
                    {
                        if((*i)->getName().compare("synch") == 0 || (*i)->getName().compare("\\L") == 0)
                        {
                            outToPrint.push_back("reach " + (*i)->getName());
                            break;
                        }
                        else
                        {
                           // cout << (*i)->getName() << " ";
                            ourStack.push(*i);
                            out = out + (*i)->getName() + " ";
                        }
                    }
                   // cout<<"\n";
                    currentTopStack = ourStack.top();
                   // cout << "stack is \n";
                    string out2 = PrintStack(ourStack);
                    outToAdd = "";
                    outToPrint.push_back(out2);
                    //cout<<"\n";

                }
                else
                {
                    error = true;
                    break;
                    //accept = false;
                }
            }
    }
}

string Derivation::PrintStack(stack<Rule*> s)
{
    // If stack is empty then return
    if (s.empty())
        return "";
    Rule* x = s.top();
    // Pop the top element of the stack
    s.pop();
    // Recursively call the function PrintStack
    PrintStack(s);

    // Print the stack element starting
    // from the bottom
    outToAdd = outToAdd + x->getName() + " ";
    //cout << x->getName() << " ";

    // Push the same element onto the stack
    // to preserve the order
    s.push(x);
    return outToAdd;
}
void Derivation::PrintDerivations()
{
    ofstream myfile ("Syntax_file.txt");
    if (myfile.is_open())
    {
        for(int i = 0; i < outToPrint.size(); i++)
        {
            myfile << outToPrint[i];
            myfile << "\n";
           // cout << outToPrint[i] << "\n";
        }
        myfile.close();
    }
    else
        cout << "Unable to open file";
}

void Derivation::enterToken(string token)
{
    this->tokens.push(token);
    generateDerivations();
}

void Derivation::setStart(Rule* start)
{
    Rule* endDoll = new Rule("$");
    endDoll->setTerminal();
    this->ourStack.push(endDoll);
    this->ourStack.push(start);
    outToPrint.push_back("$" + (*start).getName());
    //outToPrint.push_back((*start).getName());
    //cout<< (*start).getName();
}

void Derivation::setParsingTable(unordered_map<string,unordered_map<string, vector<Rule*>>> parsingTable)
{
    this->ourParsingTable = parsingTable;
}

bool Derivation::getAccept()
{
    return this->accept;
}

void Derivation::endDerivation()
{
    this->tokens.push("$");
    if (tokens.empty())
    {
        cout << "\nwe reach to case that token finish but still we have non terminals in ourstack which they are :\n";
        outToPrint.push_back("\nwe reach to case that token finish but still we have non terminals in ourstack which they are :\n");
        while(!ourStack.empty())   //case if token finish but still we have things in ourstack
        {
            cout << ourStack.top()->getName()<< "\n";
            outToPrint.push_back(ourStack.top()->getName());
            ourStack.pop();
        }
    }
    else
    {
        while(!tokens.empty())   //case if token finish but still we have things in ourstack
        {
            generateDerivations();
        }
        if(!ourStack.empty())
        {
            cout << "\nwe reach to case that token finish but still we have non terminals in ourstack which they are :\n";
            outToPrint.push_back("\n we reach to case that token finish but still we have non terminals in ourstack which they are :\n");
            while(!ourStack.empty())   //case if token finish but still we have things in ourstack
            {
                cout << ourStack.top()->getName()<< "\n";
                outToPrint.push_back(ourStack.top()->getName());
                ourStack.pop();

            }
        }
    }
    PrintDerivations();
}
