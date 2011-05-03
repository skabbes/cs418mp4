#include <GL/glew.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gfx/vec3.h"
#include "gfx/vec4.h"
#include "gfx/mat3.h"
#include "gfx/mat4.h"
#include "model.h"
#include "textureloader.h"
#include "shaderhelp.h"

using namespace gfx;

Model blockI("I.obj");

int bezier[][3] = { 
    {10, 10, 2},
    {10, 20, 5},
    {0, -10, 10},
    {8, 0, 2}
};

int showSprings = 0;

Vec3 computeBezier(double time){
    while( time > 15 ) time -= 15;
    time /= 15;

    Vec3 camera;
    int i = (int)time;
    double t = time - (int)(time);
    double t1 = 1-t;

    double b[] = 
    {
        t1 * t1 * t1,
        3 * t1 * t1 * t,
        3 * t1 * t * t,
        t * t * t
    };


    for(int j=0;j<4;j++){
        camera[0] += bezier[j][0] * b[j];
        camera[1] += bezier[j][1] * b[j];
        camera[2] += bezier[j][2] * b[j];
    }
    return camera;
}

void init(void) 
{
    printf("Welcome to the Block I particle system\n");
    printf("\tpress 'b' to visualize springs with balls\n");
    printf("\tpress 's' to visualize springs with colored lines\n");
    printf("\tpress 'n' to return to normal mode\n");
    blockI.translate(0,0,5);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat white[] = {1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glClearColor (0.5, 0.5, 1.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat position[] = { -1.0, 1.0, 1.0, 0.0 };

    static int time = 0;
    double secondsElapsed = (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0;
    time = glutGet(GLUT_ELAPSED_TIME);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    glPushMatrix ();
    Vec3 centroid = blockI.centroid();
    Vec3 camera = computeBezier(time / 1000.0);

    gluLookAt (camera[0], camera[1], camera[2], centroid[0], centroid[1], centroid[2], 0.0, 0.0, 1.0);
    //gluLookAt (camera[0], camera[1], camera[2], 0, 0, 0, 0.0, 0.0, 1.0);

    glPushMatrix ();
    glLightfv (GL_LIGHT0, GL_POSITION, position);
    glPopMatrix ();

    glPushMatrix();

	GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
	GLfloat tandiff[] = {0.4,0.3,0.2,1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tandiff);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

    blockI.render( secondsElapsed, showSprings );

    glPopMatrix();

	GLfloat flooramb[] = {0.2,0.0,0.0,1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, flooramb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, flooramb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, flooramb);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

    // draw other stuff
    glBegin(GL_QUADS);
        float width = 20.f;
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f(width, width, 0.0);
        glVertex3f(width, -width, 0.0);
        glVertex3f(-width, -width, 0.0);
        glVertex3f(-width, width, 0.0);
    glEnd(); // GL_QUADS

    glPopMatrix ();

    glutSwapBuffers();
    glFlush ();
    glutPostRedisplay();
}


void reshape (int w, int h)
{
    float aspect = ((float)w)/h;
    float fov = 70.f;
    float near = 0.1f;
    float far = 1000.f;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov,aspect,near,far);
    glMatrixMode (GL_MODELVIEW);
}

void arrows(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            break;
        case GLUT_KEY_DOWN:
            break;
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
    }
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
        case 's':
            showSprings = 2;
            break;
        case 'b':
            showSprings = 1;
            break;
        case 'n':
            showSprings = 0;
            break;
        case 27:
            exit(0);
            break;
   }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);

    glewInit();
    if ( glewIsSupported("GL_VERSION_2_0" )){
        printf("Ready for OpenGL 2.0\n");
    }
    else {
        printf("OpenGL 2.0 not supported\n");
        exit(1);
    }

    init ();
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrows);

    glutMainLoop();

    return 0;
}
