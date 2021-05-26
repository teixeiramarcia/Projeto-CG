#include "../headers/Point3D.h"

Point3D::Point3D()
{
    this->x = this->y = this->z = 0;
}

Point3D::Point3D(double x,double y,double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Point3D::Point3D(double v)
{
    this->x = this->y = this->z = v;
}

Point3D::Point3D(Point3D *p)
{
    this->x = p->x;
    this->y = p->y;
    this->z = p->z;
}

Point3D::Point3D(const Point3D &p)
{
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
}

Point3D::~Point3D(){}

std::string Point3D::toString()
{
    std::string s;

    s.append( std::to_string(this->x) + " "
            + std::to_string(this->y) + " "
            + std::to_string(this->z) );

    return s;
}

template<typename T>
Point3D Point3D::operator*(const T &v)
{
    return Point3D(
        this->x * v,
        this->y * v,
        this->z * v
    );
}

template<>
Point3D Point3D::operator*<Point3D>(const Point3D &p)
{
    return Point3D(
        this->x * p.x,
        this->y * p.y,
        this->z * p.z
    );
}

template<>
Point3D Point3D::operator*<int>(const int &p)
{
    return Point3D(
        this->x * p,
        this->y * p,
        this->z * p
    );
}

template<>
Point3D Point3D::operator*<double>(const double &p)
{
    return Point3D(
        this->x * p,
        this->y * p,
        this->z * p
    );
}

Point3D Point3D::operator+(const Point3D &p)
{
    return Point3D(
        this->x + p.x,
        this->y + p.y,
        this->z + p.z
    );
}
