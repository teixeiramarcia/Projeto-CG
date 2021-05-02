#pragma once
#include <string>

class Figuras
{
    public:
        static std::string plane(double x);
        static std::string box(double x,double y,double z,int divisions);
        static std::string esfera(double radius,int slices,int stacks);
        static std::string cone(float radius, float height, int slices, int stacks);
        static std::string anel(float radius, int slices, int stacks, float outerR);
        static std::string surface(std::string filename,int tesselation);
};
