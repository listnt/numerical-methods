// lab11Furie.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#define _USE_MATH_DEFINES 
#include<cmath>
#include<iostream>
#include<fstream>

using namespace std;

typedef float (*afunc)(float);

float fun(float x) {
	return (int)x % 2;
}

float energy(float a,float b, afunc) {
	return 0;
}

int main()
{
	float dt, a, b, T, *ak, *bk;
	int n, m;
	cout << "enter a" << endl;
	cin >> a;
	cout << "enter b" << endl;
	cin >> b;
	cout << "enter T" << endl;
	cin >> T;
	cout << "enter n" << endl;
	cin >> n;
	cout << "enter m" << endl;
	cin >> m;
	//n = 2 * m + 1;
	dt = 1.0*(b - a) / n;
	ak = new float[m+1];
	bk = new float[m+1];
	int  i = 0, j = 0;
	float temp = 0;
	for (j = 0; j <= m; j++){
		ak[j] = 0;
		temp = 0;
		for (i = 0; i < n; i++) {
			temp += fun(a + i * dt)*cos(j * 2 * M_PI / T * (a + i * dt));
		}
		ak[j] = temp * 2.0 / n;
		bk[j] = 0;
		temp = 0;
		for (i = 0; i < n ; i++) {
			temp += fun(a + i * dt)*sin(j * 2 * M_PI / T * (a + i * dt));
		}
		bk[j] = temp * 2.0 / n;
	}

	ofstream origin("origin.txt");
	ofstream fur("fur.txt");
	for (i = 0; i < n; i++) {
		origin << a + i * dt << " " << fun(a + i * dt) << endl;
		fur << a + i * dt << " ";
		temp = 0;
		for (j = 1; j <= m; j++) {
			temp += ak[j] * cos(j * 2 * M_PI / T * (a + i * dt)) + bk[j] * sin(j * 2 * M_PI / T * (a + i * dt));
		}
		fur << ak[0] / 2 + temp << endl;
	}
	energy(a, b, fun);
	system("pause");
	return 0;
}

