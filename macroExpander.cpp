#include<iostream>
#include<fstream>
#include<sstream>
#include <algorithm>
#include<string>
#include<unordered_map>
#include<vector>

using namespace std;

class macros{
    public:
    string name;
    vector<string> MDT;
    unordered_map<string, int> ILT;
    string str;
    bool arguementfilled = false;
    macros(){
        MDT.clear();
        ILT.clear();
        name.clear();
        arguementfilled = false;
    }
    void Empty()
    {
        *this = macros();
    }

};

vector<macros> macroList;
vector<string> MLT;
string expandedCode;
string getData(string filename){
    ifstream f(filename); 
    string str;
    if(f) {
        ostringstream ss;
        ss << f.rdbuf(); 
        str = ss.str();
    }
    else
    {
        cout<<"\nFile not found or file is empty\n";
        str = "";
    }
    
    return str;
}

void macroProcedure(string line, macros& dummyMacro){
    
    dummyMacro.MDT.push_back(line);
    stringstream ss(line);
    int i = 0;
        while (ss.good()) {
            string identifier;
            getline(ss, identifier, ',');
            // cout<<identifier<<endl;//
            string word;
            stringstream iss(identifier);
            
            while (iss >> word){
                // cout<<word<<endl;
                if (dummyMacro.name == "")
                {
                    dummyMacro.name = word;
                }
                else
                {
                    if (dummyMacro.arguementfilled == false)
                    {
                        dummyMacro.ILT[word] = ++i;
                        // cout<<word<<" "<<i<<endl;
                        
                    }
                    else
                    {
                        // cout<<word<<endl;
                        if (dummyMacro.ILT.find(word) != dummyMacro.ILT.end())
                        {
                            // boost::replace_all(dummyMacro.MDT[dummyMacro.MDT.size() - 1], word, to_string(dummyMacro.ILT[word]));
                            size_t pos;
                            while ((pos = dummyMacro.MDT[dummyMacro.MDT.size() - 1].find(word)) != std::string::npos) {
                                dummyMacro.MDT[dummyMacro.MDT.size() - 1].replace(pos, word.length(), "#" + to_string(dummyMacro.ILT[word]));
                            }
                        }
                        
                        
                    }
                    
                }
            }
        }
        dummyMacro.arguementfilled = true;
    // getchar();
}

string printKey(unordered_map<string, int>& Map,int K)
{
 
    bool a = true;
    for (auto& it : Map) {
 
        if (it.second == K) {
            return it.first;
            a = false;
        }
    }
 
    if (a) {
        cout << "-1";
    }
    return NULL;
}

void expansionProcedure(string line){
    // cout<<line<<endl;
    stringstream ss(line);
    int index = 0;
    macros dummyMacro;
    int mapiterator = 0;
    bool macrofound = false;
        while (ss.good()) {
            string identifier;
            getline(ss, identifier, ',');
            string word;
            stringstream iss(identifier);
            while (iss >> word){
                
                if (!macrofound)
                {
                    auto pos = find(MLT.begin(), MLT.end(), word);
                    if (pos != MLT.end())
                    {
                        index = pos - MLT.begin();
                        dummyMacro = macroList.at(index);
                        macrofound = true;
                    }
                }
                else
                {
                    cout<<++mapiterator<<"\t";
                    cout<<printKey(dummyMacro.ILT, mapiterator)<<"\t";
                    cout<<word<<endl;

                    string stfind = "#" + to_string(mapiterator);
                    size_t pos;
                    // cout<<dummyMacro.str<<endl;
                    while ((pos = dummyMacro.str.find(stfind)) != std::string::npos) {
                        dummyMacro.str.replace(pos, stfind.length(), word);
                    }                    
                }     
            } 
        }
        if (macrofound)
        {
            expandedCode += dummyMacro.str;
            macrofound = false;
        }
        else
        {
            expandedCode += line + "\n";
        }

}

bool macrofound = false;
int main(){
    string data = getData("macroFile.txt");
    // cout<<data;
    stringstream ss(data);
    macros dummyMacro;
    while (ss.good()) {
        
        string identifier;
        getline(ss, identifier, '\n');
        if (identifier == "MEND")
        {
            macrofound = false;
            for(int i=1; i < dummyMacro.MDT.size(); i++)
                dummyMacro.str += (dummyMacro.MDT.at(i) + "\n");
            macroList.push_back(dummyMacro);
            MLT.push_back(dummyMacro.name);
            cout<<"-----------MDT---------------\n";
            for(int i=0; i < dummyMacro.MDT.size(); i++)
                cout<<dummyMacro.MDT.at(i)<<endl;
            dummyMacro.Empty();
        }
        if (macrofound)
        {
            macroProcedure(identifier, dummyMacro);
        }
        if (identifier == "MACRO")
        {
            macrofound = true;
            
        }
         
    }

    cout<<"----------MNT--------------\n";
    for (int i = 0; i < macroList.size(); i++)
    {
        cout<<i<<"\t"<<macroList.at(i).name<<endl;
    }
    

    stringstream ss2(data);
    cout<<"----------ALT------------\n";
    while (ss2.good()) {
        
        string identifier;
        getline(ss2, identifier, '\n');
        if (identifier == "MACRO")
        {
            macrofound = true;
        }
        if (!macrofound)
        {
            expansionProcedure(identifier);
        }
        if (identifier == "MEND")
        {
            macrofound = false;
        }   
    }

    cout<<"----------Expanded Code--------------\n";
    cout<<expandedCode;
    


    return 0;
}