#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <GL/glut.h>
GLuint loadTexture( const char * filename, int wrap);
GLuint loadSphereMap( const char * filename);
#endif
