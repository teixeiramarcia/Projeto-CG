#include "../headers/Figuras.h"
#include "../headers/Esfera.h"

using namespace std;

string Figuras::plane(double x)
{
    string s;
    float h = 0.0f;          //todos os pontos terão y == 0
    float lado = x / 2;
    // inserir codigo aqui
    s.append(to_string(6) + "\n");      //todos os planos são compostos por 6 pontos (2 triângulos)

    s.append(to_string(lado) + " " + to_string(h) + " " + to_string(lado) + "\n");
    s.append(to_string(lado) + " " + to_string(h) + " " + to_string(-lado) + "\n");
    s.append(to_string(-lado) + " " + to_string(h) + " " + to_string(lado) + "\n");

    s.append(to_string(lado) + " " + to_string(h) + " " + to_string(-lado) + "\n");
    s.append(to_string(-lado) + " " + to_string(h) + " " + to_string(-lado) + "\n");
    s.append(to_string(-lado) + " " + to_string(h) + " " + to_string(lado) + "\n");

    printf("%s\n", s.c_str());

    // return value
    return s;
}

string Figuras::box(double x,double y,double z,int divisions)
{
    string s;
    // inserir codigo aqui


    // return value
    return s;
}

string Figuras::esfera(double radius,int slices,int stacks)
{
    Esfera esfera(radius,slices,stacks);
    return esfera.toString(); 
}

string Figuras::cone(double radius,double height,int slices,int stacks)
{
    string s;
    // inserir codigo aqui

    // return value
    return s;
}