#include <iostream>
#include <fstream>
#include <string>
#include "./headers/Generator.h"

int main(int argc,char** argv)
{
    Generator generator(argc,argv);

    generator.writeModelo();

    std::cout << "Ficheiro gerado: " << *generator.fileName << std::endl;

    return 1;
}