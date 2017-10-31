#include "geom.h"

Geom::Geom()
{
    nodes = new vector<MyVertex>;
}

Geom::~Geom()
{
    delete nodes;
}

void Geom::GetFootprint(vector<MyVertex> &plan2d,string path)
{
    table.clear();
    plan2d.clear();
    plan2d.reserve(4);
    Model model(path);
    ModelNode *node=model.modelNodes[0];
    table.clear();
    nodes->clear();
    double minz=99999999999;
    double maxz=-9999999999;
    int nov=node->getNumOfVertices();
    for (int i=0;i<nov;i++)
    {
        nodes->push_back(node->getVertex(i));
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
        //cout<<nop<<"\t";
    }

    connect(plan2d);
    if(plan2d.size()<3)
    {
        MyVertex a(0,-120000,0),b(1,-120000,0),c(0,-120001,0);
        plan2d.clear();
        plan2d.push_back(a);
        plan2d.push_back(b);
        plan2d.push_back(c);
    }
}


void Geom::sort(Vertex * p0,Vertex * p1,Vertex * p2)
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

int Geom::Cut(Vertex * p0,Vertex * p1,Vertex * p2, double z)
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

void Geom::InsertPoint(vector<MyVertex> &v, MyVertex *p0)
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
        //cout<<"\t"<<cos;
        if(1-fabs(cos)<tol) //在同一直线上
            v.pop_back();
        v.push_back(*p0);
    }
}

Vertex * Geom::GetIntersect(Vertex * p0, Vertex * p1, double z)
{
    Vertex * p = new Vertex;
    double ratio=(p1->z-z)/(p1->z-p0->z);
    p->x=p1->x-ratio*(p1->x-p0->x);
    p->y=p1->y-ratio*(p1->y-p0->y);
    p->z=z;

    return p;
}

void Geom::connect(vector<MyVertex> &v)
{
    if(table.size()<6)
    {
        cout<<"Error 101: Too few intersection points!";
        return;
    }
    MyVertex start=table[0][0];
    MyVertex last=table[0][1];
    InsertPoint(v,&start);
    InsertPoint(v,&last);
    try
    {
        MyVertex next=findNext(&start,&last);
        int c=0;
        while(!(next==start))
        {
            InsertPoint(v,&next);
            MyVertex temp=findNext(&last,&next);
            last=next;
            next=temp;
            c++;
            if(c>=600)
            {
                cout<<"Error 102: Polygon not closed!";
                return;
            }
        }
    }
    catch (char* s)
    {
        cout<<s;
        return;
    }
}

MyVertex Geom::findNext(MyVertex* p0,MyVertex *p1)
{
    for(unsigned int i=0;i<table.size();i++)
    {
        if(table[i][0]==*p1 && !(table[i][1]==*p0))
            return table[i][1];
        else if(table[i][1]==*p1 && !(table[i][0]==*p0))
            return table[i][0];
    }
    if(1)
       throw "Error 100: Failed to find next point!";
}
