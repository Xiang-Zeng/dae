#include "output.h"

Output::Output()
{
    oall.open("results/all.txt");
    otable.open("results/table.txt");
    opoint.open("results/Point.txt");
}

Output::~Output()
{
    oall.close();
    otable.close();
    opoint.close();
}

void Output::WriteAll(vector<MyVertex> *v)
{
    int nov=v->size();
    for (int i=0;i<nov;i++)
    {
        oall<<fixed<<setprecision(2)<<(*v)[i].x<<"\t"<<(*v)[i].y<<endl;
    }
}

void Output::WritePoint(vector<MyVertex> *v,int count)
{
    opoint<<v->size()<<"\n";
    for(int i=0;i<v->size();i++)
        opoint<<count<<"\t"<<fixed<<setprecision(2)<<(*v)[i].x<<"\t"<<(*v)[i].y<<"\n";
}

void Output::WriteTable(vector<vector<MyVertex>> *table)
{
    for(unsigned int i=0;i<table->size();i++)
    {
        for(int j=0;j<2;j++)
        {
            otable<<fixed<<setprecision(2)<<(*table)[i][j].x<<"\t"<<(*table)[i][j].y<<"\t"<<"\t";
        }
        otable<<endl;
    }
}

