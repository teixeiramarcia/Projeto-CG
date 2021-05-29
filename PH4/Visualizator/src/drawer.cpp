#include <string>

#include "drawer.h"
#include "types.h"
#include "catmull.h"

#ifdef __APPLE__

#include <glut.h>
#include <iostream>

#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

void drawGroup(struct group *);

void drawModel(Model model) {
    auto points = model->points.size() * 3;
    auto normals = model->normals.size() * 3;
    auto textures = model->texture_points.size() * 2;

    //Points
    float v[points];

    auto i = 0;
    for (Point point : model->points) {
        v[i++] = point->x;
        v[i++] = point->y;
        v[i++] = point->z;
    }

    //Normals
    float n[normals];

    i = 0;
    for (Point point : model->normals) {
        n[i++] = point->x;
        n[i++] = point->y;
        n[i++] = point->z;
    }
    float infoN = sizeof(n)/sizeof(n[0]);

    //Texture
    float t[textures];
    i = 0;
    for (Texture_point point : model->texture_points) {
        t[i++] = point->x;
        t[i++] = point->y;
    }

    float infoT = sizeof(t)/sizeof(t[0]);
    GLuint buffer_points;
    GLuint buffer_normals;
    GLuint buffer_textures;

    //Points
    glGenBuffers(1, &buffer_points);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_points);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * points, v, GL_STATIC_DRAW);

    //Normals
    //if(infoN != 0) {
        glGenBuffers(1, &buffer_normals);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_normals);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals, n, GL_STATIC_DRAW);
    //}

    //Texture
    //if(infoT != 0) {
        glGenBuffers(1, &buffer_textures);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_textures);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textures, t, GL_STATIC_DRAW);
    //}

    if (model->color != nullptr) {
        glColor3f(model->color->red, model->color->green, model->color->blue);
        //glBindBuffer(GL_ARRAY_BUFFER, buffer_points);
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        //glBindBuffer(GL_ARRAY_BUFFER, buffer_normals);
        //if(infoN != 0) {
            glNormalPointer(GL_FLOAT, 0, nullptr);
        //}
        //glBindBuffer(GL_ARRAY_BUFFER, buffer_textures);
        //if(infoT != 0) {
            glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
        //}
        glDrawArrays(GL_TRIANGLES, 0, points);
    } else {
        glBindTexture(GL_TEXTURE_2D, model->texture);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_points);
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_normals);
        glNormalPointer(GL_FLOAT, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_textures);
        glTexCoordPointer(2, GL_FLOAT, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, points);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    std::cout << buffer_textures;
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
    m[0] = x[0];
    m[1] = x[1];
    m[2] = x[2];
    m[3] = 0;
    m[4] = y[0];
    m[5] = y[1];
    m[6] = y[2];
    m[7] = 0;
    m[8] = z[0];
    m[9] = z[1];
    m[10] = z[2];
    m[11] = 0;
    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;
}

void cross(float *a, float *b, float *res) {
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a) {
    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}

float antYY[3] = {0, 1, 0};

void transformMove(Move movement) {
    float deriv[3];

    drawMove(movement);
    deriv[0] = deriv[0];
    deriv[1] = deriv[1];
    deriv[2] = deriv[2];
    normalize(deriv);
    float tt = glutGet(GLUT_ELAPSED_TIME) / movement->time;
    Point point = getGlobalCatmullRomPoint(movement, tt, deriv);
    glTranslatef(point->x, point->y, point->z);

    float xi[4];
    xi[0] = deriv[0];
    xi[1] = deriv[1];
    xi[2] = deriv[2];
    normalize(xi);

    float zi[4];
    cross(xi, antYY, zi);
    normalize(zi);

    float yi[4];
    cross(zi, xi, yi);
    normalize(yi);
    antYY[0] = yi[0];
    antYY[1] = yi[1];
    antYY[2] = yi[2];

    float m[16];
    buildRotMatrix(xi, yi, zi, m);
    glMultMatrixf(m);
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
    //checkGLError();
    glPolygonMode(GL_FRONT_AND_BACK, config->drawingType);
    for (Group group : config->groups) {
        drawGroup(group);
    }
}
