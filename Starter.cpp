#include <string>
#include <vector>
#include <iostream>
#include "Matrix.h"
#include "Util.h"
#include <omp.h>

using namespace std;

const int TEST_NUMBER = 1;
const int chunks[] = { 1000 };
string matrixPath1 = "generatedMatrix1.txt";
string matrixPath2 = "generatedMatrix2.txt";

void test(string);
void testWithChunks(string);

int main(int argc, char* argv[]) {

	switch (atoi(argv[2])) {

	case 1: {
		matrixPath1 = argv[3];
		matrixPath2 = argv[4];
		break;
	}

	case 2: {
		int i = atoi(argv[3]);
		int j = atoi(argv[4]);
		int k = atoi(argv[5]);
		Util::generateInput(matrixPath1, i, j);
		Util::generateInput(matrixPath2, j, k);
		break;
	}
	}

	int maxThreadNum = atoi(argv[1]);
	omp_set_num_threads(maxThreadNum);

	test("single");
	test("for");
	test("static");
	testWithChunks("dynamic");
	testWithChunks("guided");

	system("pause");
	return 0;
}

void test(string type) {
	vector<string> args;
	args.push_back(matrixPath1);
	args.push_back(matrixPath2);
	args.push_back(type);

	cout << type << ",";
	for (int i = 0; i < TEST_NUMBER; i++) {
		run(args, 1);
		cout << ",";
	}
	cout << endl;
}

void testWithChunks(string type) {
	vector<string> args;
	args.push_back(matrixPath1);
	args.push_back(matrixPath2);
	args.push_back(type);

	
	for (int j = 0; j < 4; j++) {
		cout << type << "[" << chunks[j] << "],";
		for (int i = 0; i < TEST_NUMBER; i++) {
			run(args, chunks[j]);
			cout << ",";
		}
		cout << endl;
	}
}