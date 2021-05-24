#pragma once
#include <string>
#include <math.h>
#include "./Point3D.h"
#include "./Triangle.h"

typedef class Anel{
private:
    float raio, outerR;
    int slices,stacks;

    void freeTriangles(std::vector<PTriangle>& vec);

public:
    Anel(float raio, int slices, int stacks, float outerR);
    ~Anel();

    std::vector<PTriangle> desenhaAnel();
    std::string toString();

}* PAnel;