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
#define T	60
using namespace std;

random_device rnd;     // 非決定的な乱数生成器
mt19937 mt(rnd());  // メルセンヌ・ツイスタの32ビット版、引数は初期シード
uniform_real_distribution<> unif(0.0, 1.0);   // [0.0,1.0]上一様に分布させる
normal_distribution<> gauss(0.0, 1.0);   // 平均0.0、標準偏差1.0で分布させる

int lorenzFlg = 2;	// 0: 回転, 1: グリグリ, 2: 墨流し
int winh = 800;
int winw = 1000;
double xMin = -20.0;
double xMax = 20.0;
double yMin = -20.0;
double yMax = 20.0;
double zMin = 0.0;
double zMax = 40.0;
int itrNum = T / EPS;
int seqNum = 8000;
double **x, **y, **z;
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
	if(lorenzFlg == 0 || lorenzFlg == 1){
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
	}else if(lorenzFlg == 2){
		glLoadIdentity();
	}
}
void timer(int value) {
	glutPostRedisplay();
	if(lorenzFlg == 0){
		glutTimerFunc(30 , timer , 0);
	}else if(lorenzFlg == 2){
		glutTimerFunc(10 , timer , 0);
	}
	if(cnt < itrNum-10){
		cnt += 10;
		if(cnt % 1000 == 0) cout << cnt << endl;
	}
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
void motion(int cx, int cy){
	if(lorenzFlg == 1 && btnFlg){
		t = t0 - 0.005 * (cx - px);
		s = max(min(s0 + 0.005 * (cy - py), M_PI/2), -M_PI/2);
		glutPostRedisplay();
	}
}

/*--Display func-------------------------------------------------------------------------*/
void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	if(lorenzFlg == 0){
		gluLookAt(0.0, 0.0, 2.5,
				  0.0, 0.0, 0.0,
				  0.0, 1.0, 0.0);
		glRotated(t, 0.0, 1.0, 0.0);
		if (t >= 360){ t = 0.0; }else{ t += 2.0; }
	}else if(lorenzFlg == 1){
		gluLookAt(r*cos(s)*sin(t), r*sin(s), r*cos(s)*cos(t),
				  0.0, 0.0, 0.0,
				  0.0, 1.0, 0.0);
	}else if(lorenzFlg == 2){
		glOrtho(-1.5, 1.5, -1.2, 1.8, -1.5, 1.5);
		glRotated(90.0, 1.0, 0.0, 0.0);
	}
	
	if(lorenzFlg == 0 || lorenzFlg == 1){
		//x-axis
		glColor3d(1.0, 0.0, 0.0);	//Red
		glBegin(GL_LINES);
			glVertex3d(-2.0, 0.0, 0.0);
			glVertex3d(2.0, 0.0, 0.0);
		glEnd();
		//y-axis
		glColor3d(0.0, 1.0, 0.0);	//Green
		glBegin(GL_LINES);
			glVertex3d(0.0, -2.0, 0.0);
			glVertex3d(0.0, 2.0, 0.0);
		glEnd();
		//z-axis
		glColor3d(0.0, 0.0, 1.0);	//Blue
		glBegin(GL_LINES);
			glVertex3d(0.0, 0.0, -2.0);
			glVertex3d(0.0, 0.0, 2.0);
		glEnd();
		
		//trajectoryの描画
		for(int k = 0; k < seqNum; k++){
		//	for(int i = 0; i < cnt; i++){
			for(int i = itrNum*9/10; i < itrNum; i++){
				glColor3d((double)k/seqNum, cos((double)i/itrNum), sin((double)i/itrNum));
				glBegin(GL_LINES);
					glVertex3d(1.0-2.0*(x[k][i-1]-xMin)/(xMax-xMin),
							   1.0-2.0*(y[k][i-1]-yMin)/(yMax-yMin),
							   1.0-2.0*(z[k][i-1]-zMin)/(zMax-zMin));
					glVertex3d(1.0-2.0*(x[k][i]-xMin)/(xMax-xMin),
							   1.0-2.0*(y[k][i]-yMin)/(yMax-yMin),
							   1.0-2.0*(z[k][i]-zMin)/(zMax-zMin));
				glEnd();
			}
		}
	}else if(lorenzFlg == 2){
		//particlesの描画
		glPointSize(3.0);
		glColor3d(0.5, 0.1, 0.1);
		glBegin(GL_POINTS);
				for(int k = 0; k < seqNum; k++){
					glVertex3d(1.0-2.0*(x[k][cnt]-xMin)/(xMax-xMin),
							   1.0-2.0*(y[k][cnt]-yMin)/(yMax-yMin),
							   1.0-2.0*(z[k][cnt]-zMin)/(zMax-zMin));
				}
		glEnd();
	}
	
	glutSwapBuffers();
}


/*--Main func-------------------------------------------------------------------------*/
int main(int argc, char * argv[]) {
	/*--trajectoryの計算-------*/
	if(lorenzFlg == 0 || lorenzFlg == 1) seqNum = 1;
	cout << "itrNum: " << itrNum << endl
		<< "seqNum: " << seqNum << endl;
	
	x = (double **)calloc(seqNum, sizeof(double *));
	y = (double **)calloc(seqNum, sizeof(double *));
	z = (double **)calloc(seqNum, sizeof(double *));
	for(int k = 0; k < seqNum; k++){
		x[k] = (double *)calloc(itrNum, sizeof(double));
		y[k] = (double *)calloc(itrNum, sizeof(double));
		z[k] = (double *)calloc(itrNum, sizeof(double));
		x[k][0] = inix + unif(mt) * 0.001;
		y[k][0] = iniy + unif(mt) * 0.001;
		z[k][0] = iniz + unif(mt) * 0.001;
	}
	
	for(int k = 0; k < seqNum; k++){
		for(int i = 1; i < itrNum; i++){
			x[k][i] = x[k][i-1] + EPS * dx(x[k][i-1],y[k][i-1],z[k][i-1]);
			y[k][i] = y[k][i-1] + EPS * dy(x[k][i-1],y[k][i-1],z[k][i-1]);
			z[k][i] = z[k][i-1] + EPS * dz(x[k][i-1],y[k][i-1],z[k][i-1]);
		}
		if(k % 100 == 0) cout << k << " / " << seqNum << endl;
	}
	
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
	if(lorenzFlg == 0 || lorenzFlg == 2) glutTimerFunc(100 , timer , 0);
	setup();
	glutMainLoop();
	
	for(int k = 0; k < seqNum; k++){
		free(x[k]); free(y[k]); free(z[k]);
	}
	free(x); free(y); free(z);
	return 0;
}
