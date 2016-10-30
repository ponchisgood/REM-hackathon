#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
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
    cout << "Total Products: " << ProductCount << endl;
    for (auto &x : CountOfEachProduct)
        cout << "Product \"" << x.first << "\" " << x.second << " times" << endl;
}

