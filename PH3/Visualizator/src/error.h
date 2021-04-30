#ifndef VISUALIZADOR_ERROR_H
#define VISUALIZADOR_ERROR_H

#if DEBUG
#define checkGLError() debugCheckGLError()
#else
#define checkGLError()
#endif

#endif //VISUALIZADOR_ERROR_H
