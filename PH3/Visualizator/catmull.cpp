#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include "types.h"

#else
#include <GL/glut.h>
#endif

void multMatrixVector(float *m, float *v, float *res) {

    for (int j = 0; j < 4; ++j) {
        res[j] = 0;
        for (int k = 0; k < 4; ++k) {
            res[j] += v[k] * m[j * 4 + k];
        }
    }
}


Point getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float *deriv) {

    // catmull-rom matrix
    float m[4][4] = {	{-0.5f,  1.5f, -1.5f,  0.5f},
                         { 1.0f, -2.5f,  2.0f, -0.5f},
                         {-0.5f,  0.0f,  0.5f,  0.0f},
                         { 0.0f,  1.0f,  0.0f,  0.0f}};

    // Compute A = M * P
    float a[3][4];

    float px[4] = { p0->x, p1->x, p2->x, p3->x };
    float py[4] = { p0->y, p1->y, p2->y, p3->y };
    float pz[4] = { p0->z, p1->z, p2->z, p3->z };

    multMatrixVector(*m, px, a[0]);
    multMatrixVector(*m, py, a[1]);
    multMatrixVector(*m, pz, a[2]);

    // Compute pos = T * A
    float tv[4] =  { t*t*t, t*t, t, 1 };
    float tvd[4] = { 3*t*t, 2*t, 1, 0 };

    // compute deriv = T' * A
    deriv[0] = tvd[0]*a[0][0] + tvd[1]*a[0][1] + tvd[2]*a[0][2] + tvd[3]*a[0][3];
    deriv[1] = tvd[0]*a[1][0] + tvd[1]*a[1][1] + tvd[2]*a[1][2] + tvd[3]*a[1][3];
    deriv[2] = tvd[0]*a[2][0] + tvd[1]*a[2][1] + tvd[2]*a[2][2] + tvd[3]*a[2][3];

    auto point = new struct point();
    point->x = tv[0]*a[0][0] + tv[1]*a[0][1] + tv[2]*a[0][2] + tv[3]*a[0][3];
    point->y = tv[0]*a[1][0] + tv[1]*a[1][1] + tv[2]*a[1][2] + tv[3]*a[1][3];
    point->z = tv[0]*a[2][0] + tv[1]*a[2][1] + tv[2]*a[2][2] + tv[3]*a[2][3];

    return point;
}


// given  global t, returns the point in the curve
Point getGlobalCatmullRomPoint(Move movement, float gt, float *deriv) {
    size_t n_points = movement->points.size();
    float t = gt * n_points; // this is the real global t
    int index = floor(t);  // which segment
    t = t - index; // where within  the segment

    // indices store the points
    int indices[4];
    indices[0] = (index + n_points - 1) % n_points;
    indices[1] = (indices[0] + 1) % n_points;
    indices[2] = (indices[1] + 1) % n_points;
    indices[3] = (indices[2] + 1) % n_points;

    Point point = getCatmullRomPoint(t, movement->points[indices[0]], movement->points[indices[1]], movement->points[indices[2]], movement->points[indices[3]], deriv);

    return point;
}