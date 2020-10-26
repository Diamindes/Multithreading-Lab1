#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <omp.h>

#include "Util.h"

using namespace std;

struct MatrixInfo {
	int rows1, cols1, rows2, cols2;
	long long int total;
};

MatrixInfo getMatrixInfo(
	vector< vector<double> >& matrix1,
	vector< vector<double> >& matrix2) {
	MatrixInfo matrixInfo;

	matrixInfo.cols1 = matrix1[0].size();
	matrixInfo.rows1 = matrix1.size();
	matrixInfo.cols2 = matrix2[0].size();
	matrixInfo.rows2 = matrix2.size();
	matrixInfo.total = matrixInfo.rows1 * matrixInfo.cols2;

	return matrixInfo;
};


struct MatrixHolder {
	MatrixInfo matrixInfo;
	vector<vector<double>>& matrix1, matrix2, result;
	long long int chunk;
};


MatrixHolder getMatrixHolder(vector< vector<double> >& matrix1, vector< vector<double> >& matrix2, vector< vector<double> >& result, long long chunk) {
	MatrixInfo matrixInfo = getMatrixInfo(matrix1, matrix2);
	MatrixHolder matrixHolder = { matrixInfo, matrix1, matrix2, result };
	matrixHolder.matrix1 = matrix1;
	matrixHolder.matrix2 = matrix2;
	matrixHolder.result = result;
	matrixHolder.chunk = chunk;

	return matrixHolder;
};

int prepareResultMatrix(vector< vector<double> >& matrix1, vector< vector<double> >& matrix2, vector< vector<double> >& result)
{
	int cols1 = matrix1[0].size();
	int rows1 = matrix1.size();
	int cols2 = matrix2[0].size();
	int rows2 = matrix2.size();

	if (cols1 != rows2) {
		return 1;
	}

	result.resize(rows1);

	for (int i = 0; i < rows1; i++) {
		result[i].resize(cols2);
	}

	for (int i = 0; i < rows1; i++) {
		for (int j = 0; j < cols2; j++) {
			result[i][j] = 0;
		}
	}

	return 0;
}

void compute(MatrixHolder& matrixHolder, int index) {
	MatrixInfo mi = matrixHolder.matrixInfo;
	int i = index % matrixHolder.matrixInfo.rows1;
	int j = index / matrixHolder.matrixInfo.rows1;

	for (int ci = 0; ci < matrixHolder.matrixInfo.cols1; ci++) {
		matrixHolder.result[i][j] += matrixHolder.matrix1[i][ci] * matrixHolder.matrix2[ci][j];
	}
}

void computeWithSingle(MatrixHolder& matrixHolder) {
	for (long long int i = 0; i < matrixHolder.matrixInfo.total; i++) {
		compute(matrixHolder, i);
	}
}

void computeWithFor(MatrixHolder& matrixHolder) {
#pragma omp parallel for
	for (long long int i = 0; i < matrixHolder.matrixInfo.total; i++) {
		compute(matrixHolder, i);
	}
}

void computeWithStatic(MatrixHolder& matrixHolder) {
#pragma omp parallel for schedule(static, matrixHolder.chunk)
	for (long long int i = 0; i < matrixHolder.matrixInfo.total; i++) {
		compute(matrixHolder, i);
	}
}

void computeWithDynamic(MatrixHolder& matrixHolder) {
#pragma omp parallel for schedule(dynamic, matrixHolder.chunk)
	for (long long int i = 0; i < matrixHolder.matrixInfo.total; i++) {
		compute(matrixHolder, i);
	}
}

void computeWithGuided(MatrixHolder& matrixHolder) {
#pragma omp parallel for schedule(guided, matrixHolder.chunk)
	for (long long int i = 0; i < matrixHolder.matrixInfo.total; i++) {
		compute(matrixHolder, i);
	}
}

int run(vector<string> args, int chunk) {
	if (args.size() < 3) {
		cout << "Arguments size mismatch" << endl;
		return 1;
	}

	char* matrixPath1 = &args[0][0];
	char* matrixPath2 = &args[1][0];
	string mode = args[2];

	vector< vector<double> > matrix1, matrix2, result;
	matrix1 = Util::loadInput(matrixPath1);
	matrix2 = Util::loadInput(matrixPath2);

	int err_code = prepareResultMatrix(matrix1, matrix2, result);

	MatrixInfo matrixInfo = getMatrixInfo(matrix1, matrix2);
	MatrixHolder matrixHolder = getMatrixHolder(matrix1, matrix2, result, 10);

	if (err_code == 1) {
		cout << "Wrong size of matrix" << endl;
		return 1;
	}
	auto started = std::chrono::high_resolution_clock::now();

	if (mode == "for")				 computeWithFor(matrixHolder);
	//else if (mode == "single")       computeWithSingle(matrixHolder);
	else if (mode == "static")       computeWithStatic(matrixHolder);
	else if (mode == "dynamic")      computeWithDynamic(matrixHolder);
	else if (mode == "guided")       computeWithGuided(matrixHolder);
	else cout << "This type does not exist";

	auto done = std::chrono::high_resolution_clock::now();
	cout << std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count() << "";
	Util::saveResult(matrixHolder.result);

	return 0;
}
