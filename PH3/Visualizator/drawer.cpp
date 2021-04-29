#include <string>

#include "drawer.h"
#include "types.h"
#include "catmull.h"

#ifdef __APPLE__

#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

void drawGroup(struct group *);

void drawModel(Model model) {
    glBegin(GL_TRIANGLES);
    glColor3f(model->color->red, model->color->green, model->color->blue);
    for (Point point : model->points) {
        glVertex3f(point->x, point->y, point->z);
    }
    glEnd();
}

void drawTranslate(Point point) {
    glTranslatef(point->x, point->y, point->z);
}

void drawMove(Move movement) {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 10000; ++i) {
        float t = 0.0001f * i;
        float deriv[3];
        Point point = getGlobalCatmullRomPoint(movement, t, deriv);
        glVertex3f(point->x, point->y, point->z);
    }
    glEnd();
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {
    m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
    m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
    m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void cross(float *a, float *b, float *res) {
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

void normalize(float *a) {
    float l = sqrt(a[0]*a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0]/l;
    a[1] = a[1]/l;
    a[2] = a[2]/l;
}

void transformMove(Move movement) {
    drawMove(movement);
    float deriv[3];
    float tt = glutGet(GLUT_ELAPSED_TIME) / movement->time;
    Point point = getGlobalCatmullRomPoint(movement, tt, deriv);
    glTranslatef(point->x, point->y, point->z);

    float zr[3];
    cross(deriv, movement->axis, zr);
    float ty[3];
    cross(zr, deriv, ty);
    normalize(zr);
    normalize(deriv);
    normalize(ty);
    movement->axis = ty;
    float m[16];
    //buildRotMatrix(deriv, ty, zr, m);
    //glMultMatrixf(m);
}

void drawRotate(Rotate rotate) {
    float ticker = glutGet(GLUT_ELAPSED_TIME);
    float angle = ticker * 360 / rotate->time;
    glRotatef(angle, rotate->point->x, rotate->point->y, rotate->point->z);
}

void drawScale(Point point) {
    glScalef(point->x, point->y, point->z);
}

void drawAction(Action action) {
    if (!strcmp(action->name, "model")) {
        drawModel(action->model);
    } else if (!strcmp(action->name, "translate")) {
        drawTranslate(action->translate);
    } else if (!strcmp(action->name, "rotate")) {
        drawRotate(action->rotate);
    } else if (!strcmp(action->name, "scale")) {
        drawScale(action->scale);
    } else if (!strcmp(action->name, "group")) {
        drawGroup(action->group);
    } else if (!strcmp(action->name, "move")) {
        transformMove(action->movement);
    }
}

void drawGroup(struct group *group) {
    glPushMatrix();
    for (Action action : group->actions) {
        drawAction(action);
    }
    glPopMatrix();
}

void drawScene(Config config) {
    glPolygonMode(GL_FRONT_AND_BACK, config->drawingType);
    for (Group group : config->groups) {
        drawGroup(group);
    }
}
