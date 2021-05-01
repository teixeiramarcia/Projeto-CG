#pragma once
#include<vector>
#include"Point3D.h"
#include"Triangle.h"
#include<string>
#include<fstream>
#include<iostream>

typedef class Surface
{
    private:
        int tesselation;
        std::vector<std::vector<std::vector<Point3D>>> patchesCtrlP;

        template<typename T>
            std::vector<std::vector<T>> multiplyMatrices(const std::vector<std::vector<T>> &m1, const std::vector<std::vector<T>> &m2);
        Point3D bezierPatch(const double u,const double v,const std::vector<std::vector<Point3D>> &controlPoints);

        std::vector<Triangle> getTrianglesIn2Vectors(const std::vector<Point3D>&v1,const std::vector<Point3D> &v2);
        std::vector<std::vector<Point3D>> calculatePatchPoints(const std::vector<std::vector<Point3D>> &controlPoints);
        std::vector<Triangle> getPatchTriangles(const std::vector<std::vector<Point3D>> &controlPoints);

        std::vector<std::vector<Point3D>> getPatchMatrix(const std::vector<int> &indices,const std::vector<Point3D> &pontosControlo);
        void readPatchFile(std::string filename);

    public:
        Surface(const std::string &filename);
        ~Surface();
        std::vector<Triangle> getSurfaceTriangles();
        std::string toString();

}* PSurface;
