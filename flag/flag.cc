//
//  main.cpp
//  OpenGL_test
//
//  Created by WTH on 2017/3/5.
//
//
#include <cstdio>
#include <cmath>
#include <GLUT/GLUT.h>
#include <set>
#include <assert.h>
using namespace std;
#define toRad(x) (x/180.0*Pi)
const double A = 10000;
const double B = A*1.9;
const int n = 20;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;

int sgn(double x) {
	return x < -1e-9 ? -1 : x > 1e-9;
}
void drawPolygon() {
	
	double C = 0.5385 * A;
	double D = 0.7600 * A;
	double E = 0.0538 * A, F = 0.0538 * A;
	double G = 0.0633 * A, H = 0.0633 * A;
	double K = 0.0616 * A;
	double L = 0.0769 * A;
	double r = K / 2;
	double sr = r / ((tan(toRad(72)) + tan(toRad(54))) * cos(toRad(54)));
	// Draw red bar
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.698, 0.132, 0.20);
	for(int i = 0; i < 7; i++) {
		glBegin(GL_POLYGON);
			glVertex2f(0.0, A - 2*i*L);
			glVertex2f(B, A - 2*i*L);
			glVertex2f(B, A - 2*i*L - L);
			glVertex2f(0, A - 2*i*L - L);
		glEnd();
		glFlush();
	}
	// Draw candon
	glColor3f(0.234, 0.233, 0.430);
	glBegin(GL_POLYGON);
		glVertex2f(0.0, A);
		glVertex2f(D, A);
		glVertex2f(D, A-C);
		glVertex2f(0.0, A-C);
	glEnd();
	glFlush();
	// Draw stars
	glColor3f(1, 1, 1);
	int par = 0;
	for(double y = A-E; y > A - C + E/2; y -= F) {
		for(double x = G + par * H; x < D - G/2; x += 2*H) {
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(x, y);
				for(double ang = 0; sgn(ang - 2*Pi - Pi/5) < 0; ang += 2*Pi/5) {
					glVertex2f(x + r * sin(ang), y + r * cos(ang));
					glVertex2f(x + sr * sin(ang + Pi/5), y + sr * cos(ang + Pi/5));
				}
			glEnd();
			glFlush();
		}
		par ^= 1;
	}
	
}
void bonus(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(950, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("US Flag");
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, B, 0.0, A, -1.0, 1.0);
	glutDisplayFunc(drawPolygon);
	glutMainLoop();
}
int main(int argc,char** argv)
{
	
	bonus(argc, argv);
}
