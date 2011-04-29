#include "model.h"
#include "textureloader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <GL/glut.h>
#include <cmath>

Model::Model(string filename){
    ifstream in;
    in.open(filename.c_str(), ios::in);

    string command;
    float f1, f2, f3;

    while( in ){
        in >> command;
        if( in.eof() ) continue;
        
        if( command == "v" ){
            Vec3f v;
            in >> v[0];
            in >> v[1];
            in >> v[2];
            verticies.push_back(v);
        } else if (command == "f" ){
            Vec3i f;
            in >> f[0];
            in >> f[1];
            in >> f[2];
            faces.push_back(f);
        } 

        getline(in, command);
    }

    Vec3f zero(0, 0, 0);
    for(int i=0;i<faces.size();i++){
        normals.push_back( zero );
    }
    computeNormals();
}

void Model::computeNormals(){
    for(int i=0;i<faces.size();i++){
        Vec3f one = verticies[ faces[i][0] ];
        Vec3f two = verticies[ faces[i][1] ];
        Vec3f three = verticies[ faces[i][2] ];
        
        Vec3f x = two - one;
        Vec3f y = three - one;
        Vec3f normal = cross(x, y);
        unitize(normal);
        normals[i] = normal;
    }
}

void Model::render(){
    float x, y, z;
    float nx, ny, nz;

    for(int i=0;i<faces.size();i++){
        glBegin(GL_TRIANGLES);
            for(int j=0;j<3;j++){
                x = verticies[ faces[i][j] ][0];
                y = verticies[ faces[i][j] ][1];
                z = verticies[ faces[i][j] ][2];

                nx = normals[i][0];
                ny = normals[i][1];
                nz = normals[i][2];

                glNormal3f(nx, ny, nz);
                glVertex3f(x, y, z);
            }
        glEnd();
    }
}
