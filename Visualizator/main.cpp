
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include <vector>
#include <tuple>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

vector<tuple<double, double, double>> points;

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;
float alpha = 0.0f;
int drawingType = GL_LINE;

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

void spherical2Cartesian() {
    camX = radius * cos(beta) * sin(alfa);
    camY = radius * sin(beta);
    camZ = radius * cos(beta) * cos(alfa);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0) h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
    gluLookAt(camX,camY,camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    glRotatef(x, 1.0, 0.0, 0.0);
    glRotatef(y, 0.0, 1.0, 0.0);
    glRotatef(z, 0.0, 0.0, 1.0);

    /* Axis drawing */

    // X negative axis in dotted Red
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(4, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); //color

    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f( 0.0f, 0.0f, 0.0f);
    glEnd();
    glPopAttrib();

    // Y negative axis in dotted Green
    glPushAttrib(GL_ENABLE_BIT);
    glLineStipple(4, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f); //color

    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glPopAttrib();

    // Z negative axis in dotted Blue
    glPushAttrib(GL_ENABLE_BIT);

    glLineStipple(4, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f,  0.0f);
    glEnd();
    glPopAttrib();

    glBegin(GL_LINES);
    // X positive axis in red
    glColor3f(1.0f, 0.0f, 0.0f); //color

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f( 100.0f, 0.0f, 0.0f);

    // Y positive axis in Green
    glColor3f(0.0f, 1.0f, 0.0f); //color

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);

    // Z positive axis in Blue
    glColor3f(0.0f, 0.0f, 1.0f); //color

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f,  100.0f);
    glEnd();

    // put the geometric transformations here
    glTranslatef(x, y, z);
    glRotatef(alpha, 0.0f, 1.0f, 0.0f);
    glScalef(1.0f, 1.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, drawingType);

    glBegin(GL_TRIANGLES);
    for(auto const& value : points) {
        glColor3f(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX));
        glVertex3f(get<0>(value), get<1>(value), get<2>(value));
    }
    glEnd();

    // End of frame
	glutSwapBuffers();
}

void keyboardCallback(unsigned char key_code, int _unused1, int _unused2) {
    switch (key_code) {
        //Rodar a figura (direita e esquerda)
        case 'd':
            alpha += 22.5f;
            break;
        case 'e':
            alpha -= 22.5f;
            break;
        //Subir ou descer (north e south)
        case 'n':
            y += 0.1f;
            break;
        case 's':
            y -= 0.1f;
            break;
        //Esquerda ou direita (w e f)
        case 'w':
            x -= 0.1f;
            break;
        case 'f':
            x += 0.1f;
            break;
        // Preencher a cor da figura
        case 'c':
            drawingType = GL_FILL;
            break;
        //Colocar a figura em linhas
        case 'l':
            drawingType = GL_LINE;
            break;
        //Colocar a figura em pontos
        case 'p':
            drawingType = GL_POINT;
            break;
        default:
            break;
    }

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

        case GLUT_KEY_PAGE_DOWN:
            radius -= 0.1f;
            if (radius < 0.1f)
                radius = 0.1f;
            break;

        case GLUT_KEY_PAGE_UP:
            radius += 0.1f;
            break;
        default:
            break;
    }
    spherical2Cartesian();
    glutPostRedisplay();
}


vector<string> splitter(string& str, char delimiter) {
    stringstream ss(str);
    string item;
    vector<string> splittedStrs;

    while (getline(ss, item, delimiter)){
        splittedStrs.push_back(item);
    }

    return splittedStrs;
}

void readModel(const char * filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        getline(file, line);
        while(getline(file, line)) {
            vector<string> s = splitter(line, ' ');
            points.emplace_back(make_tuple(stod(s.at(0)), stod(s.at(1)), stod(s.at(2))));
        }
    }
    file.close();
}

bool readModels(const char * filename) {
    XMLDocument document;

    XMLError result = document.LoadFile(filename);

    if (result != XML_SUCCESS) return false;

    XMLNode* root = document.FirstChildElement("scene");

    if (root == nullptr) return false;

    XMLElement * element = root->FirstChildElement("model");
    while(element != nullptr) {
        readModel(element->Attribute("file"));
        element = element->NextSiblingElement("model");
    }

    return true;
}

void glutSetup(int argc, char **argv) {
    // put GLUT init here
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow("CG@DI");
    glutFullScreen();

    // put callback registration here
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(processSpecialKeys);

    // OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
    spherical2Cartesian();
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "No model file provided." << endl;
        return 2;
    }

    if (!readModels(argv[1])) {
        return 3;
    }

    glutSetup(argc, argv);

    // enter GLUT's main loop
	glutMainLoop();
	
	return 1;
}
