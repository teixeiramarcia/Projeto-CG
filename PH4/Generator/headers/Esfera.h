#pragma once
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "./Point3D.h"
#include "./Triangle.h"

typedef class Esfera
{
    private:
        double radius;
        int slices, stacks;

        std::vector<PPoint3D>* getYoZStackInterceptionPoints();
        PPoint3D rotatePoint_Yaxis(PPoint3D p,double angle);
        std::vector<std::vector<PPoint3D>*> getVerticalLinePoints();
        std::vector<Triangle> getSliceTriangles(std::vector<PPoint3D>* s1,std::vector<PPoint3D>* s2);

    public:
        Esfera(double r,int slices,int stacks);
        ~Esfera();

        double getRadius();
        int getSlices();
        int getStacks();
        double getZCoord(double x,double y);
        std::vector<Triangle> getTriangles();
        std::string toString();

}* PEsfera;
