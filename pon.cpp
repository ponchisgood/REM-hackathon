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

int ProductCount=0;
map<string, int> CountOfEachProduct;

bool not_target(const char *user){
    //fliters
    static const char* fliter[] = {
        "b8nkfu54lbcjcecc",
        nullptr
    };

    for (int i=0;fliter[i];i++){
        bool found = true;
        for (int j=0;*(fliter[i]+j) && *(user+j);j++)
            if (*(fliter[i]+j) != *(user+j)){
                found = false;
                break;
            }
        if (found) return false;
    }
    return true;
}


//get a product name each time
string getproductname(string& str){
    int i=0;
    string tmp;
    while (str[i]!= ',' && str[i]!= '\0'){
        i++;
        tmp+=str[i];
    }
    if(str[i]==','){
        str.erase(0,i+1);
        ProductCount++;
    }
    else str.clear();
    return tmp;
}


//read a file and store in global variables
void readfile(int date, int page){
    stringstream filename;
    filename << "data/transactions/" << date << "-page" << page << ".json";
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
        if (d["results"][i]["user"].IsNull()) continue;
        const char* user = d["results"][i]["user"].GetString();
        if (not_target(user) || d["results"][i]["content_ids"].IsNull()) continue; 
        string products=d["results"][i]["content_ids"].GetString();
        while(1){
            tmp=getproductname(products);
            if(tmp.size()==0)break;
            CountOfEachProduct[tmp] ++;
        }
    }
    file.close();
}


int main (){
    for (int i=0;i<5;i++)
        readfile(20161028, i);
    cout << "Total Products: " << ProductCount << endl;
    for (auto &x : CountOfEachProduct)
        cout << "Product \"" << x.first << "\" " << x.second << " times" << endl;
}

