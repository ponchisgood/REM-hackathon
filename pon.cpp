#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <unistd.h>

using namespace rapidjson;
using namespace std;

int ProductCount=0;
map<string, int> CountOfEachProduct;

//fliters
char** fliter;
bool not_target(const char *user){
    for (int i=0;fliter[i];i++){
        if (!strcmp(fliter[i], user)) return false;
    }
    return true;
}


//get a product name each time
string getproductname(string& str){
    int i=0;
    string tmp;
    while (str[i]!= ',' && str[i]!= '\0'){
        tmp+=str[i++];
    }
    if(str[i]==','){
        str.erase(0,i+1);
        ProductCount++;
    }
    else str.clear();
    return tmp;
}


//read a file and store in global variables
int readfile(int date, int page){
    stringstream filename;
    filename << "data/transactions/" << date << "-page" << page << ".json";
    fstream file(filename.str(), fstream::in);
    if (!file.is_open()) return 1;
    string tmp;
    for (int c; (c = file.get())!= EOF;) tmp+= c;
    Document d;
    d.Parse(tmp.c_str());
    if (d.IsNull()){
        file.close();
        return 0;
    }
    for (size_t i=0;i<d["results"].Size();i++){
        if (d["results"][i]["user"].IsNull()) continue;
        const char* user = d["results"][i]["user"].GetString();
        if (not_target(user) || d["results"][i]["content_ids"].IsNull()) continue;
        string products = d["results"][i]["content_ids"].GetString();
        while(1){
            tmp = getproductname(products);
            if (tmp == "") break;
            CountOfEachProduct[tmp] ++;
        }
    }
    file.close();
    return 0;
}


int main (int argc, char** argv){
    fliter = argv+2;
    {
        if (argc < 2){
            cerr << "error: at least one argument needed." << endl;
            return 1;
        }
        stringstream ss;
        ss << argv[1];
        int date;
        ss >> date;
        //readfile return 1 if file is not exist
        for (int i=0; i<1837; i++)
            readfile(date, i);
    }
    {
        fstream file;
        vector<pair<int, string> > v;
        for (auto &x : CountOfEachProduct)
            v.push_back({x.second, x.first});
        sort(v.begin(), v.end());

        StringBuffer s;
        Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("count");
        writer.Uint(ProductCount);
        writer.Key("imgs");
        writer.StartArray();

        for (size_t i=0;i<5;i++){
            if (v.size() < i + 1) break;
            cout << v[v.size()-i-1].second << endl;
            string tmp = "curl -H \"Authorization: token a04fdfceb19325e627514bbcb551ef1f78fdbb18\" \"https://meichu.tagtoo.com.tw/products/";
            tmp += v[v.size()-i-1].second;
            tmp += "\" > tmp.json";
            system(tmp.c_str());
            file.open("tmp.json", fstream::in);
            tmp = "";
            for (int c; (c = file.get())!= EOF; tmp+=c);
            file.close();

            Document d;
            d.Parse(tmp.c_str());
            writer.StartObject();
            writer.Key("image_url");
            if (d.HasMember("detail") || d["image_url"].IsNull()) writer.Null();
            else writer.String(d["image_url"].GetString());
            writer.Key("title");
            if (d.HasMember("detail") || d["title"].IsNull()) writer.Null();
            else writer.String(d["title"].GetString());
            writer.Key("description");
            if (d.HasMember("detail") || d["description"].IsNull()) writer.Null();
            else writer.String(d["description"].GetString());
            writer.Key("count");
            writer.Uint(v[v.size()-i-1].first);
            writer.EndObject();
        }
        writer.EndArray();
        writer.EndObject();

        file.open("output/img.json", fstream::app);
        file << s.GetString() << endl;
        file.close();
    }
}

