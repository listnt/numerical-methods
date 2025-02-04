// lab10montecarlo.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include<cmath>
#include<iostream>
#include<ctime>
#define N 10000

using namespace std;

float func(float x) {
	return ((x*x + 1)*x*x) / (x*x*x*x + x * x + 1);
}

int main()
{
	srand(time(0));
	float a=1, b=21;
	float eps,*X;
	int MAGIC_C = N,temp;
	X = new float[N];
	for (int i = 0; i < N; i++) {
		temp = rand() % MAGIC_C;
		eps = 1.0*(temp+1) / (MAGIC_C+1);
		X[i] = 1.0 / eps;
	}
	float sum=0;
	for (int i = 0; i < N; i++) {
		sum += func(X[i]) / N;
	}
	cout <<"sum: "<< sum << endl;
	float disp = 0,Af=0;
	for (int i = 0; i < N; i++) {
		disp+=((func(X[i]) - sum)*(func(X[i]) - sum)) / (N - 1);
	}
	Af = sqrt(disp*disp) / N;
	cout << "disp: " << Af << endl;
	system("pause");
    return 0;
}

