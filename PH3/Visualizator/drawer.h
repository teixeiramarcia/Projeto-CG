#ifndef VISUALIZADOR_DRAWER_H
#define VISUALIZADOR_DRAWER_H

#ifdef __APPLE__

#include <GLUT/glut.h>
#include "types.h"

#else
#include <GL/glut.h>
#include <GL/gl.h>
#endif

void drawScene(Config config);

#endif //VISUALIZADOR_DRAWER_H
