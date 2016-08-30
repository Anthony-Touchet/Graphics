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
	return matrix;
}

float** GenerateOrthoProjection(float right, float left, float top, float bottom, float near, float far) {
	float** matrix = MakeIdentiy(4);

	//Scale
	matrix[0][0] = 2 / (right - left);
	matrix[1][1] = 2 / (top - bottom);
	matrix[2][2] = 2 / (near - far);

	//Positioning
	matrix[0][3] = -((right + left) / (right - left));
	matrix[1][3] = -((top + bottom) / (top - bottom));
	matrix[2][3] = -((far + near) / (far - near));

	return matrix;
}

float** GeneratePersProjection(float fov, float n, float f, float a) {
	float** matrix = MakeIdentiy(4);

	matrix[0][0] = 1 / (a * tan(fov / 2));
	matrix[1][1] = 1 / (tan(fov / 2));
	matrix[2][2] = -((f + n) / (f - n));
	matrix[2][3] = ((2 * f *n) / (f - n));

	matrix[3][2] = -1;

	return matrix;
}

//int main() {
//	float** matrix = GeneratePersProjection(45, .1, 100, 16 / 9);
//	float** matrix = GenerateOrthoProjection(1, -1, 1, -1, 0, 1);
//	
//	for (int z = 0; z < 4; z++) {
//		for (int zz = 0; zz < 4; zz++) {
//			std::cout << matrix[z][zz] << " ";
//		}
//		std::cout << std::endl;
//	}
//
//	system("pause");
//	return 0;
//}