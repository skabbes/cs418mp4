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

    Vec3 zero(0, 0, 0);
    vector<int> emptyi;
    vector<double> emptyd;

    while( in ){
        in >> command;
        if( in.eof() ) continue;
        
        if( command == "v" ){
            Vec3 v;
            in >> v[0];
            in >> v[1];
            in >> v[2];
            verticies.push_back(v);
            forces.push_back( zero );
            velocities.push_back( zero );
            springs.push_back( emptyi );
            springDistances.push_back( emptyd );

        } else if (command == "f" ){
            vector<int> f(3);
            in >> f[0];
            in >> f[1];
            in >> f[2];
            faces.push_back(f);
            normals.push_back( zero );
        } else if (command == "s" ){
            int v1, v2;
            double d;
            in >> v1;
            in >> v2;
            in >> d;

            springs[v1].push_back(v2);
            springDistances[v1].push_back(d);
        }

        getline(in, command);
    }

    computeNormals();
}

void Model::translate(int x, int y, int z){
    Vec3 translate(x, y, z);
    for(int i=0;i<verticies.size();i++){
        verticies[i] += translate;
    }
}

void Model::computeNormals(){
    for(int i=0;i<faces.size();i++){
        Vec3 one = verticies[ faces[i][0] ];
        Vec3 two = verticies[ faces[i][1] ];
        Vec3 three = verticies[ faces[i][2] ];
        
        Vec3 x = two - one;
        Vec3 y = three - one;
        Vec3 normal = cross(x, y);
        unitize(normal);
        normals[i] = normal;
    }
}

Vec3 Model::centroid(){
    Vec3 centroid;
    for(int i=0;i<verticies.size();i++){
        centroid += verticies[i];
    }
    return centroid / verticies.size();
}

void Model::applyForces(double time){
    Vec3 gravity(0, -1, -9.8);
    Vec3 zero(0, 0, 0);

    // 500, 8
    // 800, 25
    double k = 500;
    double damp = 8;

    int t = 10;
    double eps = time / t;
    while(--t){

        for(int i=0;i<verticies.size();i++){
            forces[i] = gravity;
        }

        for(int i=0;i<verticies.size();i++){
            vector<int> & currentSprings = springs[i];
            vector<double> & distances = springDistances[i];
            Vec3 v1 = verticies[i];
            Vec3 vel1 = velocities[i];

            for(int j=0; j<currentSprings.size();j++){
                Vec3 v2 = verticies[ currentSprings[j] ];
                Vec3 vel2 = velocities[ currentSprings[j] ];

                Vec3 d = v2 - v1;
                float dist = sqrt( d[0] * d[0] + d[1] * d[1] + d[2] * d[2] );
                unitize(d);

                float diff = dist - distances[j];
                forces[i] += d *  k * (diff);
                forces[i] += d * d * -damp * ( vel1 - vel2);
            }
        }


        for(int i=0;i<verticies.size();i++){
            verticies[i] += velocities[i] * eps;
            velocities[i] += forces[i] * eps;
        }

        for(int i=0;i<verticies.size();i++){
            if( verticies[i][2] < 0 ){
                verticies[i][2] = 0;
                velocities[i] = zero;
                forces[i] = zero;
            }
        }
    }
}

void Model::render(double time, int how){
    applyForces(time);
    float x, y, z;
    float nx, ny, nz;

    if( how == 0){
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
    } else if( how == 1){
        glColor3f(1.0, 0.0, 0.0);
        for(int i=0;i<verticies.size();i++){
            Vec3 v1 = verticies[i];

            glPushMatrix();
            glTranslatef( v1[0], v1[1], v1[2] + .5 );
            glutSolidSphere(.4, 10, 10);
            glPopMatrix();
        }
    } else if( how == 2){
        glEnable(GL_COLOR_MATERIAL);
        glBegin(GL_LINES);
        for(int i=0;i<verticies.size();i++){
            Vec3 v1 = verticies[i];
            vector<int> & currentSprings = springs[i];
            vector<double> & distances = springDistances[i];

            for(int j=0; j<currentSprings.size();j++){
                Vec3 v2 = verticies[ currentSprings[j] ];

                Vec3 d = v2 - v1;
                float dist = sqrt( d[0] * d[0] + d[1] * d[1] + d[2] * d[2] );
                float restDist = distances[j];

                float percent = (dist - restDist + .05) / .05;
                percent = percent < 0 ? 0 : percent;
                percent = percent > 1 ? 1 : percent;

                if( percent <= .5){
                    percent *= 2;
                    glColor3f(1, 1-percent, 1-percent);
                } else {
                    percent = (percent - .5) * 2;
                    percent *= 2;
                    glColor3f(1-percent, 1-percent, 1);
                }
                glVertex3f( v1[0], v1[1], v1[2] );
                glVertex3f( v2[0], v2[1], v2[2] );
            }
        }
        glEnd();
        glDisable(GL_COLOR_MATERIAL);
    }

}
