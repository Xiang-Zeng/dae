/*---------------
Copyright (c) 2017 Xiang Zeng

-----------------*/

#include <iostream>
#include "lib/SimpleCOLLADA.hpp"
#include <vector>

using namespace std;
using namespace SimpleCOLLADA;

//按照标高z，把三个点从小到大排序
void sort(Vertex * p0,Vertex * p1,Vertex * p2);

//用标高为z的水平面切一个三角形，得到的交点推入v的末端。返回交点个数
int Cut(Vertex * p0,Vertex * p1,Vertex * p2,
        double z, vector<Vertex> &v);

//将点p0推入v的末端。如果p0与v的最后两点在同一直线，则不推入。
void InsertPoint(vector<Vertex> &v, Vertex *p0);

//获取线段和给定标高z的交点
Vertex * GetIntersect(Vertex * p0, Vertex * p1, double z);

int main(int argc, char *argv[])
{
    vector<Vertex> plan2d;    //水平面切建筑得到的平面多边形
    plan2d.clear();
    plan2d.reserve(4);
    const double elevation=1.0; //切建筑的水平面的标高

    Model model("san.dae");
    ModelNode *node=model.modelNodes[0];

    int not=node->getNumOfTriangles();
    for(int i=0;i<not;i++)
    {
        Triangle * triangle = node->getTriangle(i);
        Vertex * vet0=node->getVertex(triangle->position[0]);
        Vertex * vet1=node->getVertex(triangle->position[1]);
        Vertex * vet2=node->getVertex(triangle->position[2]);
        //sort(vet0,vet1,vet2);
        int nop=Cut(vet0,vet1,vet2,elevation,plan2d);
        cout<<nop<<"\n";
    }

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

int Cut(Vertex * p0,Vertex * p1,Vertex * p2,
        double z, vector<Vertex> &v)
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
        InsertPoint(v,p0);
        if(p1->z==z)
        {
            InsertPoint(v,p1);
            return 2;
        }
        return 1;
    }
    else if (p1->z==z)
    {
        InsertPoint(v,p1);
        if(p2->z==z)     InsertPoint(v,p2);
        else  InsertPoint(v,GetIntersect(p0,p2,z));
        return 2;
    }
    else if (p2->z==z)
    {
        InsertPoint(v,p2);
        return 2;
    }
    else if( p0->z<z&&z<p1->z)
    {
        InsertPoint(v,GetIntersect(p0,p1,z));
        InsertPoint(v,GetIntersect(p0,p2,z));
        return 2;
    }
    else    // p1->z < z && z < p2->z
    {
        InsertPoint(v,GetIntersect(p1,p2,z));
        InsertPoint(v,GetIntersect(p0,p2,z));
        return 2;
    }
}

void InsertPoint(vector<Vertex> &v, Vertex *p0)
{
    double tol=1.0e-5;
    int size=v.size();
    if(size<2)  v.push_back(*p0);
    else
    {
        Vertex p1= v[size-2];
        Vertex p2= v[size-1];
        double value=(p1.y-p0->y)*(p2.x-p1.x)-(p1.x-p0->x)*(p2.y-p1.y);
        if(fabs(value)>tol)
            v.push_back(*p0);
    }
}

Vertex * GetIntersect(Vertex * p0, Vertex * p1, double z)
{
    Vertex * p = new Vertex;

    return p;
}
