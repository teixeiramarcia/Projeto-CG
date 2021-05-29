#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>
#include "../headers/Cone.h"
using namespace std;


Cone::Cone(float raio, float altura, int slices, int stacks){
    this->raio = raio;
	this->altura = altura;
    this->slices = slices;
    this->stacks = stacks;
}

Cone::~Cone(){}

//Construção do cone
vector<Triangle> Cone::desenhaCone(){
	vector<Triangle> estrutura;
	float raio = this->raio;
	float altura = this->altura;
    int slices = this->slices;
    int stacks = this->stacks;
    //Base do cone
	for(int i = 0; i < slices; i++){
		float angulo = (2*M_PI/slices) * i;
        float catadj = cos(angulo) * raio;
        float catopt = sin(angulo) * raio;
        float nextcatadj = cos(angulo + (2*M_PI/slices)) * raio;
        float nextcatopt = sin(angulo + (2*M_PI/slices)) * raio;
        estrutura.push_back(Triangle(0.0f, 0.0f, 0.0f, catadj, 0.0f, catopt, nextcatadj, 0.0f, nextcatopt));
    }
	//Face lateral
	for(int i = 0; i < stacks; i++){
		float raio1 = raio - ((raio/stacks) * i);
		float raio2 = raio - ((raio/stacks) * (i+1));
		float altura1 = (altura/stacks) * i;
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
			if(stacks-1 != i){
				estrutura.push_back(Triangle(catadj, altura1, catopt, nextcadj, altura1 + (altura/stacks), nextcopt, nextcatadj, altura1, nextcatopt));

				estrutura.push_back(Triangle(catadj, altura1, catopt, cadj, altura1 + (altura/stacks), copt, nextcadj, altura1 + (altura/stacks), nextcopt));
			}else{
				estrutura.push_back(Triangle(catadj, altura1, catopt, 0.0f, altura1 + (altura/stacks), 0.0f, nextcatadj, altura1, nextcatopt));
			}
		}
	}
	return estrutura;
}

string Cone::toString(){
    vector<Triangle> triangles = this->desenhaCone();
    string s;

    int nPontos = triangles.size() * 3;
    s.append(to_string(nPontos)+"\n");
    for(auto triangle : triangles)
        s.append(triangle.toString());

    s.append(Normals::toString(triangles));

    return s;
}
