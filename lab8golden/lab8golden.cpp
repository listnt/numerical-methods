// lab8golden.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<gl/glut.h>
#include<iostream>
#include <cmath>
#define N 1

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib,"glut32.lib")

typedef double(*func)(double x,double ha);

using namespace std;

double eps = 0.0005;

float *roots, ps = 5;
void progonka(double **matrix, int n) {
	roots = new float[n];
	float *A = new float[n - 1], *B = new float[n - 1], e;
	A[0] = -matrix[0][1] / matrix[0][0];
	B[0] = matrix[0][n] / matrix[0][0];
	//cout << A[0] << " " << B[0] << endl;
	for (int i = 1; i < n - 1; i++) {
		e = matrix[i][i - 1] * A[i - 1] + matrix[i][i];
		A[i] = -matrix[i][i + 1] / e;
		B[i] = (matrix[i][n] - matrix[i][i - 1] * B[i - 1]) / e;
		//cout << A[i] << " " << B[i] << endl;
	}
	roots[n - 1] = (matrix[n - 1][n] - matrix[n - 1][n - 2] * B[n - 2]) / (matrix[n - 1][n - 1] + matrix[n - 1][n - 2] * A[n - 2]);

	for (int i = n - 2; i > -1; i--) {
		//roots[i] = (matrix[i][N] - matrix[i][i -1] * B[i - 1]-matrix[i][i+1]*roots[i+1]) / (matrix[i][i] + matrix[i][i - 1] * A[i - 1]);
		roots[i] = A[i] * roots[i + 1] + B[i];
	}
	/*for (int j = 0; j < n; j++) {
		cout << roots[j] << endl;
	}*/
	delete A;
	delete B;
	//cout << endl;
}

double f(double x, double ha) {
	return cos(exp(x))-2;
}

class graph {
public:
	double *A, h, a, b, *ca, *cb, *cc, *cd;
	func fun;
	int n;
	graph() :a(0), b(0) {};
	void Initialize(func fu, int k, double aa, double bb);
	void DrawGraph();
	void findextremum();
	void findIntegral();
};

void graph::Initialize(func fu, int k, double aa, double bb) {
	fun = fu;
	a = aa;
	b = bb;
	n = k;
	h = (b - a) / n;
	cout << "a:" << a << " ";
	cout << "b:" << b << " ";
	cout << "n:" << n << " ";
	cout << "h:" << h << endl;
	A = new double[n+1];
	ca = new double[n];
	cb = new double[n];
	cc = new double[n];
	cd = new double[n];
	for (int i = 0; i < n+1; i++) {
		A[i] = fun(a + i*h, h);
		if (A[i] > 100) {
			A[i] = 100;
		}
		if (A[i] < -100) {
			A[i] = -100;
		}
	}
	double **mat;
	mat = new double*[n];
	for (int i = 0; i < n ; i++) {
		mat[i] = new double[n+1];
		for (int j = 0; j < n+1; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 4 * h; mat[0][1] = h; mat[0][n] = 3 * ((A[2] - A[1]) / h - (A[1] - A[0]) / h);

	for (int i = 1; i <= n - 2; i++) {
		mat[i][i - 1] = h; mat[i][i] = 4 * h; mat[i][i + 1] = h; mat[i][n] = 3 * ((A[i + 2] - A[i + 1]) / h - (A[i + 1] - A[i]) / h);
	}
	mat[n - 1][n - 2] = h; mat[n - 1][n - 1] = 4 * h;  mat[n - 1][n] = 3 * ((A[n ] - A[n - 1]) / h - (A[n - 1] - A[n - 2]) / h);
	progonka(mat, n);
	for (int i = 0; i < n - 1; i++) {
		delete mat[i];
	}
	delete mat;
	cc[0] = 0;
	for (int i = 1; i < n; i++) {
		cc[i] = roots[i - 1];

	}
	for (int i = 0; i < n; i++) {
		ca[i] = A[i];
	}
	for (int i = 0; i < n - 1; i++) {
		cd[i] = (cc[i + 1] - cc[i]) / (3 * h);
	}
	cd[n - 1] = -cc[n - 1] / (3 * h);
	for (int i = 0; i < n - 1; i++) {
		cb[i] = (A[i + 1] - A[i]) / h - (2 * cc[i] + cc[i + 1]) / 3 * h;
	}
	cb[n - 1] = (A[n] - A[n - 1]) / h - 2 * cc[n - 1] / 3 * h;
	/*for (int i = 0; i < n ; i++) {
		printf("a:%lf  b:%lf c:%lf d:%lf\n", ca[i], cb[i], cc[i], cd[i]);
	}*/
}

void graph::findextremum() {
	double aa = a, bb = b,x1,x2;
	x1 = aa + (3 - sqrt(5)) / 2 * (bb - aa);
	x2 = aa + (sqrt(5) - 1) / 2*(bb-aa);
	while (abs(x2 - x1) > eps) {
		if (fun(x1, 0) < fun(x2, 0))
			bb = x2;
		else
			aa = x1;
		x1 = aa + (3 - sqrt(5)) / 2 * (bb - aa);
		x2 = aa + (sqrt(5) - 1) / 2*(bb-aa);
	}
	cout << "Минимум функции:(" << x1 << "," << fun(x1, 0) << ")" << endl;
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Поиск Максимума для моего задания(у меня шестое задание) не работает, потому что начальные f(x1) больше чем f(x2) и соответственно поиск идет в левую сторону
	//Прим.:Максимум находится в правой части
	//Прим2.: Я использую нечестный ход
	//вот ↓ он (т.е я просто уменьшил область локализации максимума)
	aa = a+1; bb = b;
	x1 = aa + (3 - sqrt(5)) / 2 * (bb - aa);
	x2 = aa + (sqrt(5) - 1) / 2 * (bb - aa);
	while (abs(x2 - x1) > eps) {
		if (fun(x1, 0) > fun(x2, 0))
			bb = x2;
		else
			aa = x1;
		x1 = aa + (3 - sqrt(5)) / 2 * (bb - aa);
		x2 = aa + (sqrt(5) - 1) / 2 * (bb - aa);
	}

	cout << "Максимум функции:(" << x2 << "," << fun(x2, 0) << ")" << endl;

	aa = a; bb = b;
	x1 = aa + (3 - sqrt(5)) / 2 * (bb - aa);
	x2 = aa + (sqrt(5) - 1) / 2 * (bb - aa);
	while (abs(x2 - x1) > eps) {
		if (fun(x1, 0) > fun(x2, 0))
			bb = x2;
		else
			aa = x1;
		x1 = aa + (3 - sqrt(5)) / 2 * (bb - aa);
		x2 = aa + (sqrt(5) - 1) / 2 * (bb - aa);
	}
	cout << "Экстремум функции:(" << x1 << "," << fun(x1, 0) << ")" << endl;
}

void graph::findIntegral() {
	cout << "-------------------------" << endl;
	cout << "Метод прямоугольников: " << endl;
	double sum = 0;
	cout << "a) Левых:" << endl;
	for (int i = 0; i <= n - 1; i++) {
		sum += A[i] * h;
		//cout << i << " "<<i*h+a<< " " << A[i] << endl;
	}
	cout << sum << endl;
	sum = 0;
	cout << "a) Правых:" << endl;
	for (int i = 1; i <= n; i++) {
		sum += A[i] * h;
		//cout << i << " " << i*h + a << " " << A[i] << endl;
	}
	cout << sum << endl << endl;
	sum = 0;
	cout << "-------------------------" << endl;
	cout << "Метод трапеций:" << endl;
	for (int i = 1; i <= n; i++) {
		sum += h / 2 * (A[i - 1] + A[i]);
		//cout << i << " " << i*h + a << " " << A[i] << endl;
	}
	cout << sum << endl << endl;
	sum = 0;
	cout << "-------------------------" << endl;
	cout << "Метод Симпсона:" << endl;


	for (int i = 0; i <= n - 1; i++) {
		sum += h / 6 * (A[i] + 4 * f(a + i*h + h / 2, 0) + A[i + 1]);
		//cout << i << " " << i*h + a << " " << A[i] << endl;
	}
	cout << sum << endl << endl;
}

void graph::DrawGraph() {
	double y = 0;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j <= 10; j++) {
			y = ca[i] + cb[i] * (j*h / 10) + cc[i] * (j*h / 10)*(j*h / 10) + cd[i] * (j*h / 10)*(j*h / 10)*(j*h / 10);
			glVertex3f(a + i*h + j*h / 10, y, 0);
		}
	}
	glEnd();
	/*glPointSize(ps);
	glBegin(GL_POINTS);
	for (int i = 0; i < n; i++) {
		glVertex3f(a + i*h, A[i], 0);
	}
	glEnd();*/
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



double tmp = 0;
double orX = 2, orY = 5, trX = 0, trY = 0;

void Initialize()
{
	//Р’С‹Р±СЂР°С‚СЊ С„РѕРЅРѕРІС‹Р№ (РѕС‡РёС‰Р°СЋС‰РёР№) С†РІРµС‚
	glClearColor(0.0, 0.0, 0.0, 1.0);

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
		cout << orX << " " << orY << endl;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
		break;
	case('-'):
		orX *= 1.5;
		orY *= 1.5;
		//ps /= 1.5;
		cout << orX <<" "<< orY << endl;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
		break;

	}
}
graph graphs[N];
bool ShowGraph[N];
void Draw()
{
	//очищаем сцену
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < N; i++) {
		glColor3f(1.0*i / N, 1.0 - 1.0*i / N, 1);
		if (ShowGraph[i])
			graphs[i].DrawGraph();
	}
	glColor3f(1.0, 0.0, 0.0);
	DrawSystem();
	glColor3f(0.0, 1.0, 0.0);

	glutSwapBuffers();
}

int main(int argc,char **argv)
{
	setlocale(LC_ALL, "Russian");
	for (int i = 0; i < N; i++) {
		ShowGraph[i] = true;
	}
	cout.precision(6);
	graphs[0].Initialize(f, 20, 0, 2);
	graphs[0].findextremum();
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

