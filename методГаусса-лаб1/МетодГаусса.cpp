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
	float tp;
	for (int k = 0; k < N; k++) {
		if(matrix[k][k]==0)
			for (int z = k; z < N; z++) {
				if (matrix[z][k] != 0) {
					for (int p = 0; p < N; p++)
						swap(matrix[z][p], matrix[k][p]);
					break;
				}
			}
		for (int i = k + 1; i < N; i++) {
			tp = -matrix[i][k] / matrix[k][k];
			for (int j = k; j < N + 1; j++) {
				matrix[i][j] += tp*matrix[k][j];
			}
		}
	}
	float roots[N];
	float sum=0;
	roots[N - 1] = matrix[N - 1][N] / matrix[N-1][N-1];
	for (int i = N-2; i >-1; i--) {
		sum = 0;
		for (int j = i + 1; j < N; j++) {
			sum += matrix[i][j] * roots[j];
		}
		roots[i] =   (matrix[i][N]-sum)/ matrix[i][i];
	}
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N + 1; j++) {
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
	for (int j = 0; j < N; j++) {
		cout << roots[j] << "\t";
	}
	system("pause");
    return 0;
}

