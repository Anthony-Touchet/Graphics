#include <iostream>

void MultiplyMatrix(float** a, float** b) {
	float result[4][4];
	float slot = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				slot = a[j][i] * b[i][k];
			}
			result[i][j] = slot;
			slot = 0;
		}
	}

	for (int z = 0; z < 4; z++) {
		for (int zz = 0; zz < 4; zz++) {
			std::cout << result[z][zz] << " ";
		}
		std::cout << std::endl;
	}
		
}

float** MakeIdentiy(int matrixSize) {

	//Make Idnetiy Matrix
	float** matrix;
	matrix = new float*[matrixSize];
	for (int i = 0; i < matrixSize; i++)
		matrix[i] = new float[matrixSize];

	int oneSpot = 0;
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			if (i == oneSpot && j == oneSpot) {
				matrix[i][j] = 1;
				oneSpot++;
			}

			else {
				matrix[i][j] = 0;
			}
		}
	}

	//Printing Matrix
	/*for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			std::cout << matrix[i][j] << " ";
		}

		std::cout << std::endl;
	}*/

	return matrix;
}


//int main() {
//	float** c = MakeIdentiy(4);
//	float** b = MakeIdentiy(4);
//
//	b[3][0] = 3;
//	b[3][1] = 2;
//	b[3][2] = 1;
//
//	MultiplyMatrix(c, b);
//
//	system("pause");
//	return 0;
//}