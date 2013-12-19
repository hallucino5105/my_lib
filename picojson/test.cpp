#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include "picojson.h"


using namespace std;


int main(int argc, char* argv[])
{
    picojson::value v;

    //const char* str = "{\"a\" : 1, \"b\" : \"test\"}";
    //picojson::parse(v, str, str + strlen(str));

    ifstream is("/Users/hoshino/program_local/openFrameworks/apps/project/oftest/src/dat/data_20120510123810.json");
    picojson::parse(v, is);

    if(v.is<picojson::object>()) {
        picojson::value a = v.get("a");
        if(a.is<double>()) {
            cout << "a: " << a.get<double>() << endl;
        }

        picojson::value b = v.get("b");
        if(b.is<string>()){
            cout << "b: " << b.get<string>() << endl;
        }
    }

    return 0;
}


