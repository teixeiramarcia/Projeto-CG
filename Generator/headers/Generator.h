#pragma once
#include <string>
#include <vector>

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
};
