#ifndef VISUALIZADOR_TYPES_H
#define VISUALIZADOR_TYPES_H

#include <vector>
#include <glut.h>

using namespace std;

typedef struct point {
    float x;
    float y;
    float z;
} *Point;

typedef struct texture_point {
    float x;
    float y;
} *Texture_point;

typedef struct rotate {
    float time;
    Point point;
} *Rotate;

typedef struct move {
    float *axis;
    float time;
    vector<Point> points;
} *Move;

typedef struct color {
    float red;
    float green;
    float blue;
} *Color;

typedef struct model {
    vector<Point> points;
    vector<Point> normals;
    vector<Texture_point> texture_points;
    GLuint texture;
    Color color{};
} *Model;

struct group;

typedef struct action {
    const char *name{};
    Point translate{};
    Move movement{};
    Point scale{};
    Rotate rotate{};
    Model model{};
    group *group{};
} *Action;

typedef struct group {
    vector<Action> actions;
} *Group;

typedef struct light {
    const char *type;
    int posX;
    int posY;
    int posZ;
} *Light;

typedef struct config {
    int drawingType;
    vector<Group> groups;
    vector<Light> lights;
} *Config;

#endif //VISUALIZADOR_TYPES_H
