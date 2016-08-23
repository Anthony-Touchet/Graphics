#include <iostream>

void Translate(float vec[], int matrixSize) {
	
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

	//Put transform on

	float* lastrow = new float[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		float spot = 0;
		for (int j = 0; j < matrixSize; j++)
		{
			spot += matrix[i][j] * vec[j];
		}
		lastrow[i] = spot;
	}

	matrix[3] = lastrow;
	
	//Print
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			std::cout << matrix[j][i] << " ";
		}

		std::cout << std::endl;
	}

	delete matrix;
}

int main() {
	int matrixSize;

	std::cout << "Give size of matrix:" << std::endl;
	std::cin >> matrixSize;

	float* trans = new float[matrixSize];

	std::cout << "Enter transformation. Identity will be moved by this much: " << std::endl;
	for (int i = 0; i < matrixSize; i++)
		std::cin >> trans[i];

	std::cout << std::endl;

	Translate(trans, matrixSize);

	std::cout << std::endl;

	system("pause");
	return 0;
}