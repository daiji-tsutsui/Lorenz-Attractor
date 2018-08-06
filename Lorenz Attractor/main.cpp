//
//  main.cpp
//  Lorenz Attractor
//
//  Created by 筒井 大二 on 2018/08/06.
//  Copyright © 2018年 筒井 大二. All rights reserved.
//

#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <random>
#define P	10.0
#define R	28.0
#define B	8.0/3.0
#define EPS	0.001
#define T	100
using namespace std;

random_device rnd;     // 非決定的な乱数生成器
mt19937 mt(rnd());  // メルセンヌ・ツイスタの32ビット版、引数は初期シード
uniform_real_distribution<> unif(0.0, 1.0);   // [0.0,1.0]上一様に分布させる
normal_distribution<> gauss(0.0, 1.0);   // 平均0.0、標準偏差1.0で分布させる

int winh = 800;
int winw = 1000;
double xMin = -20.0;
double xMax = 20.0;
double yMin = -20.0;
double yMax = 20.0;
double zMin = 0.0;
double zMax = 40.0;
int itrNum = T / EPS;
double *x, *y, *z;
double inix = 1.0;
double iniy = 1.0;
double iniz = 1.0;
int r = 0;
int cnt = 0;

double dx(double x1, double y1, double z1){
	return -P * x1 + P * y1;
}
double dy(double x1, double y1, double z1){
	return -x1 * z1 + R * x1 - y1;
}
double dz(double x1, double y1, double z1){
	return x1 * y1 - B * z1;
}


/*--For OpenGL-------------------------------------------------------------------------*/
void idle(void){
	glutPostRedisplay();
}
void setup(void) {
	glClearColor(1.0, 0.99, 0.91, 1.0);       //White
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,
				   (double)width/height,
				   1.0,
				   100.0);
	gluLookAt(0.0, 0.0, 2.5,       //Position of Camera
			  0.0, 0.0, 0.0,        //Position of Object
			  0.0, 1.0, 0.0);       //Upward direction of Camera
	glMatrixMode(GL_MODELVIEW);
}
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(1 , timer , 0);
	if(cnt < itrNum) cnt++;
}
void mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			printf("left");
			break;
		case GLUT_MIDDLE_BUTTON:
			printf("middle");
			break;
		case GLUT_RIGHT_BUTTON:
			printf("right");
			break;
		default:
			break;
	}
	
	printf(" button is ");
	
	switch (state) {
		case GLUT_UP:
			printf("up");
			break;
		case GLUT_DOWN:
			printf("down");
			break;
		default:
			break;
	}
	
	printf(" at (%d, %d)\n", x, y);
}

/*--Display func-------------------------------------------------------------------------*/
void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.5,
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
	glRotated((double)r, 0.0, 1.0, 0.0);
	if (r >= 360) r = 0;

	glColor3d(0.0, 0.0, 0.0);	//Black
//	glutWireTeapot(0.5);
//	glBegin(GL_LINES);
//	//x-axis
//	glVertex2d(-1.0, 0.0);
//	glVertex2d(1.0, 0.0);
//	//y-axis
//	glVertex2d(0.0, -1.0);
//	glVertex2d(0.0, 1.0);
//	glEnd();
	
	//trajectoryの描画
	for(int i = 1; i < cnt; i++){
		glColor3d(0.2, cos((double)i/(double)itrNum), sin((double)i/(double)itrNum));
		glBegin(GL_LINES);
			glVertex3d(1.0-2.0*(x[i-1]-xMin)/(xMax-xMin),
					   1.0-2.0*(y[i-1]-yMin)/(yMax-yMin),
					   1.0-2.0*(z[i-1]-zMin)/(zMax-zMin));
			glVertex3d(1.0-2.0*(x[i]-xMin)/(xMax-xMin),
					   1.0-2.0*(y[i]-yMin)/(yMax-yMin),
					   1.0-2.0*(z[i]-zMin)/(zMax-zMin));
		glEnd();
	}
	
	glFlush();
}


/*--Main func-------------------------------------------------------------------------*/
int main(int argc, char * argv[]) {
	/*--trajectoryの計算-------*/
	x = (double *)calloc(itrNum, sizeof(double));
	y = (double *)calloc(itrNum, sizeof(double));
	z = (double *)calloc(itrNum, sizeof(double));
	x[0] = inix; y[0] = iniy; z[0] = iniz;
	
	for(int i = 1; i < itrNum; i++){
		x[i] = x[i-1] + EPS * dx(x[i-1],y[i-1],z[i-1]);
		y[i] = y[i-1] + EPS * dy(x[i-1],y[i-1],z[i-1]);
		z[i] = z[i-1] + EPS * dz(x[i-1],y[i-1],z[i-1]);
	}
	cout << itrNum << endl;
	
//	getchar();
	
	/*--Main loop-------*/
	glutInit(&argc, argv);
	glutInitWindowSize(winw, winh);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Lyapunov Exponents");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
//	glutIdleFunc(idle);
	glutTimerFunc(100 , timer , 0);
	setup();
	glutMainLoop();
	
	
	free(x); free(y); free(z);
	return 0;
}
