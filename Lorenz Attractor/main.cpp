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
#define T	1000
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
double t = 0.0; double t0 = 0.0;
double s = 0.0; double s0 = 0.0;
double r = 2.5;
int cnt = 0;
int btnFlg = 0;
double px, py;

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
	glutTimerFunc(30 , timer , 0);
	if(cnt < itrNum) cnt++;
}
void mouse(int button, int state, int cx, int cy)
{
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN){
			btnFlg = 1;
			px = cx; py = cy;
		}else if(state == GLUT_UP){
			btnFlg = 0;
			t0 = t;
			s0 = s;
		}
	}
}
void motion(int x, int y){
	if(btnFlg){
		t = t0 - 0.005 * (x - px);
		s = max(min(s0 + 0.005 * (y - py), M_PI/2), -M_PI/2);
		glutPostRedisplay();
	}
}

/*--Display func-------------------------------------------------------------------------*/
void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(r*cos(s)*sin(t), r*sin(s), r*cos(s)*cos(t),
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
//	glRotated(t, 0.0, 1.0, 0.0);
//	if (t >= 360){ t = 0.0; }else{ t += 2.0; }

	//x-axis
	glColor3d(1.0, 0.0, 0.0);	//Black
	glBegin(GL_LINES);
		glVertex3d(-2.0, 0.0, 0.0);
		glVertex3d(2.0, 0.0, 0.0);
	glEnd();
	//y-axis
	glColor3d(0.0, 1.0, 0.0);	//Black
	glBegin(GL_LINES);
		glVertex3d(0.0, -2.0, 0.0);
		glVertex3d(0.0, 2.0, 0.0);
	glEnd();
	//z-axis
	glColor3d(.0, 0.0, 1.0);	//Black
	glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, -2.0);
		glVertex3d(0.0, 0.0, 2.0);
	glEnd();
	
	//trajectoryの描画
//	for(int i = 1; i < cnt; i++){
	for(int i = itrNum*9/10; i < itrNum; i++){
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
	
	glutSwapBuffers();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Lorenz Attractor");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
//	glutIdleFunc(idle);
//	glutTimerFunc(100 , timer , 0);
	setup();
	glutMainLoop();
	
	
	free(x); free(y); free(z);
	return 0;
}
