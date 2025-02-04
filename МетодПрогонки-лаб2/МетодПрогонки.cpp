// МетодГаусса.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#define N 6
using namespace std;
int main()
{
	cout.precision(3);
	ifstream file;
	file.open("inputdata.txt");
	float matrix[N][N + 1];
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N + 1; j++) {
			file >> matrix[i][j];
		}
	}
	float roots[N];
	float A[N - 1], B[N - 1],e;
	A[0] = -matrix[0][1] / matrix[0][0];
	B[0] = matrix[0][N ] / matrix[0][0];
	for (int i = 1; i < N-1; i++) {
		e = matrix[i ][i-1]*A[i-1] + matrix[i][i];
		A[i] = -matrix[i ][i+1] / e;
		B[i] = (matrix[i][N] - matrix[i][i-1] * B[i - 1])/e;
	}
	roots[N - 1] = (matrix[N - 1][N] - matrix[N - 1][N - 2] * B[N - 2]) / (matrix[N - 1][N - 1] + matrix[N - 1][N - 2] * A[N - 2]);

	for (int i = N - 2; i > -1; i--) {
		//roots[i] = (matrix[i][N] - matrix[i][i -1] * B[i - 1]-matrix[i][i+1]*roots[i+1]) / (matrix[i][i] + matrix[i][i - 1] * A[i - 1]);
		roots[i] = A[i] * roots[i + 1] + B[i];
	}
	for (int j = 0; j < N ; j++) {
		cout << roots[j] << " ";
	}
	cout << endl;
	system("pause");
	return 0;
}

