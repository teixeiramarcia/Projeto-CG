#pragma once
#include <string>

typedef class Point3D
{
    public:
        double x,y,z;

        Point3D();
        Point3D(double x,double y,double z);
        Point3D(Point3D *p);
        ~Point3D();

        std::string toString();

}* PPoint3D;