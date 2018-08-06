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
#define A	3.5699
using namespace std;

random_device rnd;     // 非決定的な乱数生成器
mt19937 mt(rnd());  // メルセンヌ・ツイスタの32ビット版、引数は初期シード
uniform_real_distribution<> unif(0.0, 1.0);   // [0.0,1.0]上一様に分布させる
normal_distribution<> gauss(0.0, 1.0);   // 平均0.0、標準偏差1.0で分布させる

int winh = 800; int winw = 1000;
double yMin = -1.0; double yMax = 1.0;
int itrNum = 3000; int itrCnt = 0;
int seqNum = 1000;
double **lyp;
double *x;

double f(double u){
	return A * u * (1.0-u);
}
double df(double u){
	return A * (1.0 - 2.0*u);
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
	//	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();
	//	gluPerspective(45.0,
	//				   (double)width/height,
	//				   0.1,
	//				   100.0);
	//	glMatrixMode(GL_MODELVIEW);
	//	glLoadIdentity();
	//	gluLookAt(0.0, 0.0, 2.5,       //Position of Camera
	//			  0.0, 0.0, 0.0,        //Position of Object
	//			  0.0, 1.0, 0.0);       //Upward direction of Camera
}
void timer(int value) {
	if(itrCnt % 10 == 0) cout << "timer: " << itrCnt << endl;
	glutPostRedisplay();
	if(itrCnt < itrNum) glutTimerFunc(1 , timer , 0);
}

/*--Display func-------------------------------------------------------------------------*/
void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glutWireTeapot(0.5);
	
	glColor3d(0.0, 0.0, 0.0);	//Black
	glBegin(GL_LINES);
	//x-axis
	glVertex2d(-1.0, 0.0);
	glVertex2d(1.0, 0.0);
	//y-axis
	glVertex2d(0.0, -1.0);
	glVertex2d(0.0, 1.0);
	glEnd();
	
	//trajectoryの描画
	for(int k = 0; k < seqNum; k++){
		glColor3d(sin(0.1*k), cos(0.3*k), cos(0.2*k));
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < itrCnt; i++){
			glVertex2d(-1.0 + 2.0*i/itrNum, -1.0 + 2.0*(lyp[k][i]-yMin)/(yMax-yMin));
		}
		glEnd();
	}
	
	glFlush();
}


/*--Main func-------------------------------------------------------------------------*/
int main(int argc, char * argv[]) {
	/*--Initialize-------*/
	lyp = (double **)calloc(seqNum, sizeof(double *));
	for(int k = 0; k < seqNum; k++){ lyp[k] = (double *)calloc(itrNum, sizeof(double)); }
	x = (double *)calloc(seqNum, sizeof(double));
	for(int k = 0; k < seqNum; k++){ x[k] = unif(mt); }
	
	for(int k = 0; k < seqNum; k++){ lyp[k][0] = log(abs(df(x[k]))); }
	itrCnt = 1;
	//trajectoryの計算
	for(itrCnt = 1; itrCnt < itrNum; itrCnt++){
		for(int k = 0; k < seqNum; k++){
			x[k] = f(x[k]);
			lyp[k][itrCnt] = (1.0-1.0/(itrCnt+1)) * lyp[k][itrCnt-1] + log(abs(df(x[k])))/(itrCnt+1);
		}
	}
//	getchar();
	
	/*--Main loop-------*/
	glutInit(&argc, argv);
	glutInitWindowSize(winw, winh);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Lyapunov Exponents");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
//	glutIdleFunc(idle);
//	glutTimerFunc(400 , timer , 0);
	setup();
	glutMainLoop();
	
	
	for(int i = 0; i < seqNum; i++){ free(lyp[i]); }
	free(lyp); free(x);
	return 0;
}
