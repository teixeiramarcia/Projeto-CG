#include "../headers/Generator.h"
#include "../headers/Figuras.h"
#include <iostream>
#include <fstream>

using namespace std;

Generator::Generator(int argc,char** argv)
{
    int nArgsFig;
    
    this->isValid = false;
    
    if(argc<=3)
        return;
    
    this->figura = new string(argv[1]);
    
    // figura e/ou numero argumentos invalidos
    if(!(*this->figura=="plane" && argc==4 || *this->figura=="box" && argc==7 || *this->figura=="sphere" && argc==6
        || *this->figura=="cone" && argc==7))
    {
        cerr << "Invalid figure or number of arguments\n";
        exit(1);
    }
    
    string name(argv[argc-1]);
    this->fileName = new string(PATH+name);
    
    if(*this->figura=="plane")
        nArgsFig = 1;
    else if(*this->figura=="box")
        nArgsFig = 4;
    else if(*this->figura=="sphere")
        nArgsFig = 3;
    else
        nArgsFig = 4;
    
    for(int i=2; nArgsFig>0; i++,nArgsFig--)
    {
        double n;
        try{
            string s(argv[i]);
            n = stod(s);
        }
        catch(const invalid_argument& error){
            cerr << "Invalid figure argument: " << error.what() << '\n';
            exit(1);
        }
        argsFig.push_back(n);
    }
    
    this->isValid = true;            
}

Generator::~Generator()
{
    figura->clear();
    fileName->clear();
    argsFig.clear();
}

string Generator::modelo()
{
    string model;

    if(!this->isValid)
    {
        cerr << "Comando Invalido\n";
        exit(1);
    }

    if(*this->figura=="plane")
    {
        double x = this->argsFig[0];

        model = Figuras::plane(x);
    }
    else if(*this->figura=="box")
    {
        double x=this->argsFig[0];
        double y=this->argsFig[1];
        double z=this->argsFig[2];
        int divisions=this->argsFig[3];

        model = Figuras::box(x,y,z,divisions);
    }
    else if(*this->figura=="sphere")
    {
        double radius = this->argsFig[0];
        int slices = this->argsFig[1];
        int stacks = this->argsFig[2];
        
        model = Figuras::esfera(radius,slices,stacks);
    }
    else
    {
        double radius = this->argsFig[0];
        double height = this->argsFig[1];
        int slices = this->argsFig[2];
        int stacks = this->argsFig[3];

        model = Figuras::cone(radius,height,slices,stacks);
    }

    return model;
}

void Generator::writeModelo()
{
    ofstream out(*this->fileName);
    string model = this->modelo();

    out << model;
    out.close();
    model.clear();
}