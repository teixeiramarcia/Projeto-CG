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

vector<PPoint2D>* Esfera::getYoZStackInterceptionPointsTexture()
{
    vector<PPoint2D>* points = new vector<PPoint2D>();
    const double x = 0;
    int stc = this->stacks;
    double y = 1;
    //int sections = this->stacks+1;
    const double gap = (double) 1 / (stc+1);
    PPoint2D point;

    for(int nPoints=stc+2 ;nPoints>0;nPoints--)
    {
        point = new Point2D(x,y);
        points->push_back(point);
        y-=gap;
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

PPoint2D Esfera::rotatePointTexture_Yaxis(PPoint2D p, double dif) //angulo desde origem e ponto no plano YoZ
{
    //double radius = p->z;
    double x = p->x + dif;
    //double z = cos(angle)*radius;

    return new Point2D(x,p->y);
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

vector<vector<PPoint2D>*> Esfera::getVerticalLinePointsTexture()
{
    vector<vector<PPoint2D>*> slicePoints;
    vector<PPoint2D>* sliceYoZ = getYoZStackInterceptionPointsTexture();
    vector<PPoint2D>* slice;
    PPoint2D point;
    int nVerticalLines = this->slices;
    double sliceAngle = (double)1/nVerticalLines;

    // primeira linha
    slicePoints.push_back(sliceYoZ);   //certo!!!

    // outras linhas
    for(int i=1; nVerticalLines-1>0 ; i++,nVerticalLines--)
    {
        slice = new vector<PPoint2D>();
        //slice->push_back(new Point2D(1,1)); // primeiro ponto nao sofre rotacao
        slice->push_back(new Point2D((*sliceYoZ)[0])); // primeiro ponto nao sofre rotacao

        /*for(int j=1;j<sliceYoZ->size()-1;j++) // rotacao de todos os outros
        {
            point = this->rotatePoint_Yaxis((*sliceYoZ)[j],sliceAngle*i);
            slice->push_back(point);
        }*/
        for(int j=1;j<sliceYoZ->size()-1;j++){
            point = this->rotatePointTexture_Yaxis((*sliceYoZ)[j], sliceAngle*i);
            slice->push_back(point);
        }

        //slice->push_back(new Point2D(0,0)); // ultimo ponto nao sofre rotacao
        slice->push_back(new Point2D((*sliceYoZ)[sliceYoZ->size()-1])); // ultimo ponto nao sofre rotacao
        slicePoints.push_back(slice);
    }

    // ultima linha=primeira
    slice = new vector<PPoint2D>();
    for(auto ppoint: *sliceYoZ)
        slice->push_back(new Point2D(ppoint));
    slicePoints.push_back(slice);

    return slicePoints;
}

vector<Triangle> Esfera::getSliceTriangles(vector<PPoint3D>* s1,vector<PPoint3D>* s2)
{
    vector<Triangle> triangles;
    int size = s1->size();
    int sections = size-1;

    // 1º triangulo topo da fatia
    triangles.push_back(Triangle((*s1)[0],(*s1)[1],(*s2)[1]));

    //decomposicao quadrados da fatia em triangulos
    for(int i=1;i<size-2;i++)
    {
        // triangulo top left
        Triangle topLeft((*s1)[i],(*s1)[i+1],(*s2)[i]);
        triangles.push_back(topLeft);

        //triangulo bot right
        Triangle botRight((*s1)[i+1],(*s2)[i+1],(*s2)[i]);
        triangles.push_back(botRight);
    }

    // ultimo triangulo fatia
    triangles.push_back(Triangle((*s1)[size-1],(*s2)[size-2],(*s1)[size-2]));

    return triangles;
}

vector<TriangleTexture> Esfera::getSliceTrianglesTexture(vector<PPoint2D>* s1,vector<PPoint2D>* s2)
{
    vector<TriangleTexture> triangles;
    int size = s1->size();
    int sections = size-1;

    // 1º triangulo topo da fatia
    triangles.push_back(TriangleTexture((*s1)[0],(*s1)[1],(*s2)[1]));

    //decomposicao quadrados da fatia em triangulos
    for(int i=1;i<size-2;i++)
    {
        // triangulo top left
        TriangleTexture topLeft((*s1)[i],(*s1)[i+1],(*s2)[i]);
        triangles.push_back(topLeft);

        //triangulo bot right
        TriangleTexture botRight((*s1)[i+1],(*s2)[i+1],(*s2)[i]);
        triangles.push_back(botRight);
    }

    // ultimo triangulo fatia
    triangles.push_back(TriangleTexture((*s1)[size-1],(*s2)[size-2],(*s1)[size-2]));

    return triangles;
}

vector<Triangle> Esfera::getTriangles()
{
    vector<vector<PPoint3D>*> slicesPoints = this->getVerticalLinePoints();
    vector<Triangle> triangles;
    int size = slicesPoints.size();

    for(int i=0;i<size-1;i++) // -1 nao faz para ultima slize -> igual á primeira
    {
        vector<Triangle> slice = getSliceTriangles(slicesPoints[i],slicesPoints[i+1]);
        triangles.insert(triangles.end(),slice.begin(),slice.end());
    }

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

vector<TriangleTexture> Esfera::getTrianglesTexture()
{
    vector<vector<PPoint2D>*> slicesPoints = this->getVerticalLinePointsTexture();
    vector<TriangleTexture> triangles;
    int size = slicesPoints.size();

    for(int i=0;i<size-1;i++) { // -1 nao faz para ultima slize -> igual á primeira
        vector<TriangleTexture> slice = getSliceTrianglesTexture(slicesPoints[i], slicesPoints[i + 1]);
        triangles.insert(triangles.end(), slice.begin(), slice.end());
    }

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

string Esfera::toString()
{
    vector<Triangle> triangles = this->getTriangles();
    string s;
    int nSlicePieces = triangles.size();
    int npontos = triangles.size()*3;

    s.append(to_string(npontos)+"\n");

    for(auto triang: triangles)
        s.append(triang.toString());

    s.append( Normals::toString(triangles) );

    vector<TriangleTexture> trianglesTexture = this->getTrianglesTexture();
    int nSlicesPiecesTexture = trianglesTexture.size();
    int nPontosTexture = trianglesTexture.size()*3;

    s.append(to_string(nPontosTexture)+"\n");

    for(auto triangText : trianglesTexture)
        s.append(triangText.toString());


    return s;
}
