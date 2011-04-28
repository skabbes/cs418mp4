//////////////////////////////////////////////////////////////////////
// Help functions for shader operation, source code from www.lighthouse3d.com
/////////////////////////////////////////////////////////////////////
#include <GL/glew.h>


char *textFileRead(char *fn); // text file reading 

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line);
void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);
