#pragma once
#include <string>

typedef class Point3D
{
    public:
        double x,y,z;

        Point3D();
        Point3D(double x,double y,double z);
        Point3D(double v);
        Point3D(Point3D *p);
        Point3D(const Point3D &p);
        ~Point3D();

        Point3D operator+(const Point3D &p);

        template<typename T>
        Point3D operator*(const T &n);


        std::string toString();

}* PPoint3D;

template<>
Point3D Point3D::operator*<Point3D>(const Point3D &p);
