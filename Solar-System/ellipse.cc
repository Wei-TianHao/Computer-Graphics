//
//  main.cpp
//  OpenGL_test
//
//  Created by WTH on 2017/3/5.
//
//
#include <cstdio>
#include <GLUT/GLUT.h>
#include <set>
#include <assert.h>
#include <cmath>
using namespace std;
#define sqr(x) ((x)*(x))
const int WIDTH = 500;
const int HEIGHT = 500;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;
bool keys[1024];
struct vec3{
	double x, y, z;
	vec3(){}
	vec3(double _x, double _y, double _z):x(_x), y(_y), z(_z){}
};
vec3 operator +(vec3 a,vec3 b){
	return vec3(a.x + b.x,a.y + b.y,a.z + b.z);
}
vec3 operator -(vec3 a,vec3 b){
	return vec3(a.x - b.x,a.y - b.y,a.z - b.z);
}
vec3 operator *(double c, vec3 a){
	return vec3(c * a.x, c * a.y, c * a.z);
}
vec3 operator ^(vec3 a,vec3 b){
	return vec3(a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x);
}
vec3 norm(vec3 a) {
	double len = sqrt(sqr(a.x)+sqr(a.y)+sqr(a.z));
	return (1/len) * a;
}
vec3 u(0, -50, 0);
vec3 v(0, 1, 0);
vec3 n(0, 0, 1);


struct cel_body{
	double dis, r, v;
	vec3 ax;
	cel_body(){}
	cel_body(double _dis, double _r, double _v, vec3 _ax) :
	dis(_dis), r(_r), v(_v), ax(_ax) {}
};
cel_body sun(0, 10, 0, vec3(0,0,1));
cel_body earth(30, 5, 0.01, vec3(0,0,1));
cel_body moon(10, 3, 0.1, vec3(1, 0, 0));
double Mx = 0;
double My = 0;

void init(void) 
{
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glClearColor (0.0, 0.0, 0.0, 0.0); //背景黑色
	gluPerspective(45, (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 3000.0f);
}
void prt(vec3 a) {
	printf("%.1lf %.1lf %.1lf\n", a.x, a.y, a.z);
}
void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  //加载单位矩阵
	vec3 t = norm(v ^ n);
	v = norm(v + My*norm(n) + Mx*t);
	if(keys['w']) u = u + 0.5 * v;
	if(keys['s']) u = u - 0.5 * v;
	if(keys['d']) u = u + 0.5 * t;
	if(keys['a']) u = u - 0.5 * t;
	vec3 ov = u+v;
	gluLookAt(u.x, u.y, u.z,  ov.x, ov.y, ov.z,  n.x, n.y, n.z);
	//=============== draw sun ================
	GLfloat pos[] = {0., 0., 0.};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	GLfloat sun_emi[] = {1., .5, .3, 0.5};

	glMaterialfv(GL_FRONT, GL_EMISSION, sun_emi);
	glMaterialfv(GL_FRONT, GL_AMBIENT, sun_emi);
	GLUquadric *quad;
	quad = gluNewQuadric();
	gluSphere(quad, sun.r, 100, 20);
	
	//=============== draw earth ================
	glPushMatrix();
		GLfloat earth_emi[] = {0., 0., 0., 1.0};
		GLfloat earth_amb[] = {0., 0.4, 1., 1.0};
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_emi);
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_emi);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, earth_amb);
		quad = gluNewQuadric();
		glRotatef(earth.v * glutGet(GLUT_ELAPSED_TIME), earth.ax.x, earth.ax.y, earth.ax.z);
		glTranslatef(earth.dis, 0, 0);
		gluSphere(quad, earth.r, 100, 20);
		//=============== draw moon ================
		glPushMatrix();
			GLfloat moon_emi[] = {0., 0., 0., 1.0};
			GLfloat moon_amb[] = {0.6, 0.6, 0.6, 1.0};
			glMaterialfv(GL_FRONT, GL_EMISSION, moon_emi);
			glMaterialfv(GL_FRONT, GL_AMBIENT, sun_emi);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, moon_amb);
			quad = gluNewQuadric();
			glRotatef(moon.v * glutGet(GLUT_ELAPSED_TIME), moon.ax.x, moon.ax.y, moon.ax.z);
			glTranslatef(0, moon.dis, 0);
			gluSphere(quad, moon.r, 100, 20);
		glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}


void mouseCB(int x, int y){
	if(x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
		Mx *= 0.8;
		My *= 0.8;
		return;
	}
	Mx =  0.01 * (x -  WIDTH/2) / (WIDTH/2);
	My = -0.01 * (y - HEIGHT/2) / (HEIGHT/2);
	if(fabs(Mx) < 0.001) Mx *= 0.1;
	if(fabs(My) < 0.001) My *= 0.1;
	glutPostRedisplay();
}
void keyboardCB(unsigned char key, int x, int y) {
    keys[key] = true;
	glutPostRedisplay();
}
void keyboardUpCB(unsigned char key, int x, int y) {
    keys[key] = false;
	glutPostRedisplay();
}
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 3000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vec3 ov = u+v;
	gluLookAt(u.x, u.y, u.z,  ov.x, ov.y, ov.z,  n.x, n.y, n.z);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Solar System");

	init ();
	
	
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(mouseCB);
	glutKeyboardFunc(keyboardCB);
	glutKeyboardUpFunc(keyboardUpCB);

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();
	return 0;
}