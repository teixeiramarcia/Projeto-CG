#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include "../headers/Anel.h"
using namespace std;


Anel::Anel(float raio, int slices, int stacks, float outerR){
    this->raio = raio;
    this->outerR = outerR;
    this->slices = slices;
    this->stacks = stacks;
}

Anel::~Anel(){}

//Construção do Anel
vector<PTriangle> Anel::desenhaAnel(){
    vector<PTriangle> estrutura;
    float raio = this->raio;
    float outerR = this->outerR;
    int slices = this->slices;
    int stacks = this->stacks;
    //Face lateral
    for(int i = 0; i < stacks; i++){
        float angulo2 = (2*M_PI/stacks) * i;
        float altura = (sin(angulo2) * raio)/20;
        float altura1 = (sin(angulo2 + (2*M_PI/stacks)) * raio)/20;
        float raio1 = (sin(angulo2) * raio) - outerR;
        float raio2 = (sin(angulo2 + (2*M_PI/stacks)) * raio) - outerR;
        for(int j = 0; j < slices; j++){
            float angulo = (2*M_PI/slices) * j;
            float catadj = cos(angulo) * raio1;
            float catopt = sin(angulo) * raio1;
            float nextcatadj = cos(angulo + (2*M_PI/slices)) * raio1;
            float nextcatopt = sin(angulo + (2*M_PI/slices)) * raio1;
            float cadj = cos(angulo) * raio2;
            float copt = sin(angulo) * raio2;
            float nextcadj = cos(angulo + (2*M_PI/slices)) * raio2;
            float nextcopt = sin(angulo + (2*M_PI/slices)) * raio2;

            estrutura.push_back(new Triangle(catadj, altura, catopt, nextcadj, altura1, nextcopt, nextcatadj, altura, nextcatopt));

            estrutura.push_back(new Triangle(catadj, altura, catopt, cadj, altura1, copt, nextcadj, altura1, nextcopt));
        }
    }
    /*for(int i = 0; i > - stacks/2; i--){
        float angulo2 = (2*M_PI/stacks) * i;
        float altura = (sin(angulo2) * raio)/5;
        float altura1 = (sin(angulo2 + (2*M_PI/stacks)) * raio)/5;
        float raio1 = (sin(angulo2) * raio) - outerR;
        float raio2 = (sin(angulo2 + (2*M_PI/stacks)) * raio) - outerR;
        for(int j = 0; j < slices; j++){
            float angulo = (2*M_PI/slices) * j;
            float catadj = cos(angulo) * raio1;
            float catopt = sin(angulo) * raio1;
            float nextcatadj = cos(angulo + (2*M_PI/slices)) * raio1;
            float nextcatopt = sin(angulo + (2*M_PI/slices)) * raio1;
            float cadj = cos(angulo) * raio2;
            float copt = sin(angulo) * raio2;
            float nextcadj = cos(angulo + (2*M_PI/slices)) * raio2;
            float nextcopt = sin(angulo + (2*M_PI/slices)) * raio2;

            estrutura.push_back(new Triangle(catadj, altura, catopt, nextcadj, altura1, nextcopt, nextcatadj, altura, nextcatopt));

            estrutura.push_back(new Triangle(catadj, altura, catopt, cadj, altura1, copt, nextcadj, altura1, nextcopt));
        }
    }*/
    return estrutura;
}

void Anel::freeTriangles(vector<PTriangle>& vec){
    for(auto t : vec)
        delete(t);
    vec.clear();
}

string Anel::toString(){
    vector<PTriangle> triangles = this->desenhaAnel();
    string s;

    int nPontos = triangles.size() * 3;
    s.append(to_string(nPontos)+"\n");
    for(auto triangle : triangles)
        s.append(triangle->toString());

    this->freeTriangles(triangles);

    return s;
}