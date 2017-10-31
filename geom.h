#ifndef GEOM_H
#define GEOM_H

#include <iostream>
#include "lib/SimpleCOLLADA.hpp"
#include "myvertex.h"
#include <map>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace std;
using namespace SimpleCOLLADA;

class Geom
{
public:
    Geom();
    ~Geom();

    void GetFootprint(vector<MyVertex> &plan2d,string path);

    //按照标高z，把三个点从小到大排序
    void sort(Vertex * p0,Vertex * p1,Vertex * p2);

    //用标高为z的水平面切一个三角形，得到的交点插入容器。返回交点个数
    int Cut(Vertex * p0,Vertex * p1,Vertex * p2, double z);

    //将点p0推入v的末端。如果p0与v的最后两点在同一直线，则不推入。
    void InsertPoint(vector<MyVertex> &v, MyVertex *p0);

    //获取线段和给定标高z的交点。p0, p1不能在同一标高。
    Vertex * GetIntersect(Vertex * p0, Vertex * p1, double z);

    //把乱序的线段连成一个多边形
    void connect(vector<MyVertex> &v);

    //给定一个线段，返回该线段相邻的另一线段的非公共端点
    MyVertex findNext(MyVertex* p0,MyVertex *p1);

    vector<MyVertex> *nodes;
    double h_ratio=0.1; //切建筑的水平面的标高占总高的比例
    //multimap<MyVertex,MyVertex> table;   //水平面切各三角形得到的线段。1个线段对应两个记录 (A,B) (B,A)
    vector<vector<MyVertex>> table;   //水平面切各三角形得到的线段。1个线段对应两个记录 (A,B) (B,A)
};

#endif // GEOM_H
