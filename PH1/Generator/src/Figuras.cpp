#include "../headers/Figuras.h"
#include "../headers/Esfera.h"
#include "../headers/Cone.h"

using namespace std;

string Figuras::plane(double x)
{
    string s;
    float h = 0.0f;          //todos os pontos terao y == 0
    float lado = x / 2;      //uma vez que o plano está centrado na origem

    //escrever número de pontos no ficheiro:
    s.append(to_string(6) + "\n");      //todos os planos sao compostos por 6 pontos (2 triangulos)

    s.append(to_string(lado) + " " + to_string(h) + " " + to_string(lado) + "\n");
    s.append(to_string(lado) + " " + to_string(h) + " " + to_string(-lado) + "\n");
    s.append(to_string(-lado) + " " + to_string(h) + " " + to_string(lado) + "\n");

    s.append(to_string(lado) + " " + to_string(h) + " " + to_string(-lado) + "\n");
    s.append(to_string(-lado) + " " + to_string(h) + " " + to_string(-lado) + "\n");
    s.append(to_string(-lado) + " " + to_string(h) + " " + to_string(lado) + "\n");

    // return value
    return s;
}


string Figuras::box(double x,double y,double z,int divisions)
{
    string s;
    int nTriangulos, nPontos = -1;
    
    if (divisions > 1) {
        //se existem divisões (é opcional):
        nTriangulos = (6 * divisions^2) * 2;   //número de triângulos
        nPontos = nTriangulos * 3;    //número de pontos
    }
    else {
        //caso não tenham sido pedidas divisões:
        nPontos = 36;
    }
    //escrever no ficheiro o número de pontos:
    s.append(to_string(nPontos) + "\n"); 

    //uma vez que a caixa vai estar centrada na origem:
    float hMax = y / 2;
    float xLado = x / 2;
    float zLado = z / 2;
    
    //caso não existam divisões (divisions = 0 ou 1):
    if (divisions <= 1) {
        //anotações, tendo como referência a representação habitual dos eixos: 
        //calculo e escrita dos pontos dos triângulos da tampa (ordem mão direita, y fixo):
        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");

        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");

        //calculo e escrita dos pontos dos triângulos da base (ordem mão esquerda):
        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");

        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");


        //calculo e escrita dos pontos dos triângulos das faces laterais:
        //1ª face ("frente", z fixo:
        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");

        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");

        //2ª fase ("traseira", z fixo):
        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");

        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");

        //3ª fase ("direita", x fixo):
        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");

        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");

        //4ª fase ("esquerda", x fixo):
        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");

        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(-zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(-hMax) + " " + to_string(zLado) + "\n");
        s.append(to_string(-xLado) + " " + to_string(hMax) + " " + to_string(zLado) + "\n");
    }
    else {
        //caso tenham sido requeridas divisões:
        float xAtual, yAtual, zAtual;

        //anotações, tendo como referência a representação habitual dos eixos: 
        for (int i = 0; i < divisions; i++) {
            float xAtual = (-x / 2) + i * (x / divisions);
            float xAtualSup = (-x / 2) + (i + 1) * (x / divisions);
            float zAtualEsqDir = (-z / 2) + i * (z / divisions);   //no caso da face esquerda e direita, a variação em z, necessita de estar
            float zAtualSupEsqDir = (-z / 2) + (i + 1) * (z / divisions); //num ciclo diferente da variação em y, que se encontra no ciclo mais "interior"
            for (int j = 0; j < divisions; j++) {
                float zAtual = (-z / 2) + j * (z / divisions);
                float zAtualSup = (-z / 2) + (j + 1) * (z / divisions);
                float yAtual = (-y / 2) + j * (y / divisions);
                float yAtualSup = (-y / 2) + (j + 1) * (y / divisions);

                //tampa:
                s.append(to_string(xAtual) + " " + to_string(hMax) + " " + to_string(zAtual) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(hMax) + " " + to_string(zAtualSup) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(hMax) + " " + to_string(zAtual) + "\n");

                s.append(to_string(xAtual) + " " + to_string(hMax) + " " + to_string(zAtual) + "\n");
                s.append(to_string(xAtual) + " " + to_string(hMax) + " " + to_string(zAtualSup) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(hMax) + " " + to_string(zAtualSup) + "\n");

                //base:
                s.append(to_string(xAtual) + " " + to_string(-hMax) + " " + to_string(zAtual) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(-hMax) + " " + to_string(zAtual) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(-hMax) + " " + to_string(zAtualSup) + "\n");

                s.append(to_string(xAtual) + " " + to_string(-hMax) + " " + to_string(zAtual) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(-hMax) + " " + to_string(zAtualSup) + "\n");
                s.append(to_string(xAtual) + " " + to_string(-hMax) + " " + to_string(zAtualSup) + "\n");

                //face frente:
                s.append(to_string(xAtual) + " " + to_string(yAtual) + " " + to_string(zLado) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(yAtual) + " " + to_string(zLado) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(yAtualSup) + " " + to_string(zLado) + "\n");

                s.append(to_string(xAtual) + " " + to_string(yAtual) + " " + to_string(zLado) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(yAtualSup) + " " + to_string(zLado) + "\n");
                s.append(to_string(xAtual) + " " + to_string(yAtualSup) + " " + to_string(zLado) + "\n");

                //face trás:
                s.append(to_string(xAtual) + " " + to_string(yAtual) + " " + to_string(-zLado) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(yAtualSup) + " " + to_string(-zLado) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(yAtual) + " " + to_string(-zLado) + "\n");

                s.append(to_string(xAtual) + " " + to_string(yAtual) + " " + to_string(-zLado) + "\n");
                s.append(to_string(xAtual) + " " + to_string(yAtualSup) + " " + to_string(-zLado) + "\n");
                s.append(to_string(xAtualSup) + " " + to_string(yAtualSup) + " " + to_string(-zLado) + "\n");

                //face direita:
                s.append(to_string(xLado) + " " + to_string(yAtual) + " " + to_string(zAtualEsqDir) + "\n");
                s.append(to_string(xLado) + " " + to_string(yAtualSup) + " " + to_string(zAtualSupEsqDir) + "\n");
                s.append(to_string(xLado) + " " + to_string(yAtual) + " " + to_string(zAtualSupEsqDir) + "\n");

                s.append(to_string(xLado) + " " + to_string(yAtual) + " " + to_string(zAtualEsqDir) + "\n");
                s.append(to_string(xLado) + " " + to_string(yAtualSup) + " " + to_string(zAtualEsqDir) + "\n");
                s.append(to_string(xLado) + " " + to_string(yAtualSup) + " " + to_string(zAtualSupEsqDir) + "\n");

                //face esquerda:
                s.append(to_string(-xLado) + " " + to_string(yAtual) + " " + to_string(zAtualEsqDir) + "\n");
                s.append(to_string(-xLado) + " " + to_string(yAtual) + " " + to_string(zAtualSupEsqDir) + "\n");
                s.append(to_string(-xLado) + " " + to_string(yAtualSup) + " " + to_string(zAtualSupEsqDir) + "\n");

                s.append(to_string(-xLado) + " " + to_string(yAtual) + " " + to_string(zAtualEsqDir) + "\n");
                s.append(to_string(-xLado) + " " + to_string(yAtualSup) + " " + to_string(zAtualSupEsqDir) + "\n");
                s.append(to_string(-xLado) + " " + to_string(yAtualSup) + " " + to_string(zAtualEsqDir) + "\n");
            }
        }
    }

    // return value
    return s;
}

string Figuras::esfera(double radius,int slices,int stacks)
{
    Esfera esfera(radius,slices,stacks);
    return esfera.toString(); 
}

string Figuras::cone(double radius, double height, int slices, int stacks){
    Cone cone(radius, height, slices, stacks);
    return cone.toString(); 
}