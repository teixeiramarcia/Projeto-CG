#include "../headers/Figuras.h"
#include "../headers/Esfera.h"

using namespace std;

string Figuras::plane(double x,double z)
{
    string s;
    // inserir codigo aqui

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