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

float f(float x,float ha) {
	//return (x*x) / (2 * sqrt(1 - 3 * x*x*x*x));
	return x*x*x - 7 * sin(x) + x + 4;
}

float fpl(float x,float ha) {
	return (f(x,ha) - f(x - ha,ha)) / ha;

}
float fpr(float x, float ha) {
	return  (f(x+ha, ha) - f(x, ha)) / ha;
}

float fpc(float x, float ha) {
	return (f(x + ha, ha) - f(x-ha, ha)) / (2*ha);
}
float fp2c(float x, float ha) {
	return (f(x + ha, ha) - 2 * f(x, ha) + f(x - ha, ha)) / (ha*ha);
}

float fp3c(float x, float ha) {
	return (f(x + 2*ha, ha) - 2 * f(x+ha, ha) + 2*f(x - ha, ha)-f(x-2*ha,ha)) / (2*ha*ha*ha);
}

float aa=-5, bb=5;

class graph {
private:
	float *A,h,a,b;
	int n;
public:
	graph() :a(aa), b(bb) {};
	void Initialize(float(*func)(float x,float ha),int k);
	void DrawGraph();
};

void graph::Initialize(float(*func)(float x,float ha), int k) {
	n = k;
	h = (b - a) / n;
	cout << "a:" << a << " ";
	cout << "b:" << b << " ";
	cout << "n:" << n << " ";
	cout << "h:" << h << endl;
	A = new float[n];
	for (int i = 0; i < n; i++) {
		A[i] = func(a+i*h,h);
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
	for (int i=0; i < n; i++) {
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
		glVertex3f(-0.1,i,0);
		glVertex3f(0.1,i,0);
		glVertex3f(i*0.4, -0.25, 0);
		glVertex3f(i*0.4, 0.25, 0);
	}
	glEnd();
}
float eps = 0.0005;
void findroot(float ac, float bc) {
	float x=-3;
	cout << "Метод дихотомии: ";
	if ((f(ac, 0) * f(bc, 0)) > 0) {
		cout << "решения нет";
	}
	else {
		while (true) {
			x = (bc + ac) / 2;
			if ((f(x, 0) * f(bc, 0)) < 0) {
				ac = x;
			}
			else if ((f(ac, 0) * f(x, 0)) < 0) {
				bc = x;
			}
			if (abs(ac - bc) < eps) {
				cout << "Корень: " << x<<endl;
				break;
			}
		}
	}
	cout << "Метод Ньютона: ";
	float oldx=x;
	while (true) {
		oldx = x;
		x -= f(x,0) / fpc(x,0.0357143);
		if (abs(x-oldx)<eps) {
			cout << "Корень: " << x << endl;
			break;
		}
	}
}

graph graphs[N];
bool ShowGraph[N];
float tmp=0;
float orX=2, orY=5,trX=0,trY=0;

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
		trX -= 1*orX/2;
		glLoadIdentity();
		glOrtho(trX - orX, trX + orX, trY - orY, trY + orY, -10, 10);
	}
	if (key == GLUT_KEY_RIGHT) {
		trX += 1*orX / 2;
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
		glOrtho(trX-orX, trX+orX, trY-orY, trY+orY, -10, 10);
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
		glColor3f(1.0*i / (N-1), 1.0 - 1.0*i / (N - 1), 1);
		if (ShowGraph[i])
			graphs[i].DrawGraph();
	}
	glColor3f(1.0, 0.0, 0.0);
	DrawSystem();

	glutSwapBuffers();
}


int main(int argc, char **argv)
{
	setlocale(LC_ALL,"Russian");
	for (int i = 0; i < N; i++) {
		ShowGraph[i] = true;
	}

	graphs[0].Initialize(f, 280);
	graphs[1].Initialize(fpc, 280);
	//graphs[2].Initialize(fpr, 280);
	//graphs[3].Initialize(fpc, 280);
	//graphs[4].Initialize(fp2c, 280);
	//graphs[5].Initialize(fp3c, 280);

	cout << "График 1-функция" << endl;
	//cout << "График 2-первая производная слева" << endl;
	//cout << "График 3-первая производная справа" << endl;
	cout << "График 2-первая производная посередине" << endl;
	//cout << "График 5-вторая поизводная" << endl;
	//cout << "График 6-третья производная" << endl;

	findroot(aa,bb);

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