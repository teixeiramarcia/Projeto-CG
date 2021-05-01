#pragma once
#include <vector>
#include <string>
#include "./Point3D.h"

typedef class Triangle
{
    private:
        std::vector<PPoint3D> points; //ordem de desenho index 0->1->2

    public:
        Triangle(double x1,double y1,double z1,double x2,double y2,double z2,double x3,double y3,double z3);
        Triangle(PPoint3D p1,PPoint3D p2,PPoint3D p3);
        Triangle(const Point3D &p1,const Point3D &p2, const Point3D &p3);
        Triangle(Triangle* t);
        Triangle(const Triangle &t);
        ~Triangle();

        PPoint3D getPoint(int i);
        std::vector<PPoint3D> getPoints() const;
        void setPoint(int i,PPoint3D p);
        std::string toString();

}* PTriangle;
