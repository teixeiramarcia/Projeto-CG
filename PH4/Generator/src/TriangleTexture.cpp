#include "../headers/TriangleTexture.h"

using namespace std;

TriangleTexture:: TriangleTexture(double x1,double y1,double x2,double y2,double x3,double y3)
{
    points.push_back(new Point2D(x1,y1));
    points.push_back(new Point2D(x2,y2));
    points.push_back(new Point2D(x3,y3));
}

TriangleTexture :: TriangleTexture(PPoint2D p1,PPoint2D p2,PPoint2D p3)
        : TriangleTexture(p1->x,p1->y,p2->x,p2->y,p3->x,p3->y) {}

TriangleTexture :: TriangleTexture(const Point2D &p1,const Point2D &p2,const Point2D &p3)
        : TriangleTexture(p1.x,p1.y,p2.x,p2.y,p3.x,p3.y) {}

TriangleTexture :: TriangleTexture(TriangleTexture* t)
        : TriangleTexture(t->getPoint(0),t->getPoint(1),t->getPoint(2)) {}

TriangleTexture :: TriangleTexture(const TriangleTexture &t)
{
    vector<PPoint2D> points = t.getPoints();
    this->points.push_back( new Point2D( *(points[0]) ) );
    this->points.push_back( new Point2D( *(points[1]) ) );
    this->points.push_back( new Point2D( *(points[2]) ) );
}

TriangleTexture :: ~TriangleTexture()
{
    for(int i=0;i<3;i++)
        delete(this->points[i]);
}

PPoint2D TriangleTexture::getPoint(int i)
{
    return new Point2D(this->points[i]);
}

vector<PPoint2D> TriangleTexture::getPoints() const
{
    return this->points;
}

void TriangleTexture::setPoint(int i,PPoint2D p)
{
    this->points[i]->x = p->x;
    this->points[i]->y = p->y;
}

bool TriangleTexture::contains(Point2D& p) const
{
    for(auto p2: this->points)
        if(p.x==p2->x && p.y==p2->y)
            return true;
    return false;
}

string TriangleTexture::toString()
{
    string s;

    for(PPoint2D p: this->points)
        s.append( p->toStringPonto() + "\n" );

    return s;
}
