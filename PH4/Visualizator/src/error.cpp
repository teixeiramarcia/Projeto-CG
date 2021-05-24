#include <glut.h>
#include <iostream>

using namespace std;

void debugCheckGLError() {
    GLenum err;
    cout << "-" << endl;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cout << " " << err << endl;
    }
}
