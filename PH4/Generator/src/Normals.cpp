#include "../headers/Normals.h"

using namespace std;

Point3D Normals::getVector(Point3D& p1,Point3D& p2)
{
    return p2+(p1*(-1));
}

double Normals::normaVector(Point3D& vec)
{
    return sqrt( vec.x*vec.x + vec.y+vec.y + vec.z+vec.z );
}

Point3D Normals::getNormalOf2Vector(Point3D& v1,Point3D& v2)
{
    double x = v1.y*v2.z-v1.z*v2.y;
    double y = v1.z*v2.x-v1.x*v2.z;
    double z = v1.x*v2.y-v1.y*v2.x;

    return Point3D(x,y,z);
}

Point3D Normals::triangleNormalVector(Triangle& t)
{
    Point3D v1 = getVector(*t.getPoint(0),*t.getPoint(1));
    Point3D v2 = getVector(*t.getPoint(0),*t.getPoint(2));
    return getNormalOf2Vector(v1,v2);
}

Point3D Normals::vertexNormal(Point3D& vertex,vector<Triangle>& figure,vector<Point3D>& triangleNormals)
{
    int count=0;
    Point3D sum(0.0);

    for(int it=0; it<figure.size(); it++)
        if(figure[it].contains(vertex))
        {
            sum = sum + triangleNormals[it];
            count++;
        }

    Point3D average(sum*(1.0/count));
    double norma = Normals::normaVector(average);
    average = average*(1.0/norma); // normalizado

    return average;
}


vector<Point3D> Normals::getNormals(vector<Triangle>& figure)
{
    vector<Point3D> triangleNormals;
    vector<Point3D> normals;
    unordered_map<string,Point3D> vertexNormalMap;

    for(auto t: figure)
        triangleNormals.push_back(Normals::triangleNormalVector(t));

    for(auto t: figure)
    {
        for(int i=0; i<3 ; i++)
        {
            Point3D p = *t.getPoint(i);
            Point3D n;

            if(vertexNormalMap.contains(p.toString()))
                n = vertexNormalMap.find(p.toString())->second;
            else
            {
                n = Normals::vertexNormal(p,figure,triangleNormals);
                vertexNormalMap.insert({p.toString(),n});
            }
            normals.push_back(n);
        }
    }
    return normals;
}

void Normals::toString(vector<Triangle>& figure)
{
    vector<Point3D> normals = Normals::getNormals(figure);
    string s;
    for(auto p: normals)
    {
        s.append(p.toString());
        s.append("\n");
    }
}
