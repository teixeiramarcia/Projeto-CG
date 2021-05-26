#pragma once
#include <vector>
#include <string>
#include "./Point2D.h"

typedef class TriangleTexture
{
private:
    std::vector<PPoint2D> points; //ordem de desenho index 0->1->2

public:
    TriangleTexture(double x1,double y1,double x2,double y2,double x3,double y3);
    TriangleTexture(PPoint2D p1,PPoint2D p2,PPoint2D p3);
    TriangleTexture(const Point2D &p1,const Point2D &p2, const Point2D &p3);
    TriangleTexture(TriangleTexture* t);
    TriangleTexture(const TriangleTexture &t);
    ~TriangleTexture();

    PPoint2D getPoint(int i);
    std::vector<PPoint2D> getPoints() const;
    void setPoint(int i,PPoint2D p);
    bool contains(Point2D& p) const;
    std::string toString();

}* PTriangleTexture;
