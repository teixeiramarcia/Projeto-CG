
#include <vector>
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

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat z = 0.0f;
float alpha = 0.0f;
int drawingType = GL_LINE;

float alfa = 1.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

struct group;
void drawGroup(struct group*);

typedef struct point {
    float x;
    float y;
    float z;
} *Point;

typedef struct rotate {
    float angle;
    Point point;
} *Rotate;

typedef struct model {
    vector<Point> points;
} *Model;

typedef struct action {
    const char* name{};
    Point translate{};
    Point scale{};
    Rotate rotate{};
    Model model{};
    group * group{};
} *Action;

typedef struct group {
    vector<Action> actions;
} *Group;

typedef struct config {
    vector<Group> groups;
} *Config;

Config config;

//-------------------

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

void drawModel(Model model) {
    glBegin(GL_TRIANGLES);
    for(Point point : model->points) {
        glColor3f(rand() / float(RAND_MAX), rand() / float(RAND_MAX), rand() / float(RAND_MAX));
            glVertex3f(point->x, point->y, point->z);
    }
    glEnd();
}

void drawTranslate(Point point) {
    glTranslatef(point->x,point->y,point->z);
}

void drawRotate(Rotate rotate) {
    glRotatef(rotate->angle, rotate->point->x, rotate->point->y, rotate->point->z);
}

void drawScale(Point point) {
    glScalef(point->x,point->y,point->z);
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
    }
}

void drawGroup(struct group* group) {
    for(Action action : group->actions) {
        drawAction(action);
    }
}

void drawScene(){
    glPolygonMode(GL_FRONT_AND_BACK, drawingType);
    for(Group group : config->groups) {
        glPushMatrix();
        drawGroup(group);
        glPopMatrix();
    }
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

    drawScene();

    // End of frame
    glutSwapBuffers();
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

Action readTranslate(XMLElement* e) {
    auto action = new struct action();
    action->name = "translate";
    action->translate = new struct point();

    action->translate->x = strtof(e->Attribute("X"), nullptr);
    action->translate->y = strtof(e->Attribute("Y"), nullptr);
    action->translate->z = strtof(e->Attribute("Z"), nullptr);

    return action;
}

Action readRotate(XMLElement* e) {
    auto action = new struct action();
    action->name = "rotate";
    action->rotate = new struct rotate();

    action->rotate->angle = strtof(e->Attribute("angle"), nullptr);
    action->rotate->point->x = strtof(e->Attribute("X"), nullptr);
    action->rotate->point->y = strtof(e->Attribute("Y"), nullptr);
    action->rotate->point->z = strtof(e->Attribute("Z"), nullptr);

    return action;
}

Action readScale(XMLElement* e) {
    auto action = new struct action();
    action->name = "scale";
    action->scale = new struct point();

    action->scale->x = strtof(e->Attribute("X"), nullptr);
    action->scale->y = strtof(e->Attribute("Y"), nullptr);
    action->scale->z = strtof(e->Attribute("Z"), nullptr);

    return action;
}

Action readModel(XMLElement* e) {
    auto action = new struct action();
    action->name = "model";
    action->model = new struct model();

    ifstream file(e->Attribute("file"));
    if (file.is_open()) {
        string line;
        getline(file, line);
        while (getline(file, line)) {
            vector<string> s = splitter(line, ' ');
            auto point = new struct point();
            point->x = stof(s.at(0));
            point->y = stof(s.at(1));
            point->z = stof(s.at(2));

            action->model->points.push_back(point);
        }
    }
    file.close();

    return action;
}

void readModels(XMLElement* models, vector<Action> *actions) {
    XMLElement * model = models->FirstChildElement("model");
    while (model != nullptr) {
        actions->push_back(readModel(model));
        model = model->NextSiblingElement("model");
    }
}

Group readGroups(XMLNode * node) {
    auto group = new struct group();

    for (XMLNode * g = node->FirstChild(); g != nullptr; g = g->NextSibling()) {
        const char * name = g->Value();
        if (!strcmp(name, "models")) {
            auto * e = (XMLElement*) g;
            readModels(e, &group->actions);
        } else if (!strcmp(name, "translate")) {
            auto * e = (XMLElement*) g;
            group->actions.push_back(readTranslate(e));
        } else if (!strcmp(name, "rotate")) {
            auto * e = (XMLElement*) g;
            group->actions.push_back(readRotate(e));
        } else if (!strcmp(name, "scale")) {
            auto * e = (XMLElement*) g;
            group->actions.push_back(readScale(e));
        } else if (!strcmp(name, "group")) {
            Group subgroup = readGroups(g);
            auto action = new struct action();
            action->name = "group";
            action->group = subgroup;
            group->actions.push_back(action);
        }
    }

    return group;
}

bool readConfig(const char * filename) {
    XMLDocument document;
    XMLError result = document.LoadFile(filename);
    if (result != XML_SUCCESS) return false;
    XMLNode* root = document.FirstChildElement("scene");
    if (root == nullptr) return false;
    XMLElement * element = root->FirstChildElement("group");

    config = new struct config();

    while (element != nullptr) {
        Group group = readGroups(element);
        config->groups.push_back(group);
        element = (XMLElement *) element->NextSibling();
    }

    return true;
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
void processSpecialKeys(int key, int xx, int yy) { //FIXME: not working
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
        cout << "No scene file provided." << endl;
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
