/*---------------
Copyright (c) 2017 Xiang Zeng

-----------------*/

#include <iostream>
#include "lib/SimpleCOLLADA.hpp"
#include "myvertex.h"
#include <map>
#include <vector>
#include <iomanip>
#include <math.h>

using namespace std;
using namespace SimpleCOLLADA;

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

//全局变量
//multimap<MyVertex,MyVertex> table;   //水平面切各三角形得到的线段。1个线段对应两个记录 (A,B) (B,A)
vector<vector<MyVertex>> table;   //水平面切各三角形得到的线段。1个线段对应两个记录 (A,B) (B,A)

int main(int argc, char *argv[])
{
    vector<MyVertex> plan2d;    //水平面切建筑得到的平面多边形
    plan2d.clear();
    plan2d.reserve(4);
    const double h_ratio=0.1; //切建筑的水平面的标高占总高的比例

    Model model("san.dae");
    ModelNode *node=model.modelNodes[0];
    table.clear();
    double minz=99999999999;
    double maxz=-9999999999;
    int nov=node->getNumOfVertices();
    ofstream f2("results/all.txt");
    for (int i=0;i<nov;i++)
    {
        f2<<fixed<<setprecision(5)<<node->getVertex(i)->x<<"\t"<<node->getVertex(i)->y<<endl;
        if(minz>node->getVertex(i)->z)  minz=node->getVertex(i)->z;
        if(maxz<node->getVertex(i)->z)  maxz=node->getVertex(i)->z;
    }
    double elevation=minz+(maxz-minz)*h_ratio;

    int notriangle=node->getNumOfTriangles();
    for(int i=0;i<notriangle;i++)
    {
        Triangle * triangle = node->getTriangle(i);
        Vertex * vet0=node->getVertex(triangle->position[0]);
        Vertex * vet1=node->getVertex(triangle->position[1]);
        Vertex * vet2=node->getVertex(triangle->position[2]);
        //sort(vet0,vet1,vet2);
        int nop=Cut(vet0,vet1,vet2,elevation);
        cout<<nop<<"\n";
    }

    connect(plan2d);

    ofstream fout("results/vertex.txt");
    for(unsigned int i=0;i<plan2d.size();i++)
        fout<<fixed<<setprecision(5)<<plan2d[i].x<<"\t"<<plan2d[i].y<<"\t"<<minz<<endl;
    fout.close();

    return 0;
}


void sort(Vertex * p0,Vertex * p1,Vertex * p2)
{
    Vertex * temp= new Vertex;
    if(p1->z<p0->z)
    {
        temp=p1;
        p1=p0;
        p0=temp;
    }

    if(p2->z<p0->z)
    {
        temp=p2;
        p2=p0;
        p0=temp;
    }

    if(p2->z<p1->z)
    {
        temp=p2;
        p2=p1;
        p1=temp;
    }
    delete temp;
}

int Cut(Vertex * p0,Vertex * p1,Vertex * p2, double z)
{
    Vertex * temp= new Vertex;
    if(p1->z<p0->z)
    {
        temp=p1;
        p1=p0;
        p0=temp;
    }

    if(p2->z<p0->z)
    {
        temp=p2;
        p2=p0;
        p0=temp;
    }

    if(p2->z<p1->z)
    {
        temp=p2;
        p2=p1;
        p1=temp;
    }

    if(p2->z<z||p0->z>z)    return 0;
    else if(p0->z==z)
    {
        if(p1->z==z)
        {
            vector<MyVertex> a={MyVertex(p0), MyVertex(p1)};
            table.push_back(a);
            return 2;
        }
        return 1;
    }
    else if (p1->z==z)
    {
        Vertex *v;
        if(p2->z==z)     v=p2;
        else    v=GetIntersect(p0,p2,z);
        vector<MyVertex> a={MyVertex(v), MyVertex(p1)};
        table.push_back(a);
        return 2;
    }
    else if (p2->z==z)
    {
        return 1;
    }
    else if( p0->z<z&&z<p1->z)
    {
        Vertex *v1 = GetIntersect(p0,p1,z);
        Vertex *v2 = GetIntersect(p0,p2,z);
        vector<MyVertex> a={MyVertex(v1),MyVertex(v2)};
        table.push_back(a);
        return 2;
    }
    else    // p1->z < z && z < p2->z
    {
        Vertex *v1 = GetIntersect(p1,p2,z);
        Vertex *v2 = GetIntersect(p0,p2,z);
        vector<MyVertex> a={MyVertex(v1),MyVertex(v2)};
        table.push_back(a);
        return 2;
    }
}

void InsertPoint(vector<MyVertex> &v, MyVertex *p0)
{
    double tol=1e-2;
    int size=v.size();
    if(size<2)  v.push_back(*p0);
    else
    {
        double u1=v[size-2].x-p0->x;
        double v1=v[size-2].y-p0->y;
        double u2=v[size-1].x-p0->x;
        double v2=v[size-1].y-p0->y;
        double cos=(u1*u2+v1*v2)/sqrt((u1*u1+v1*v1)*(u2*u2+v2*v2));
        cout<<"\n"<<cos;
        if(1-fabs(cos)<tol) //在同一直线上
            v.pop_back();
        v.push_back(*p0);
    }
}

Vertex * GetIntersect(Vertex * p0, Vertex * p1, double z)
{
    Vertex * p = new Vertex;
    double ratio=(p1->z-z)/(p1->z-p0->z);
    p->x=p1->x-ratio*(p1->x-p0->x);
    p->y=p1->y-ratio*(p1->y-p0->y);
    p->z=z;

    return p;
}

void connect(vector<MyVertex> &v)
{
    ofstream fout("results/table.txt");
    for(unsigned int i=0;i<table.size();i++)
    {
        for(int j=0;j<2;j++)
        {
            fout<<fixed<<setprecision(5)<<table[i][j].x<<"\t"<<table[i][j].y<<"\t"<<"\t";
        }
        fout<<endl;
    }
    fout.close();

    MyVertex start=table[0][0];
    MyVertex last=table[0][1];
    InsertPoint(v,&start);
    InsertPoint(v,&last);
    MyVertex next=findNext(&start,&last);
    while(!(next==start))
    {
        InsertPoint(v,&next);
        MyVertex temp=findNext(&last,&next);
        last=next;
        next=temp;
    }
}

MyVertex findNext(MyVertex* p0,MyVertex *p1)
{
    for(unsigned int i=0;i<table.size();i++)
    {
        if(table[i][0]==*p1 && !(table[i][1]==*p0))
            return table[i][1];
        else if(table[i][1]==*p1 && !(table[i][0]==*p0))
            return table[i][0];
    }
}
