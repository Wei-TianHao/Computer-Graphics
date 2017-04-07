# Solar-System



### Compile:

g++ -std=c++11 solar.cc -o solar -framework GLUT -framework OpenGL -framework Cocoa



### Run:

./solar



### Control:

W: move front

S: move back

A: move left

D: move right

X: move down

Space: move up

X and Space are not relative direction, but along the Z-axis.



### How the Program Works

#### The Definitions:

I defined a 3-dimensional vector struct and a celestial body struct. 

u: the position of the observer

v: the view vector relative to the observer

n: the upward vector

Mx:  x-coordinate of mouse

My:  y-coordinate of mouse

```c++
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
```

#### The callback functions:

1. ##### Mouse Callback Function:

   Set the moving speed in proportion to the relative positon to the center of window.

   If the mouse is out of the window or is very near the center, the movement will slow down to zero in exponential speed with a factor of 0.8.

   ```c++
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
   ```

   ​

   ​

2. ##### Keyboard Callback Function:

   Use this trick can enable the movement in synthetic directions, like up-left.

   ```c++
   void keyboardCB(unsigned char key, int x, int y) {
       keys[key] = true;
   	glutPostRedisplay();
   }
   void keyboardUpCB(unsigned char key, int x, int y) {
       keys[key] = false;
   	glutPostRedisplay();
   }
   ```

   ​

   ​

3. ##### Reshape Function:

   Ensure the object doesn't stretch at reshaping.

   ```c++
   void reshape (int w, int h)
   {
   	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   	WIDTH = w;
   	HEIGHT = h;
   	glMatrixMode (GL_PROJECTION);
   	glLoadIdentity ();
   	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 3000.0);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
   	vec3 ov = u+v;
   	gluLookAt(u.x, u.y, u.z,  ov.x, ov.y, ov.z,  n.x, n.y, n.z);
   }
   ```

   ​

#### Display Function:

1. ##### Movement Control:

   Set the position and view direction.

   ```c++
   //=========== movement control ============
   	vec3 t = norm(v ^ n);
   	v = norm(v + My*norm(n) + Mx*t);
   	if(keys['w']) u = u + 0.5 * v;
   	if(keys['s']) u = u - 0.5 * v;
   	if(keys['d']) u = u + 0.5 * t;
   	if(keys['a']) u = u - 0.5 * t;
   	if(keys[' ']) u = u + 0.5 * n;
   	if(keys['x']) u = u - 0.5 * n;
   	vec3 ov = u+v;
   	gluLookAt(u.x, u.y, u.z,  ov.x, ov.y, ov.z,  n.x, n.y, n.z);
   ```

2. ##### Set Light Source and Zero Matrix:

   Set the point light source to cast shadow on planet.

   The light source is inside the sun.


   ```c++
   	GLfloat pos[] = {0., 0., 0.};
   	GLfloat zero[] = {0.05, 0.05, 0.05, 1.0};
   	glLightfv(GL_LIGHT0, GL_POSITION, pos);
   ```

3. ##### Draw Sun:

   Set the sun to be a emission body and without any reflection.

   ```c++
   //=============== draw sun ================
   	GLfloat sun_emi[] = {1., .5, .3, 1.0};
   	glMaterialfv(GL_FRONT, GL_EMISSION, sun_emi);
   	glMaterialfv(GL_FRONT, GL_AMBIENT, zero);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
   	GLUquadric *quad;
   	quad = gluNewQuadric();
   	gluSphere(quad, sun.r, 100, 20);
   ```

4. ##### Draw Earth:

   Push matrix to ensure a hierarchy structure.

   The earth orbit the sun in a uniform speed and can only diffuse light.

   ```c++
   	glPushMatrix();
   		GLfloat earth_amb[] = {0., 0.4, 1., 1.0};
   		glMaterialfv(GL_FRONT, GL_EMISSION, zero);
   		glMaterialfv(GL_FRONT, GL_AMBIENT, zero);
   		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_amb);
   		glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
   		quad = gluNewQuadric();
   		glRotatef(earth.v * glutGet(GLUT_ELAPSED_TIME), earth.ax.x, earth.ax.y, earth.ax.z);
   		glTranslatef(earth.dis, 0, 0);
   		gluSphere(quad, earth.r, 100, 20);
   ```

5. ##### Draw Moon:

   Push matrix too.

   The moon orbit the earth in a uniform speed and can only diffuse light.

   The oribit is perpendicular to earth's.

   ```c++
   		glPushMatrix();
   			GLfloat moon_amb[] = {0.6, 0.6, 0.6, 1.0};
   			glMaterialfv(GL_FRONT, GL_EMISSION, zero);
   			glMaterialfv(GL_FRONT, GL_AMBIENT, zero);
   			glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_amb);
   			glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
   			quad = gluNewQuadric();
   			glRotatef(moon.v * glutGet(GLUT_ELAPSED_TIME), moon.ax.x, moon.ax.y, moon.ax.z);
   			glTranslatef(0, moon.dis, 0);
   			gluSphere(quad, moon.r, 100, 20);
   		glPopMatrix();
   	glPopMatrix();
   ```



#### Screen Shot:

![pic1](/Users/wth/Desktop/CG/Solar-System/pic1.png)

![pic1](/Users/wth/Desktop/CG/Solar-System/pic2.png)