#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

#include "gfx/vec3.h"
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;
using namespace gfx;

class Model{
    private:
        vector<Vec3f> verticies;
        vector<Vec3f> normals;
        vector<Vec3i> faces;
        GLuint texture;
        GLuint mixture;
        GLuint env;
        void computeNormals();
    public:
        Model(string filename);
        void render();
};
#endif
