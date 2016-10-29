#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unistd.h>

using namespace rapidjson;
using namespace std;


//array of 24 element due to 24 hours a day
size_t userCount[24];
map<string, int> CountOfEachAd[24];
map<string, int> Users[24];

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


//remove some operator of an URL
string gethostname(const string& str){
    size_t i = str.find('?');
    if (i == string::npos) i = str.size();
    string tmp;
    for (size_t j=0;j<i;j++) tmp+= str[j];
    return tmp;
}


//read a file and store in global variables
//returns 1 if something wrong
int readfile(int date, int page){
    stringstream filename;
    filename << "data/" << date << "-page" << page << ".json";
    fstream file(filename.str(), fstream::in);
    if (!file.is_open()) return 1;
    string tmp;
    for (int c; (c = file.get())!= EOF;) tmp+= c;
    Document d;
    d.Parse(tmp.c_str());
    if (d.IsNull()) return 0;
    for (size_t i=0;i<d["results"].Size();i++){
        stringstream ss;
        ss << d["results"][i]["start_time"].GetString();
        int Time;
        (ss.ignore(20, 'T')) >> Time;
        Users[Time][d["results"][i]["user"].GetString()] ++;
        userCount[Time] ++;
        const char* ip = d["results"][i]["ip"].GetString();
        if (!is_invalid(ip)){
            if (d["results"][i]["page"].IsNull()) CountOfEachAd[Time][""] ++;
            else CountOfEachAd[Time][gethostname(d["results"][i]["page"].GetString())] ++;
        }
    }
    file.close();
    return 0;
}


int main (int argc, char** argv){
    //readfile return 1 if file is not exist
    {
        if (argc != 2){
            cerr << "error: one argument needed." << endl;
            return 1;
        }
        stringstream ss;
        ss << argv[1];
        int date;
        ss >> date;
        for (int i=0; !readfile(date, i); i+=100);
    }

    vector<pair<int, string> > v[24];
    for (int i=0;i<24;i++)
        for (auto &x : CountOfEachAd[i])
            v[i].push_back({x.second, x.first});
    for (int i=0;i<24;i++)
        sort(v[i].begin(), v[i].end());

    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();
    writer.Key("seg");
    writer.StartArray();
    for (int i=0;i<24;i++){
        writer.StartObject();
        writer.Key("count");
        writer.Uint(userCount[i]);
        writer.Key("ads");
        writer.StartArray();
        for (int j=0;j<5;j++){
            if (v[i].size() < 1u + j) continue;
            writer.StartObject();
            writer.Key("page");
            writer.String(v[i][v[i].size()-j-1].second.c_str());
            writer.Key("users");
            writer.Uint(v[i][v[i].size()-j-1].first);
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();

    cout << s.GetString() << endl;
/*
    for (int i=0;i<24;i++){
        cout << i << " o'clock:" << endl;
        for (auto &x : Users[i]) cout << ' ' << '(' << x.first << ',' << x.second << ')';
        cout << endl;
    }*/
}
