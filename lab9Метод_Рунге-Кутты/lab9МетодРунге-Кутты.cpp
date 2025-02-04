// lab9МетодРунге-Кутты.cpp : Defines the entry point for the console application.
//

#include"stdafx.h"
#include<gl/glut.h>
#include<cmath>
#include<iostream>
#include<ctime>
#include<fstream>
#pragma warning(disable: 4996)				//отвлючаем ошибку 4996

#define pi 3.14159265359					//число ПИ
#define N 2
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib,"glut32.lib")


typedef float (*func)(float x, float y, float z);
using namespace std;

float f(float x, float y, float z) {
	return log(2 * x + sqrt(4 * x*x + z*z)) - 2 * x*y + z - x - 1;
}

float g(float x, float y, float z) {
	return -sqrt(4 * x*x + y*y);
}



double tmp = 0;
double orX = 2, orY = 5, trX = 0, trY = 0;

void processSpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_F1)
		exit(1);
	if (key == GLUT_KEY_LEFT) {
		trX -= 1 * orX / 4;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_RIGHT) {
		trX += 1 * orX / 4;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_UP) {
		trY += 1 * orY / 10;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_DOWN) {
		trY -= 1 * orY / 10;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_HOME) {
		trY = 0;
		trX = 0;
		orX = 2;
		orY = 5;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}

}

void processNormalKeys(unsigned char key, int x, int y) {
	switch (key) {
	case('+'):
		orX /= 1.5;
		orY /= 1.5;
		//ps *= 1.5;
		cout << orX << orY << endl;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
		break;
	case('-'):
		orX *= 1.5;
		orY *= 1.5;
		//ps /= 1.5;
		cout << orX << orY << endl;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
		break;

	}
}
void Initialize()
{
	//Р’С‹Р±СЂР°С‚СЊ С„РѕРЅРѕРІС‹Р№ (РѕС‡РёС‰Р°СЋС‰РёР№) С†РІРµС‚
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//РЈСЃС‚Р°РЅРѕРІРёС‚СЊ РїСЂРѕРµРєС†РёСЋ
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-orX, orX, -orY, orY, -10, 10);
	//gluPerspective(50, 1, 200, 2000);
	/*glMatrixMode(GL_MODELVIEW);*/
	/*gluLookAt(0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 10.0f,
	0.0, 1.0, 0.0
	);*/


}
void DrawSystem() {
	glBegin(GL_LINES);
	glVertex3f(-100.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	glVertex3f(0.0, -100.0, 0.0);
	for (int i = -100; i < 101; i++) {
		glVertex3f(-0.1, i, 0);
		glVertex3f(0.1, i, 0);
		glVertex3f(i*0.4, -0.25, 0);
		glVertex3f(i*0.4, 0.25, 0);
	}
	glEnd();
}

GLfloat *array1, *array2, *array4;
GLint sizea;

void Draw()
{
	//очищаем сцену
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < sizea; i++){
		glVertex3f(i*0.01, array1[i], 0);
	}
	glEnd();
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < sizea; i++) {
		glVertex3f(i*0.01, array2[i], 0);
	}
	glEnd();
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < sizea; i++) {
		glVertex3f(i*0.01, array4[i], 0);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0);
	DrawSystem();

	glutSwapBuffers();
}

int main(int argc,char **argv)
{
	ofstream file3("wikipedia.txt");
	double Xo, Yo, Y1, Zo, Z1;
	double k1, k2, k4, k3, h;
	double q1, q2, q4, q3;
	/*
	*Начальные условия
	*/
	Xo = 0;
	Yo = 0.5;
	Zo = 1;

	h = 0.01; // шаг

	for (Xo+=h; Xo<=1.0; Xo += h) {

		k1 = h * g(Xo, Yo, Zo);
		q1 = h * f(Xo, Yo, Zo);

		k2 = h * g(Xo + h / 2.0, Yo + q1 / 2.0, Zo + k1 / 2.0);
		q2 = h * f(Xo + h / 2.0, Yo + q1 / 2.0, Zo + k1 / 2.0);

		k3 = h * g(Xo + h / 2.0, Yo + q2 / 2.0, Zo + k2 / 2.0);
		q3 = h * f(Xo + h / 2.0, Yo + q2 / 2.0, Zo + k2 / 2.0);

		k4 = h * g(Xo + h, Yo + q3, Zo + k3);
		q4 = h * f(Xo + h, Yo + q3, Zo + k3);

		Z1 = Zo + (k1 + 2.0*k2 + 2.0*k3 + k4) / 6.0;
		Y1 = Yo + (q1 + 2.0*q2 + 2.0*q3 + q4) / 6.0;
		file3 << "(" << Xo << ";";
		file3 << Y1 << ")" << endl;
		Yo = Y1;
		Zo = Z1;
	}

	ofstream file1("1.txt");
	ofstream file("2.txt");
	ofstream file2("4.txt");
	double X0=0,XN=1.0;

	sizea = (XN - X0) / h;
	array1 = new GLfloat[sizea];
	array2 = new GLfloat[sizea];
	array4 = new GLfloat[sizea];

	func fun[N] = {&f,&g};
	double U1[N] = { 0.5,1 };
	double U2[N] = { 0.5,1 };
	double U4[N] = { 0.5,1 };
	double K[N][4];
	double temp = 0;
	int arc = 0;

	array1[0] = 0.5;
	array2[0] = 0.5;
	array4[0] = 0.5;

	while (X0 <= XN) {
		X0 += h;
		arc++;
		temp = U1[0];
		U1[0] = U1[0] + h*fun[0](X0, U1[0], U1[1]);
		U1[1] = U1[1] + h*fun[1](X0, temp, U1[1]);
		file1 << "(" << X0 << ";";
		file1 << U1[0] << ")" << endl;
		array1[arc] = U1[0];

		file2 << "(" << X0 << ";";
		for (int i = 0; i < N; i++) {
			K[i][0] = h*fun[i](X0, U4[0], U4[1]);
		}
		for (int j = 1; j < 3; j++)
			for (int i = 0; i < N; i++)
				K[i][j] = h*fun[i](X0 + h / 2.0, U4[0] + K[0][j - 1] / 2.0, U4[1] + K[1][j - 1] / 2.0);
		for (int i = 0; i < N; i++) {
			K[i][3] = h*fun[i](X0+h, U4[0]+K[0][2], U4[1]+K[1][2]);
		}

		//K[0][0] = h*fun[0](X0, U4[0], U4[1]);
		//K[1][0] = h*fun[1](X0, U4[0], U4[1]);

		//K[0][1] = h * fun[0](X0 + h / 2.0, U4[0] + K[0][0] / 2.0, U4[1] + K[1][0] / 2.0);
		//K[1][1] = h * fun[1](X0 + h / 2.0, U4[0] + K[0][0] / 2.0, U4[1] + K[1][0] / 2.0);

		//K[0][2] = h * fun[0](X0 + h / 2.0, U4[0] + K[0][1] / 2.0, U4[1] + K[1][1] / 2.0);
		//K[1][2] = h * fun[1](X0 + h / 2.0, U4[0] + K[0][1] / 2.0, U4[1] + K[1][1] / 2.0);

		//K[0][3] = h * fun[0](X0 + h , U4[0] + K[0][2], U4[1] + K[1][2]);
		//K[1][3] = h * fun[1](X0 + h , U4[0] + K[0][2], U4[1] + K[1][2]);

		for (int i = 0; i < N; i++) {
			U4[i] = U4[i] + (K[i][0] + 2 * K[i][1] + 2 * K[i][2] + K[i][3])/6;
		}
		file2 << U4[0] << ")" << endl;

		array4[arc] = U4[0];

		file << "(" << X0 << ";";
		temp = U2[0];
		U2[0] = U2[0] + h*fun[0](X0 + h / 2, U2[0] + h / 2 * fun[0](X0, U2[0], U2[1]), U2[1] + h / 2 * fun[0](X0, U2[0], U2[1]));
		U2[1] = U2[1] + h*fun[1](X0 + h / 2, temp + h / 2 * fun[1](X0, temp, U2[1]), U2[1] + h / 2 * fun[1](X0, temp, U2[1]));
		file << U2[0] << ")" << endl;
		array2[arc] = U2[0];

	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);		//Указываем размер окна
	glutInitWindowPosition(100, 100);	//Позиция окна
	glutCreateWindow("Graphs");		//Имя окна
	Initialize();						//Вызов функции Initialize

	glutDisplayFunc(Draw);				//Вызов функции отрисовки
	glutIdleFunc(Draw);
	//Функция обработки нормальных клавиш, т.е. клавиш у которых есть ASCII кода. Например: "ц" , "а"  и др.
	//Определенны как 'char'
	glutKeyboardFunc(processNormalKeys);
	//Функция обработки специальных клавиш, т.е. клавиш у которых нет ASCII кода. Например:стрелки, Esc, F1,F2 и др.
	//Определенны в Константе GLUT_KEY...
	glutSpecialFunc(processSpecialKeys);

	/*glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	*/
	glutMainLoop(); //Основной цикл
    return 0;
}

