#include<math.h>
#include<iostream>
using namespace std;
typedef double (*func)(double x) ;

double f1(double x) {
	return 1;
}

double f2(double x) {
	return x;
}

double f3(double x) {
	return x*x;//tan(x);
}

double determ(double** Arr, int size)
{
	int i, j;
	double det = 0;
	double** matr;
	if (size == 1)
	{
		det = Arr[0][0];
	}
	else if (size == 2)
	{
		det = Arr[0][0] * Arr[1][1] - Arr[0][1] * Arr[1][0];
	}
	else
	{
		matr = new double*[size - 1];
		for (i = 0; i<size; ++i)
		{
			for (j = 0; j<size - 1; ++j)
			{
				if (j<i)
					matr[j] = Arr[j];
				else
					matr[j] = Arr[j + 1];
			}
			det += pow((double)-1, (i + j))*determ(matr, size - 1)*Arr[i][size - 1];
		}
		delete[] matr;
	}
	return det;
}

void transpose(double **mat, int n) {
	for (int i = 0; i < 0; i++) {
		for (int j = 0; j < i; j++) {
			swap(mat[i][j], mat[j][i]);
		}
	}
}

void GaussMat(double **matrix,int n) {
	double tp;
	for (int k = 0; k < n; k++) {
		if (matrix[k][k] == 0)
			for (int z = k; z < n; z++) {
				if (matrix[z][k] != 0) {
					for (int p = 0; p < n; p++)
						swap(matrix[z][p], matrix[k][p]);
					break;
				}
			}
		for (int i = k + 1; i < n; i++) {
			tp = -matrix[i][k] / matrix[k][k];
			for (int j = k; j < n + 1; j++) {
				matrix[i][j] += tp*matrix[k][j];
			}
		}
	}
	double *roots;
	roots = new double[n];
	double sum = 0;
	roots[n - 1] = matrix[n - 1][n] / matrix[n - 1][n - 1];
	for (int i = n - 2; i >-1; i--) {
		sum = 0;
		for (int j = i + 1; j < n; j++) {
			sum += matrix[i][j] * roots[j];
		}
		roots[i] = (matrix[i][n] - sum) / matrix[i][i];
	}
	cout << "roots" << endl;
	for (int j = 0; j < n; j++) {
		cout << roots[j] << "\t";
	}
	cout << endl;
}

int main() {
	double x, y;
	double input[20] = { 0.025,
		0.184,
		0.623,
		0.476,
		0.817,
		0.933,
		0.597,
		1.113,
		0.728,
		1.245,
		0.019,
		1.245,
		0.647,
		1.881,
		0.742,
		0.048,
		0.418,
		0.734,
		-0.026,
		-0.312 };//2.64,2.87,2.78,2.56,2.74,2.65,2.33,2.54,2.57,2.39,2.17,2.29,1.95,1.89,2.10,2.70,3.26,1.94,2.35,1.92,1.94 };
	func f[3];
	f[0] = &f1; f[1] = &f2; f[2] = &f3;
	double C[3][3],weight=36,F[3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			C[i][j] = 0;
			weight = 4;
			for (int k = 0; k < 20; k++) {
				if (k > 9) {
					weight = 1;
					C[i][j] += weight*f[i](0.1*(k+1))*f[j](0.1*(k+1));
				}
				else 
					C[i][j] += weight*f[i](0.1*k)*f[j](0.1*k);
				//cout << weight <<" "<< (-1.0 + 0.1*k)<<" "<< input[k] << endl;
			}
			cout << C[i][j] << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < 3; i++) {
		F[i] = 0;
		weight = 4;
		for (int k = 0; k < 20; k++) {
			if (k > 9) {
				weight = 1;
				F[i] += weight*f[i](0.1*(k+1))*input[k];
			}
			else
				F[i] += weight*f[i](0.1*k)*input[k];
		}
		cout << F[i] << endl;
	}

	double **mat;
	mat = new double*[3];
	for (int i = 0; i < 3; i++) {
		mat[i] = new double[4];
		for (int j = 0; j < 3; j++) {
			mat[i][j] = C[i][j];
		}
		mat[i][3] = F[i];
	}
	GaussMat(mat, 3);
	system("pause");
}