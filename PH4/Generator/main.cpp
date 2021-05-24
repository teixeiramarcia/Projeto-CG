#include <iostream>
#include "./headers/Generator.h"

int main(int argc,char** argv)
{
    Generator generator(argc,argv);

    generator.writeModelo();

    return 1;
}
