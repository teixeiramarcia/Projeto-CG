//
// Created by filip on 24/05/2021.
//

#include "../headers/Point2D.h"

Point2D::Point2D()
{
    this->x = this->y;
}

Point2D::Point2D(double x,double y)
{
    this->x = x;
    this->y = y;
}

Point2D::Point2D(Point2D *p)
{
    this->x = p->x;
    this->y = p->y;
}

Point2D::Point2D(const Point2D &p)
{
    this->x = p.x;
    this->y = p.y;
}

Point2D::~Point2D(){}

std::string Point2D::toStringPonto()
{
    std::string s;

    s.append( std::to_string(this->x) + " "
              + std::to_string(this->y) );

    return s;
}
