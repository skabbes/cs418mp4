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
    public:
        Model(string filename);
        void computeNormals();
        void setTexture(string filename, bool wrap);
        void setMixture(string filename, bool wrap);
        void setEnv(string filename);
        void render(int shader_program, bool showNormals);
};
#endif
