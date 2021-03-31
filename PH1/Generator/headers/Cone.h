#pragma once
#include <string>
#include <math.h>
#include "./Point3D.h"
#include "./Triangle.h"

typedef class Cone{
    private:
        float raio, altura;
        int slices,stacks;

        void freeTriangles(std::vector<PTriangle>& vec);

    public:
        Cone(float raio, float altura, int slices, int stacks);
        ~Cone();

        std::vector<PTriangle> desenhaCone();
        std::string toString();

}* PCone;