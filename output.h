#ifndef OUTPUT_H
#define OUTPUT_H

#include <fstream>
#include <iostream>
#include "lib/SimpleCOLLADA.hpp"
#include "myvertex.h"
#include <map>
#include <vector>
#include <iomanip>

using namespace SimpleCOLLADA;
using namespace std;

class Output
{
public:
    Output();
    ~Output();

    void WriteAll(vector<MyVertex> *v);
    void WritePoint(vector<MyVertex> *v,int count);
    void WriteTable(vector<vector<MyVertex>> *table);

    ofstream oall;
    ofstream otable;
    ofstream opoint;
};

#endif // OUTPUT_H
