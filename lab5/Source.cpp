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

double f(double x, double ha) {
	return sin(x)*exp(x);
	
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
private:
	double *A, h, a, b;
	int n;
public:
	graph() :a(0), b(0) {};
	void Initialize(double(*func)(double x, double ha), int k,double aa,double bb);
	void DrawGraph();
	void findIntegral(double from,double to);
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
	A = new double[n+1];
	for (int i = 0; i <= n; i++) {
		A[i] = func(a + i*h, h);
		if (A[i] > 100) {
			A[i] = 100;
		}
		if (A[i] < -100) {
			A[i] = -100;
		}
	}
}

void graph::DrawGraph() {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < n; i++) {
		glVertex3f(a + i*h, A[i], 0);
	}
	glEnd();
}

void graph::findIntegral(double from, double to) {
	cout << "-------------------------" << endl;
	cout << "Метод прямоугольников: " << endl;
	double sum = 0;
	cout << "a) Левых:" << endl;
	for (int i = 0; i<=n-1; i++) {
		sum += A[i] *h;
		//cout << i << " "<<i*h+a<< " " << A[i] << endl;
	}
	cout << sum << endl;
	sum = 0;
	cout << "a) Правых:" << endl;
	for (int i = 1; i<= n; i++) {
		sum += A[i] * h;
		//cout << i << " " << i*h + a << " " << A[i] << endl;
	}
	cout << sum << endl << endl;
	sum = 0;
	cout << "-------------------------" << endl;
	cout << "Метод трапеций:" << endl;
	for (int i = 1; i<= n; i++) {
		sum += h / 2 * (A[i - 1] + A[i]);
		//cout << i << " " << i*h + a << " " << A[i] << endl;
	}
	cout << sum << endl << endl;
	sum = 0;
	cout << "-------------------------" << endl;
	cout << "Метод Симпсона:" << endl;

	
	for (int i = 0; i <= n-1; i ++) {
		sum += h / 6 * (A[i] + 4 * f(a+i*h+h/2,0) + A[i + 1]);
		//cout << i << " " << i*h + a << " " << A[i] << endl;
	}
	cout << sum << endl << endl;
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
		trX -= 1 * orX / 2;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_RIGHT) {
		trX += 1 * orX / 2;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_UP) {
		trY += 1 * orY / 5;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_DOWN) {
		trY -= 1 * orY / 5;
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
		orX /= 2;
		orY /= 2;
		cout << orX << orY << endl;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
		break;
	case('-'):
		orX *= 2;
		orY *= 2;
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
	glBegin(GL_LINES);
	glVertex3f(1,0,0);
	glVertex3f(1, 100, 0);
	glVertex3f(2, 0, 0);
	glVertex3f(2, 100, 0);
	glEnd();

	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");
	for (int i = 0; i < N; i++) {
		ShowGraph[i] = true;
	}
	cout.precision(6);
	graphs[0].Initialize(f, 100,1,2);
	//graphs[1].Initialize(fpc, 280);
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


								//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	graphs[0].findIntegral(1,2); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
								 //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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