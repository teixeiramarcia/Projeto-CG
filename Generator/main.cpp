#include <iostream>
#include <fstream>
#include <string>
#include "headers/Generator.h"

#define XML "configuration.xml"

using namespace std;

int main(int argc,char** argv)
{
    Generator generator(argc,argv);

    // pontos modelo
    string model = generator.modelo();

    ofstream modelFile(*generator.fileName);
    ofstream xmlFile(XML);

    // escrita ficheiro .3d
    modelFile << model;
    modelFile.close();
    model.clear();
    cout << "Ficheiro gerado: " << *generator.fileName << endl;

    //escrita XML

    return 1;
}