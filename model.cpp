#include "model.h"
#include "textureloader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <GL/glut.h>
#include <cmath>

Model::Model(string filename){
    texture = 0;

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

            // obj file format actually uses 1-indexed faces
            f[0] -= 1;
            f[1] -= 1;
            f[2] -= 1;
            faces.push_back(f);
        } 

        getline(in, command);
    }

    Vec3f zero(0, 0, 0);
    for(int i=0;i<verticies.size();i++){
        normals.push_back( zero );
    }

}

void Model::setTexture(string filename, bool wrap){
    texture = loadTexture(filename.c_str(), wrap ? 1: 0);
}

void Model::setMixture(string filename, bool wrap){
    mixture = loadTexture(filename.c_str(), wrap ? 1: 0);
}

void Model::setEnv(string filename){
    env = loadSphereMap(filename.c_str());
}

void Model::computeNormals(){
    for(int i=0;i<faces.size();i++){
        Vec3f one = verticies[ faces[i][0] ];
        Vec3f two = verticies[ faces[i][1] ];
        Vec3f three = verticies[ faces[i][2] ];
        
        Vec3f x = two - one;
        Vec3f y = three - one;
        Vec3f normal = cross(x, y);

        normals[ faces[i][0] ] += normal;
        normals[ faces[i][1] ] += normal;
        normals[ faces[i][2] ] += normal;
    }

    for(int i=0;i<normals.size();i++){
        unitize( normals[i] );
    }
}

void Model::render(int shader_program, bool showNormals){

    glUseProgram(shader_program);
    int texCoordLoc = glGetAttribLocation(shader_program,"TextureCoord");
    int texLoc = glGetUniformLocation(shader_program,"texture");
    int envLoc = glGetUniformLocation(shader_program,"env");
    int mixLoc = glGetUniformLocation(shader_program,"mixture");

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, env);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);


    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, mixture);

    glUniform1i(envLoc, 0);
    glUniform1i(texLoc, 2);
    glUniform1i(mixLoc, 4);

    float x, y, z, theta;
    float nx, ny, nz;
    float ymax = .5;

    for(int i=0;i<faces.size();i++){
        glBegin(GL_TRIANGLES);
            for(int j=0;j<3;j++){
                x = verticies[ faces[i][j] ][0];
                y = verticies[ faces[i][j] ][1];
                z = verticies[ faces[i][j] ][2];
                theta = atan2(x, z);

                float percent = (theta + M_PI) / (2 * M_PI);
                if(percent > .5) percent = 1 - percent;

                nx = normals[ faces[i][j] ][0];
                ny = normals[ faces[i][j] ][1];
                nz = normals[ faces[i][j] ][2];

                glNormal3f(nx, ny, nz);
                glVertexAttrib2f(texCoordLoc, 20* percent, y / ymax);
                glVertex3f(x, y, z);
            }
        glEnd();
    }

    glUseProgram(0);

    if( showNormals ){
        for(int i=0;i<verticies.size();i++){
            glBegin(GL_LINES);
                glColor3f(1.0, 0.0, 0.0);
                Vec3f start = verticies[i];
                Vec3f normal = normals[i];
                Vec3f end = start + normal * .2;

                glVertex3f(start[0], start[1], start[2]);
                glVertex3f(end[0], end[1], end[2]);
            glEnd();
        }
    }


    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, 0);
}
