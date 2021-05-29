#pragma once
#include <string>
#include <math.h>
#include "./Point3D.h"
#include "./Triangle.h"
#include "./Normals.h"

typedef class Anel{
private:
    float raio, outerR;
    int slices,stacks;

public:
    Anel(float raio, int slices, int stacks, float outerR);
    ~Anel();

    std::vector<Triangle> desenhaAnel();
    std::string toString();

}* PAnel;
