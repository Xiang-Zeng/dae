/*---------------
Copyright (c) 2017 Xiang Zeng

-----------------*/

#include <iostream>
#include "geom.h"
#include "myvertex.h"
#include "output.h"
using namespace std;


int main(int argc, char *argv[])
{
    Geom * geom=new Geom();
    Output * out= new Output();
    int count=0;

    string path_list="list.txt";
    ifstream fin(path_list.c_str());
    if(!fin.good())
    {
        cout<<"Failed to open file "<<path_list.c_str()<<"!\n";
        exit(1);
    }
    while(!fin.eof())
    {
        string path="";
        getline(fin,path);
        if(path=="")
            continue;

        cout<<endl<<"Building "<<count<<" "<<path.c_str()<<endl;
        vector<MyVertex> plan2d;    //水平面切建筑得到的平面多边形
        geom->GetFootprint(plan2d,path);

        out->oall<<"Building "<<count<<"\n";
        out->WriteAll(geom->nodes);
        out->WritePoint(&plan2d,count);
        out->otable<<"Building "<<count<<"\n";
        out->WriteTable(&geom->table);
        count++;
    }

    cout<<"Done!";
    delete geom;
    delete out;
    return 0;
}


