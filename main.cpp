#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unistd.h>

using namespace rapidjson;
using namespace std;


size_t userCount;
size_t adCount = 5;
map<string, int> CountOfEachAd;
set<string> Users;

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
int readfile(int date, int page){
    stringstream filename;
    filename << "data/" << date << "-page" << page << ".json";
    fstream file(filename.str(), fstream::in);
    if (!file.is_open()) return 0;
    string tmp;
    for (int c; (c = file.get())!= EOF;) tmp+= c;
    Document d;
    d.Parse(tmp.c_str());
    if (d.IsNull()) return 1;
    for (size_t i=0;i<d["results"].Size();i++){
        Users.insert(d["results"][i]["user"].GetString());
        userCount ++;
        if (d["results"][i]["ip"].IsNull()) continue;
        const char* ip = d["results"][i]["ip"].GetString();
        if (!is_invalid(ip)){
            if (d["results"][i]["page"].IsNull()) CountOfEachAd[""] ++;
            else CountOfEachAd[gethostname(d["results"][i]["page"].GetString())] ++;
        }
    }
    file.close();
    return 1;
}


int main (int argc, char** argv){
    //readfile return 0 if file is not exist
    for (int i=0;readfile(20161028, i);i+=100);

    vector<pair<int, string> > v;
    for (auto &x : CountOfEachAd){
        //cout << '"' << x.first << '"' << " " << x.second << " times" << endl;
        v.push_back({x.second, x.first});
    }
    sort(v.begin(), v.end());
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("count");
    writer.Uint(5);
    writer.Key("users");
    writer.Uint(userCount);
    writer.Key("ads");
    writer.StartArray();
    for (int i=0;i<5;i++){
        if (v.size() < 1u + i) continue;
        writer.StartObject();
        writer.Key("page");
        writer.String(v[v.size()-i-1].second.c_str());
        writer.Key("users");
        writer.Uint(v[v.size()-i-1].first);
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();

    cout << s.GetString() << endl;
}
