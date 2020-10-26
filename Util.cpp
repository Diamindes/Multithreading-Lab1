#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <cstdlib>
#include <omp.h>
	
#include "Util.h"


using namespace std;


Util::Util()
{
}


Util::~Util()
{
}

void Util::readMatrix(vector< vector<double> >& x, char* fileName) {
	ifstream in(fileName);
	if (in.is_open()) {
		int count = 0;
		int temp;

		while (!in.eof()) {
			in >> temp;
			count++;
		}

		in.seekg(0, ios::beg);
		in.clear();

		int count_space = 0;
		char symbol;
		while (!in.eof()) {
			in.get(symbol);
			if (symbol == ' ') {
				count_space++;
			}
			if (symbol == '\n') {
				break;
			}
		}
		in.seekg(0, ios::beg);
		in.clear();

		int n = count / (count_space + 1);
		int m = count_space + 1;
		x.resize(n);
		for (int i = 0; i < n; i++) {
			x[i].resize(m);
		}

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				in >> x[i][j];
			}
		}

		in.close();
	}
	else {
		cout << "File is not found";
	}
}

void Util::printMatrix(vector<vector<double>> matrix) {
	int col = matrix.size();
	int row = matrix[0].size();

	cout << "\n";
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++)
			cout << matrix[i][j] << " ";
		cout << "\n";
	}
}

void Util::generateInput(const string& filename, int64_t rows, int64_t columns) {
	std::ofstream fout(filename, ios_base::out | ios_base::trunc);
	fout << rows << " " << columns << endl;
	for (int64_t i = 0; i < rows; i++) {
		for (int64_t j = 0; j < columns; j++) {
			fout << (double)(rand()) / rand() << " ";
		}
		fout << endl;
	}
	fout.close();
}

vector<vector<double>> Util::loadInput(const string& filename) {
	vector<vector<double>> matrix;
	int64_t rows, columns;
	ifstream file(filename);

	file >> rows >> columns;

	if (rows < 1 || columns < 1) {
		cerr << "The number of rows and columns must be greater than 0.\n";
		return matrix;
	}
	matrix.resize(rows);
	for (auto& row : matrix) {
		row.resize(columns);
	}

	for (auto& row : matrix) {
		for (auto& num : row) {
			file >> num;
		}
	}
	file.close();
	return matrix;
}

void Util::saveResult(vector<vector<double>> result) {
	int64_t rows = result.size();
	int64_t columns = result[0].size();

	ofstream fout("result.txt", ios_base::trunc);
	fout << rows << " " << columns << endl;

	for (int64_t i = 0; i < rows; i++) {
		for (int64_t j = 0; j < columns; j++) {
			fout << result[i][j] << " ";
		}
		fout << std::endl;
	}
	fout.close();
}