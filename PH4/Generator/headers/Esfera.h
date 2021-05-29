#pragma once
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "./Point3D.h"
#include "./Triangle.h"
#include "./Point2D.h"
#include "./Normals.h"
#include "./TriangleTexture.h"

typedef class Esfera
{
    private:
        double radius;
        int slices, stacks;

        std::vector<PPoint3D>* getYoZStackInterceptionPoints();
        std::vector<PPoint2D>* getYoZStackInterceptionPointsTexture();
        PPoint3D rotatePoint_Yaxis(PPoint3D p,double angle);
        PPoint2D rotatePointTexture_Yaxis(PPoint2D p,double dif);
        std::vector<std::vector<PPoint3D>*> getVerticalLinePoints();
        std::vector<std::vector<PPoint2D>*> getVerticalLinePointsTexture();
        std::vector<Triangle> getSliceTriangles(std::vector<PPoint3D>* s1,std::vector<PPoint3D>* s2);
        std::vector<TriangleTexture> getSliceTrianglesTexture(std::vector<PPoint2D>* s1,std::vector<PPoint2D>* s2);

    public:
        Esfera(double r,int slices,int stacks);
        ~Esfera();

        double getRadius();
        int getSlices();
        int getStacks();
        double getZCoord(double x,double y);
        std::vector<Triangle> getTriangles();
        std::vector<TriangleTexture> getTrianglesTexture();
        std::string toString();

}* PEsfera;
