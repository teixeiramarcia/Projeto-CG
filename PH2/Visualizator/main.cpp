
#include <vector>
#include <tuple>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "tinyxml2.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

using namespace std;
using namespace tinyxml2;

//Global variables
vector<tuple<float, float, float>> points;

typedef struct states {
    tuple<float, float, float> translate_state; //translates
    tuple<float, float, float, float> rotate_state; //rotates
    tuple<float, float, float> scale_state; //scales
} *States;

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;
float alpha = 0.0f;
int drawingType = GL_LINE;

//-------------------

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
    gluLookAt(5.0,5.0,5.0,
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
        glColor3f(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX));
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

States readTranslate(XMLElement* element, States state) {
    float val_x = strtof(element->Attribute("X"), nullptr);
    float val_y = strtof(element->Attribute("Y"), nullptr);
    float val_z = strtof(element->Attribute("Z"), nullptr);

    get<0>(state->translate_state) += val_x;
    get<1>(state->translate_state) += val_y;
    get<2>(state->translate_state) += val_z;

    return state;
}

States readRotate(XMLElement* element, States state) {
    float val_angle = strtof(element->Attribute("angle"), nullptr);
    float val_x = strtof(element->Attribute("X"), nullptr);
    float val_y = strtof(element->Attribute("Y"), nullptr);
    float val_z = strtof(element->Attribute("Z"), nullptr);

    get<0>(state->rotate_state) += val_angle;
    get<1>(state->rotate_state) = val_x;
    get<2>(state->rotate_state) = val_y;
    get<3>(state->rotate_state) = val_z;

    return state;
}

States readScale(XMLElement* element, States state) {
    float val_x = strtof(element->Attribute("X"), nullptr);
    float val_y = strtof(element->Attribute("Y"), nullptr);
    float val_z = strtof(element->Attribute("Z"), nullptr);

    get<0>(state->scale_state) += val_x;
    get<1>(state->scale_state) += val_y;
    get<2>(state->scale_state) += val_z;

    return state;
}

States readGroups(XMLNode * group, States state) {
    States currentState = state;
    for(XMLNode * g = group->FirstChild(); g != nullptr; g = g->NextSibling()) {
        const char * name = g->Value();
        if(!strcmp(name, "models")) {
            XMLElement * e = g->FirstChildElement("model");

            while(e != nullptr) {
                readModel(e->Attribute("file"));
                e = e->NextSiblingElement("model");
            }
        } else if (!strcmp(name, "translate")) {
            auto * e = (XMLElement*) g;
            state = readTranslate(e, currentState);
        } else if (!strcmp(name, "rotate")) {
            auto * e = (XMLElement*) g;
            state = readRotate(e, currentState);
        } else if (!strcmp(name, "scale")) {
            auto * e = (XMLElement*) g;
            state = readScale(e, currentState);
        } else if (!strcmp(name, "group")) {
            state = readGroups(g, currentState);
        }
    }
    return state;
}

bool readConfig(const char * filename) {
    XMLDocument document;

    XMLError result = document.LoadFile(filename);

    if (result != XML_SUCCESS) return false;

    XMLNode* root = document.FirstChildElement("scene");

    if (root == nullptr) return false;

    XMLElement * element = root->FirstChildElement("group");

    States state;
    {
        state = (States) malloc(sizeof(states));
        state->translate_state = make_tuple(0.0, 0.0, 0.0);
        state->rotate_state = make_tuple(0.0, 0.0, 0.0, 0.0);
        state->scale_state = make_tuple(0.0, 0.0, 0.0);

        while (element != nullptr) {
            readGroups(element, state);
            element = (XMLElement *) element->NextSibling();
        }
    }
    free(state);
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
    glutKeyboardFunc(keyboardCallback);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glClearColor(0.0f,0.0f,0.0f,0.0f);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "No model file provided." << endl;
        return 2;
    }

    if (!readConfig(argv[1])) {
        return 3;
    }

    glutSetup(argc, argv);

    // enter GLUT's main loop
    glutMainLoop();

    return 1;
}
