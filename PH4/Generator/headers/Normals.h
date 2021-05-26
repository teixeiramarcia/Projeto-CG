#pragma once
#include "./Triangle.h"
#include <vector>
#include <unordered_map>
#include <cmath>

class Normals
{
    private:
        static Point3D getVector(Point3D& p1,Point3D& p2);
        static double normaVector(Point3D& vec);
        static Point3D getNormalOf2Vector(Point3D& v1,Point3D& v2);
        static Point3D triangleNormalVector(Triangle& t);
        static Point3D vertexNormal(Point3D& vertex,std::vector<Triangle>& figure,std::vector<Point3D>& triangleNormals);

    public:
        static std::vector<Point3D> getNormals(std::vector<Triangle>& figure);
        static void toString(std::vector<Triangle>& figure);
};
