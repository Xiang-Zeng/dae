#include "myvertex.h"

MyVertex::MyVertex()
{
    tol=1.0e-4;
}

MyVertex::MyVertex(SimpleCOLLADA::Vertex *v)
{
    this->x=v->x;
    this->y=v->y;
    this->z=v->z;
    tol=1.0e-4;
}

bool MyVertex::operator==(const MyVertex &a)
{
    double sum=fabs(this->x-a.x)+fabs(this->y-a.y)+fabs(this->z-a.z);
    return (sum<=tol);
}
