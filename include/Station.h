#ifndef STATION_H
#define STATION_H

#include <string>
using namespace std;

struct Station {
    string name;
    string code;
    int id;

    Station(string n = "", string c = "", int i = -1) {
        name = n;
        code = c;
        id = i;
    }
};

#endif