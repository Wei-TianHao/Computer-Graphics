//
//  main.cpp
//  OpenGL_test
//
//  Created by WTH on 2017/3/5.
//
//
#include <bits/stdc++.h>
#include <GLUT/GLUT.h>
#include <set>
#include <assert.h>
using namespace std;
const int WIDTH = 1024;
const int HEIGHT = 768;
const int n = 20;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;


static int PPMWriteImage(const char* filePath, const unsigned char* buffer, unsigned int width, unsigned int height)
{
    FILE* fp;
    unsigned int total = width * height * 3;
    
    assert(filePath != NULL);
    assert(buffer != NULL);
    
    fp = fopen(filePath, "wb");
    if (fp == NULL)
    {
        fprintf(stderr, "Open target image file [%s] failed!\n", filePath);
        return 0;
    }
    
    /* Write file header */
    fprintf(fp, "P3 %u %u 255\n", width, height);
    
    /* Write file content */
    while (total-- > 0)
    {
        fprintf(fp, " %u", *buffer++);
    }
    
    fclose(fp);
    return 1;
}

void bresenham(int a, int b, set<tuple<int,int> >&q, tuple<int,int> func(int, int)) {
    char img[WIDTH][HEIGHT];
    for(int i = 0; i < WIDTH; i++)
        for(int j = 0; j < HEIGHT; j++)
            img[i][j] = ' ';
    int tx = 0, ty = 2*b-1;
    int d = a*a - 4*b*a*a;
    while(b*b*tx < a*a*ty) {
        q.insert(func(tx/2, (ty+1)/2));
        if(d < 0) {
            d = d + 4*b*b*tx - 4*b*b;
        }
        else {
            d = d + 4*b*b*tx - 4*b*b - 4*a*a*ty - 4*a;
            ty -= 2;
        }
        tx += 2;
    }
    if(a > b) q.insert(func(tx/2, (ty+1)/2));
}
set<tuple<int, int> > getQuarterPoint() {
    int a = WIDTH/4;
    int b = HEIGHT/4;
//    puts("Input Ra and Rb:");
//    scanf("%d%d", &a, &b);
    set<tuple<int, int> > q;
    bresenham(a, b, q, [](int x, int y){ return make_tuple(x, y); } );
    bresenham(b, a, q, [](int x, int y){ return make_tuple(y, x); } );
    return q;
}

void basic() {
    
    const unsigned char white[3] = { 0, 0, 0 };
    const unsigned char black[3] = { 255, 255, 255 };
    unsigned char testImage[HEIGHT][WIDTH][3];
    
    set<tuple<int, int> > q = getQuarterPoint();
    
    for(int j = 0; j < HEIGHT; j++) {
        for(int i = 0; i < WIDTH; i++){
            if (q.count(make_tuple(abs(i-WIDTH/2), abs(j-HEIGHT/2))))
                memcpy(testImage[j][i], black, sizeof(black));
            else
                memcpy(testImage[j][i], white, sizeof(white));
        }
    }
    PPMWriteImage("/Users/wth/Desktop/CG/test.ppm", &testImage[0][0][0], WIDTH, HEIGHT);
}


void drawPolygon() {
    glColor3f(1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    double a = WIDTH/4, b = HEIGHT/4;
    double w = WIDTH/2, h = HEIGHT/2;
    for(double i = 0; i < 2*Pi; i+=0.0001) {
        glVertex2f(a*cos(i)/w, b*sin(i)/h);
    }
    glEnd();
    glFlush();
    
}
void bonus(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ellipse");
    glutDisplayFunc(drawPolygon);
    glutMainLoop();
}
int main(int argc,char** argv)
{
    
    basic();
    
    bonus(argc, argv);
}
