#include <iostream>
#include "tinyxml2.h"
#include "types.h"
#include "load.h"
#include "drawer.h"
#ifdef __APPLE__
#include <glut.h>
#include <cmath>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

using namespace std;
using namespace tinyxml2;

Config config;

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;
float alpha = 0.0f;

float alfa = 1.0f, beta = 0.0f, radius = 60.0f;
float camX, camY, camZ;

//-------------------

void spherical2Cartesian() {
    camX = radius * cos(beta) * sin(alfa);
    camY = radius * sin(beta);
    camZ = radius * cos(beta) * cos(alfa);
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if (h == 0) h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}

void drawAxis() {
    /* Axis drawing */
    // X negative axis in dotted Red
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(4, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); //color

    glVertex3f(-1000.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glPopAttrib();

    // Y negative axis in dotted Green
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(4, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f); //color

    glVertex3f(0.0f, -1000.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glPopAttrib();

    // Z negative axis in dotted Blue
    glPushAttrib(GL_ENABLE_BIT);

    glLineStipple(4, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -1000.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glPopAttrib();

    glBegin(GL_LINES);
    // X positive axis in red
    glColor3f(1.0f, 0.0f, 0.0f); //color

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1000.0f, 0.0f, 0.0f);

    // Y positive axis in Green
    glColor3f(0.0f, 1.0f, 0.0f); //color

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1000.0f, 0.0f);

    // Z positive axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f); //color

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1000.0f);
    glEnd();
}

void drawOrbit(float size) {
    for (float angle = 0.0f, xO = 0.0f, zO = 0.0f; angle <= (2.0f * M_PI); angle += 0.08f) {
        xO = size * sin(angle);
        zO = size * cos(angle);
        glVertex3f(xO, 0.0f, zO);
    }
}

void drawPlanetsOrbits() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    // Mercury orbit
    drawOrbit(12.166f);
    // Venus orbit
    drawOrbit(13.928f);
    // Earth orbit
    drawOrbit(16.031f);
    // Mars orbit
    drawOrbit(16.9f);
    // Jupiter orbit
    drawOrbit(22.940f);
    // Saturn orbit
    drawOrbit(33.659f);
    // Uranus orbit
    drawOrbit(45.277f);
    // Neptune orbit
    drawOrbit(48.508f);
    glEnd();
}

void renderScene() {
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0f, 1.0f, 0.0f);

    glRotatef(x, 1.0, 0.0, 0.0);
    glRotatef(y, 0.0, 1.0, 0.0);
    glRotatef(z, 0.0, 0.0, 1.0);

    // put the geometric transformations here
    glTranslatef(x, y, z);
    glRotatef(alpha, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);

    drawPlanetsOrbits();
    drawScene(config);

    // End of frame
    glutSwapBuffers();
}

void keyboardCallback(unsigned char key_code, int _unused1, int _unused2) {
    switch (key_code) {
        //Esquerda ou direita (w e f)
        case 'w':
            x -= 0.1f;
            break;
        case 'f':
            x += 0.1f;
            break;
            //Aumentar ou diminuir o raio da camera (z e x)
        case 'z':
            radius += 2.0f;
            break;
        case 'x':
            radius -= 2.0f;
            break;
            //Preencher a cor da figura
        case 'c':
            config->drawingType = GL_FILL;
            break;
            //Colocar a figura em linhas
        case 'l':
            config->drawingType = GL_LINE;
            break;
            //Colocar a figura em pontos
        case 'p':
            config->drawingType = GL_POINT;
            break;
        default:
            break;
    }

    spherical2Cartesian();
    glutPostRedisplay();
}

// write function to process keyboard events
void processSpecialKeys(int key, int xx, int yy) {
    switch (key) {
        case GLUT_KEY_RIGHT:
            alfa -= 0.1;
            break;

        case GLUT_KEY_LEFT:
            alfa += 0.1;
            break;

        case GLUT_KEY_UP:
            beta += 0.1f;
            if (beta > 1.5f)
                beta = 1.5f;
            break;

        case GLUT_KEY_DOWN:
            beta -= 0.1f;
            if (beta < -1.5f)
                beta = -1.5f;
            break;
        default:
            break;
    }
    spherical2Cartesian();
    glutPostRedisplay();
}

void glutSetup(int argc, char **argv) {
    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("CG@DI");
    //glutFullScreen();

    // put callback registration here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(processSpecialKeys);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT, GL_FILL);

    glEnable(GL_VERTEX_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    spherical2Cartesian();
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "No scene file provided." << endl;
        return 2;
    }

    glutSetup(argc, argv);

    config = readConfig(argv[1]);

    if (config == nullptr) {
        return 3;
    }

    config->drawingType = GL_FILL;

    // enter GLUT's main loop
    glutMainLoop();

    return 1;
}
