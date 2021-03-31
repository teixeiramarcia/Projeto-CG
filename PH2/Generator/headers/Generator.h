#pragma once
#include <string>
#include <vector>

//#define PATH "../Visualizator/xml/"
#define PATH "./"

class Generator
{
    public:
        std::string* figura;
        std::vector<double> argsFig;
        std::string* fileName;
        bool isValid;

        Generator(int argc,char** argv);
        ~Generator();
        std::string modelo();
        void writeModelo();
};
