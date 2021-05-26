#pragma once
#include <string>

typedef class Point2D {
public:
    double x,y;

    Point2D();
    Point2D(double x,double y);
    Point2D(Point2D *p);
    Point2D(const Point2D &p);
    ~Point2D();

    std::string toStringPonto();

}* PPoint2D;
