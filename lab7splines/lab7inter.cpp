#include"stdafx.h"
#include<gl/glut.h>
#include<cmath>
#include<iostream>
#include<ctime>
#pragma warning(disable: 4996)				//отвлючаем ошибку 4996

#define pi 3.14159265359					//число ПИ
#define N 2
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib,"glut32.lib")

using namespace std;
float *roots,ps=5;
void progonka(double **matrix, int n) {
	roots=new float[n];
	float *A=new float[n-1], *B=new float[n - 1], e;
	A[0] = -matrix[0][1] / matrix[0][0];
	B[0] = matrix[0][n] / matrix[0][0];
	cout << A[0] << " " << B[0] << endl;
	for (int i = 1; i < n - 1; i++) {
		e = matrix[i][i - 1] * A[i - 1] + matrix[i][i];
		A[i] = -matrix[i][i + 1] / e;
		B[i] = (matrix[i][n] - matrix[i][i - 1] * B[i - 1]) / e;
		cout << A[i] << " " << B[i] << endl;
	}
	roots[n - 1] = (matrix[n - 1][n] - matrix[n - 1][n - 2] * B[n - 2]) / (matrix[n - 1][n - 1] + matrix[n - 1][n - 2] * A[n - 2]);

	for (int i = n - 2; i > -1; i--) {
		//roots[i] = (matrix[i][N] - matrix[i][i -1] * B[i - 1]-matrix[i][i+1]*roots[i+1]) / (matrix[i][i] + matrix[i][i - 1] * A[i - 1]);
		roots[i] = A[i] * roots[i + 1] + B[i];
	}
	for (int j = 0; j < n; j++) {
		cout << roots[j] << endl;
	}
	delete A;
	delete B;
	cout << endl;
}

double f(double x, double ha) {
	//return sin(x)*exp(x);
	return 10 * x*x*x*x*x*x*x*x*x + x*x*x*x*x*x*x*x - 5 * x*x*x*x*x*x*x + 4 * x*x*x + 11;
	
}

double fpl(double x, double ha) {
	return (f(x, ha) - f(x - ha, ha)) / ha;
	
}
double fpr(double x, double ha) {
	return  (f(x + ha, ha) - f(x, ha)) / ha;
}

double fpc(double x, double ha) {
	return (f(x + ha, ha) - f(x - ha, ha)) / (2 * ha);
}
double fp2c(double x, double ha) {
	return (f(x + ha, ha) - 2 * f(x, ha) + f(x - ha, ha)) / (ha*ha);
}

double fp3c(double x, double ha) {
	return (f(x + 2 * ha, ha) - 2 * f(x + ha, ha) + 2 * f(x - ha, ha) - f(x - 2 * ha, ha)) / (2 * ha*ha*ha);
}



class graph {
public:
	double *A, h, a, b,*ca,*cb,*cc,*cd;
	int n;
	graph() :a(0), b(0) {};
	void Initialize(double(*func)(double x, double ha), int k,double aa,double bb);
	void DrawGraph();
};

void graph::Initialize(double(*func)(double x, double ha), int k,double aa,double bb) {
	a = aa;
	b = bb;
	n = k;
	h = (b - a) / n;
	cout << "a:" << a << " ";
	cout << "b:" << b << " ";
	cout << "n:" << n << " ";
	cout << "h:" << h << endl;
	A = new double[n ];
	ca = new double[n -1];
	cb = new double[n -1];
	cc = new double[n -1];
	cd = new double[n - 1];
	for (int i = 0; i <= n; i++) {
		A[i] = func(a + i*h, h);
		if (A[i] > 100) {
			A[i] = 100;
		}
		if (A[i] < -100) {
			A[i] = -100;
		}
	}
	double **mat;
	mat = new double*[n-1];
	for (int i = 0; i < n-1; i++) {
		mat[i] = new double[n];
		for (int j = 0; j < n; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 4 * h; mat[0][1] = h; mat[0][n-1] = 3 * ((A[2] - A[1]) / h - (A[1] - A[0]) / h);
	
	for (int i = 1; i <= n-3; i++) {
		mat[i][i-1] = h; mat[i][i] = 4 * h; mat[i][i + 1] = h; mat[i][n-1] = 3 * ((A[i+2] - A[i+1]) / h - (A[i+1] - A[i]) / h);
	}
	mat[n-2][n-3] = h; mat[n-2][n-2] = 4 * h;  mat[n-2][n-1] = 3 * ((A[n-1] - A[n-2]) / h - (A[n-2] - A[n-3]) / h);
	progonka(mat, n-1);
	for (int i = 0; i < n - 2; i++) {
		delete mat[i];
	}
	delete mat;
	cc[0] = 0;
	for (int i = 1; i < n - 1; i++) {
		cc[i] = roots[i-1];
		
	}
	for (int i = 0; i < n - 1; i++) {
		ca[i] = A[i];
	}
	for (int i = 0; i < n - 2; i++) {
		cd[i] = (cc[i + 1] - cc[i]) / (3 * h);
	}
	cd[n - 2] = -cc[n - 2] / (3 * h);
	for (int i = 0; i < n - 2; i++) {
		cb[i] = (A[i + 1] - A[i]) / h - (2 * cc[i] + cc[i + 1]) / 3 * h;
	}
	cb[n - 2] = (A[n - 1] - A[n - 2]) / h - 2 * cc[n - 2] / 3 * h;
	for (int i = 0; i < n - 1; i++) {
		printf("a:%lf  b:%lf c:%lf d:%lf\n", ca[i], cb[i], cc[i], cd[i]);
	}
}

void graph::DrawGraph() {
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(a + i*h, A[i], 0);
	}
	glEnd();
	double y = 0;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j <= 10; j++) {
			y = ca[i] + cb[i] * (j*h / 10) + cc[i] * (j*h / 10)*(j*h / 10) + cd[i] * (j*h / 10)*(j*h / 10)*(j*h / 10);
			glVertex3f(a + i*h+j*h/10, y, 0);
		}
	}
	glEnd();
	glPointSize(ps);
	glBegin(GL_POINTS);
	for (int i = 0; i < n; i++) {
		glVertex3f(a + i*h, A[i], 0);
	}
	glEnd();
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
double eps = 0.0005;


graph graphs[N];
bool ShowGraph[N];
double tmp = 0;
double orX = 2, orY = 5, trX = 0, trY = 0;

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
	case('1'):
		ShowGraph[0] = (!ShowGraph[0]);
		cout << ShowGraph[0] << endl;
		break;
	case('2'):
		ShowGraph[1] = (!ShowGraph[1]);
		cout << ShowGraph[1] << endl;
		break;
	case('3'):
		ShowGraph[2] = (!ShowGraph[2]);
		cout << ShowGraph[2] << endl;
		break;
	case('4'):
		ShowGraph[3] = (!ShowGraph[3]);
		cout << ShowGraph[3] << endl;
		break;
	case('5'):
		ShowGraph[4] = (!ShowGraph[4]);
		cout << ShowGraph[4] << endl;
		break;
	case('6'):
		ShowGraph[5] = (!ShowGraph[5]);
		cout << ShowGraph[5] << endl;
		break;
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

//Основная фунция отрисовки.Выполняется каждый кадр
void Draw()
{
	//очищаем сцену
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < N; i++) {
		glColor3f(1.0*i / (N - 1), 1.0 - 1.0*i / (N - 1), 1);
		if (ShowGraph[i])
			graphs[i].DrawGraph();
	}
	glColor3f(1.0, 0.0, 0.0);
	DrawSystem();
	glColor3f(0.0, 1.0, 0.0);

	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");
	for (int i = 0; i < N; i++) {
		ShowGraph[i] = true;
	}
	cout.precision(6);
	graphs[0].Initialize(f, 15,-1,1);
	//graphs[1].Initialize(f, 200,-1,1);
	//graphs[2].Initialize(fpr, 280);
	//graphs[3].Initialize(fpc, 280);
	//graphs[4].Initialize(fp2c, 280);
	//graphs[5].Initialize(fp3c, 280);

	cout << "График 1-функция" << endl;
	//cout << "График 2-первая производная слева" << endl;
	//cout << "График 3-первая производная справа" << endl;
	//cout << "График 2-первая производная посередине" << endl;
	//cout << "График 5-вторая поизводная" << endl;
	//cout << "График 6-третья производная" << endl;



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