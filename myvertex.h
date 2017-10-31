#ifndef MYVERTEX_H
#define MYVERTEX_H

#include <iostream>
#include "lib/SimpleCOLLADA.hpp"
using namespace std;

class MyVertex: public SimpleCOLLADA::Vertex
{
public:
    MyVertex();
    MyVertex(double x,double y,double z);
    MyVertex(SimpleCOLLADA::Vertex *v);
    bool operator==(const MyVertex &a);
    double tol;
};


#endif // MYVERTEX_H
