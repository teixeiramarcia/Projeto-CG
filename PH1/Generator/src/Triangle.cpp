#include "../headers/Triangle.h"

using namespace std;

Triangle :: Triangle(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3)
{
    points.push_back(new Point3D(x1,y1,z1));
    points.push_back(new Point3D(x2,y2,z2));
    points.push_back(new Point3D(x3,y3,z3));
}

Triangle :: Triangle(PPoint3D p1,PPoint3D p2,PPoint3D p3) 
    : Triangle(p1->x,p1->y,p1->z,p2->x,p2->y,p2->z,p3->x,p3->y,p3->z) {}

Triangle :: Triangle(Triangle* t) 
    : Triangle(t->getPoint(0),t->getPoint(1),t->getPoint(2)) {}

Triangle :: ~Triangle()
{
    for(int i=0;i<3;i++)    
        delete(this->points[i]);
}

PPoint3D Triangle::getPoint(int i)
{
    return new Point3D(this->points[i]);
}

void Triangle::setPoint(int i,PPoint3D p)
{
    this->points[i]->x = p->x;
    this->points[i]->y = p->y;
}

string Triangle::toString()
{
    string s;

    for(PPoint3D p: this->points)
        s.append( p->toString() + "\n" );
    
    return s;
}