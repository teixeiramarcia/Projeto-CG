#pragma once
#include <string>

class Figuras
{
    public:
        static std::string plane(double x,double z);
        static std::string box(double x,double y,double z,int divisions);
        static std::string esfera(double radius,int slices,int stacks);
        static std::string cone(double radius,double height,int slices,int stacks);
};