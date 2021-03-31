#include "../headers/Esfera.h"
#include <iostream>

using namespace std;

Esfera::Esfera(double r,int slices,int stacks)
{
    this->radius = r;
    this->slices = slices;
    this->stacks = stacks;
}

Esfera::~Esfera(){}

double Esfera::getRadius(){return this->radius;}
int Esfera::getSlices(){return this->slices;}
int Esfera::getStacks(){return this->stacks;}

double Esfera::getZCoord(double x,double y)
{
    double r = this->radius;
    double arg1 = x*x;
    double arg2 = y*y;
    double arg = r*r - arg1 - arg2;
    double z = sqrt(arg);

    return arg<0.0?0.0:z;
}

vector<PPoint3D>* Esfera::getYoZStackInterceptionPoints()
{
    vector<PPoint3D>* points = new vector<PPoint3D>();
    const double x = 0;
    double y = this->radius;
    int sections = this->stacks+1;
    const double gap = (this->radius * 2.0f) / sections;
    PPoint3D point;

    for(int nPoints=this->stacks+2 ;nPoints>0;nPoints--,y-=gap)
    {
        point = new Point3D(x,y,this->getZCoord(x,y));
        points->push_back(point);
    }

    return points;
}

PPoint3D Esfera::rotatePoint_Yaxis(PPoint3D p,double angle) //angulo desde origem e ponto no plano YoZ
{
    double radius = p->z;
    double x = sin(angle)*radius;
    double z = cos(angle)*radius;
    
    return new Point3D(x,p->y,z);
}

vector<vector<PPoint3D>*> Esfera::getVerticalLinePoints()
{
    vector<vector<PPoint3D>*> slicePoints;
    vector<PPoint3D>* sliceYoZ = getYoZStackInterceptionPoints();
    vector<PPoint3D>* slice;
    PPoint3D point;
    double sliceAngle = (2*M_PI)/(this->slices);
    int nVerticalLines = this->slices;
    
    // primeira linha
    slicePoints.push_back(sliceYoZ);

    // outras linhas
    for(int i=1; nVerticalLines-1>0 ; i++,nVerticalLines--)
    {
        slice = new vector<PPoint3D>();
        slice->push_back(new Point3D((*sliceYoZ)[0])); // primeiro ponto nao sofre rotacao
        for(int j=1;j<sliceYoZ->size()-1;j++) // rotacao de todos os outros
        {
            point = this->rotatePoint_Yaxis((*sliceYoZ)[j],sliceAngle*i);
            slice->push_back(point);
        }
        slice->push_back(new Point3D((*sliceYoZ)[sliceYoZ->size()-1])); // ultimo ponto nao sofre rotacao
        slicePoints.push_back(slice);
    }

    // ultima linha=primeira
    slice = new vector<PPoint3D>();
    for(auto ppoint: *sliceYoZ)
        slice->push_back(new Point3D(ppoint));
    slicePoints.push_back(slice);

    return slicePoints;
}

vector<PTriangle>* Esfera::getSliceTriangles(vector<PPoint3D>* s1,vector<PPoint3D>* s2)
{
    vector<PTriangle>* triangles = new vector<PTriangle>();
    PTriangle triangle;
    int size = s1->size();
    int sections = size-1;

    // 1º triangulo topo da fatia
    triangle = new Triangle((*s1)[0],(*s1)[1],(*s2)[1]);
    triangles->push_back(triangle);

    //decomposicao quadrados da fatia em triangulos
    for(int i=1;i<size-2;i++)
    {
        // triangulo top left
        triangle = new Triangle((*s1)[i],(*s1)[i+1],(*s2)[i]);
        triangles->push_back(triangle);

        //triangulo bot right
        triangle = new Triangle((*s1)[i+1],(*s2)[i+1],(*s2)[i]);
        triangles->push_back(triangle);
    }

    // ultimo triangulo fatia
    triangle = new Triangle((*s1)[size-1],(*s2)[size-2],(*s1)[size-2]);
    triangles->push_back(triangle);

    return triangles;
}

vector<vector<PTriangle>*> Esfera::getTriangles()
{
    vector<vector<PPoint3D>*> slicesPoints = this->getVerticalLinePoints();
    vector<vector<PTriangle>*> triangles;
    int size = slicesPoints.size();

    for(int i=0;i<size-1;i++) // -1 nao faz para ultima slize -> igual á primeira
        triangles.push_back(getSliceTriangles(slicesPoints[i],slicesPoints[i+1]));
    
    // free slicesPoints
    for(auto slice: slicesPoints)
    {
        for(auto p: *slice)
            delete(p);
        (*slice).clear();
    }
    slicesPoints.clear();

    return triangles;
}

void Esfera::freeTriangles(vector<vector<PTriangle>*>& vec)
{
    for(auto slicePiece: vec)
    {
        for(auto t: *slicePiece)
            delete(t);
        (*slicePiece).clear();
    }

    vec.clear();
}

string Esfera::toString()
{
    vector<vector<PTriangle>*> triangles = this->getTriangles();
    string s;
    int nSlicePieces = triangles.size();
    int nTriangsSlice = triangles[0]->size();
    int npontos = nSlicePieces*(nTriangsSlice*3);

    s.append(to_string(npontos)+"\n");

    for(auto slicePiece: triangles)
        for(auto triang: *slicePiece)
            s.append(triang->toString());

    this->freeTriangles(triangles);

    return s;
}