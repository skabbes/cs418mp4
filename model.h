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
        vector<Vec3> verticies;
        vector<Vec3> normals;
        vector< vector<int> > faces;

        vector< vector<int> > springs;
        vector< vector<double> > springDistances;

        vector<Vec3> forces;
        vector<Vec3> velocities;

        void applyForces(double time);
        void computeNormals();
    public:
        Model(string filename);
        void render(double time, int how);
        void translate(int x, int y, int z);
        Vec3 centroid();
};
#endif
