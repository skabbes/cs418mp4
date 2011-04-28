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

Model teapot("teapot.obj");

// Yeah! I've got some shaders
GLuint vtx_shader, frag_shader, shader_program;

bool showNormals = false;

void initShaders(char * fsFilename) {
	char *vs = NULL,*fs = NULL;

	vtx_shader = glCreateShader(GL_VERTEX_SHADER);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);	

	vs = textFileRead("vs_basic.vert");
	fs = textFileRead( fsFilename );

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(vtx_shader, 1, &vv,NULL);
	glShaderSource(frag_shader, 1, &ff,NULL);

	free(vs);
	free(fs);

	glCompileShader(vtx_shader);
	glCompileShader(frag_shader);

	//printShaderInfoLog(vtx_shader);
	//printShaderInfoLog(frag_shader);

	shader_program = glCreateProgram();
	glAttachShader(shader_program,vtx_shader);
	glAttachShader(shader_program,frag_shader);

	glLinkProgram(shader_program);
	//printProgramInfoLog(shader_program);
}

void init(void) 
{
    printf("Welcome to the Teapot Demo\n");
    printf("Press 'n' to toggle showing normals\n");
    printf("Press 'e' to just show environment mapping\n");
    printf("Press 't' to just show texture mapping\n");
    printf("Press 'm' to just show mixed environment and texture mapping (with a mask)\n");

    initShaders("mixer.frag");

    teapot.computeNormals();
    teapot.setEnv("uffizi.ppm");
    teapot.setTexture("metal.ppm", true);
    teapot.setMixture("metal_mix.ppm", true);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat white[] = {.8,.8,.8,1.0};
    GLfloat lpos[] = {-2.0,1.0,0.0,0.0};

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    glClearColor (0.5, 0.5, 1.0, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);

    /* clear the matrix */
    
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

    static int time = 0;
    double secondsElapsed = (glutGet(GLUT_ELAPSED_TIME) - time) / 1000.0;
    time = glutGet(GLUT_ELAPSED_TIME);

	gluLookAt(0.f,3.f,5.f,0.f,2.f,0.f,0.f,1.f,0.f);

    glRotatef((int)(time/50) % 360 , 0, 1, 0);


	GLfloat amb[] = {.1,0.1,0.1,1.0};
	GLfloat diff[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, diff);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 110.0);

    teapot.render(shader_program, showNormals);

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
        case 'e':
            initShaders("env.frag");
            break;
        case 'm':
            initShaders("mixer.frag");
            break;
        case 't':
            initShaders("tex.frag");
            break;
        case 'n':
            showNormals = !showNormals;
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
