#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unistd.h>

using namespace rapidjson;
using namespace std;


int userCount;
int adCount = 5;
map<string, int> CountOfEachAd;


//flits the ip you want
bool is_invalid(const char *ip){
    //fliters
    static const char* fliter[] = {
        "140.113",
        "103.31",
        nullptr
    };

    for (int i=0;fliter[i];i++){
        bool found = true;
        for (int j=0;*(fliter[i]+j) && *(ip+j);j++)
            if (*(fliter[i]+j) != *(ip+j)){
                found = false;
                break;
            }
        if (found) return false;
    }
    return true;
}


//get host name of an URL
string gethostname(const string& str){
    size_t i = str.find("://");
    if (i != string::npos) i += 3;
    else i = 0;
    while (str[i]!= '/' && str[i]!= '\0') i++;
    string tmp;
    for (size_t j=0;j<i;j++) tmp+= str[j];
    return tmp;
}


//read a file and store in global variables
void readfile(int date, int page){
    stringstream filename;
    filename << "data/" << date << "-page" << page << ".json";
    fstream file(filename.str(), fstream::in);
    if (!file.is_open()){
        cerr << "error: can't open file " << filename.str() << endl;
        return;
    }
    string tmp;
    for (int c; (c = file.get())!= EOF;) tmp+= c;
    Document d;
    d.Parse(tmp.c_str());
    for (size_t i=0;i<d["results"].Size();i++){
        userCount ++;
        if (d["results"][i]["ip"].IsNull()) continue;
        const char* ip = d["results"][i]["ip"].GetString();
        if (is_invalid(ip) || d["results"][i]["page"].IsNull()) continue;
        CountOfEachAd[gethostname(d["results"][i]["page"].GetString())] ++;
    }
    file.close();
}


int main (){
    for (int i=0;i<100;i++)
        readfile(20161028, i);
    cout << "Total Users: " << userCount << endl;
    for (auto &x : CountOfEachAd)
        cout << "Page \"" << x.first << "\" " << x.second << " times" << endl;
}
