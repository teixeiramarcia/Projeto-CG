#include "../headers/Point3D.h"

Point3D::Point3D()
{
    this->x = this->y = this->z = 0;
}

Point3D::Point3D(double x,double y,double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3D::Point3D(Point3D *p)
{
    this->x = p->x;
    this->y = p->y;
    this->z = p->z;
}

Point3D::~Point3D(){}

std::string Point3D::toString()
{
    std::string s;
    
    s.append( std::to_string(this->x) + " " 
            + std::to_string(this->y) + " " 
            + std::to_string(this->z) );
    
    return s;
}