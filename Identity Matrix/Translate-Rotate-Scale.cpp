#include <iostream>
#include <assert.h>

float** Translate(float** original, float** transBy) {
	float** result;
	result = new float*[4];
	for (int i = 0; i < 4; i++)
		result[i] = new float[4];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result[i][j] += original[i][k] * transBy[k][j];
			}
		}
	}

	return result;
}

int Rotate(float angle, int axis[3],float** original) {
	//First get the axis
	int axisNumber;		//The X = 0, Y = 1, Z = 2
	for (int i = 0; i < 3; i++) {
		if (axis[i] == 1) {
			axisNumber = i;
		}

		else if (i == 2 && axisNumber == NULL) {
			return NULL;
		}
	}

	//cos(angle)	sin(angle)
	//-sin(angle)	cos(angle)

	int op = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i != axisNumber && j != axisNumber) {
				switch (op) {
				case 0:
					original[i][j] = cos(angle);
					op++;
					break;

				case 1:
					original[i][j] = sin(angle);
					op++;
					break;

				case 2:
					original[i][j] = -sin(angle);
					op++;
					break;

				case 3:
					original[i][j] = cos(angle);
					op++;
					break;
				}
			}
		}
	}

	return 0;
}

void Scale() {
	//From what i read you move the the matrix back to origin, scale it, then move it back to the original position. this could just be like translation of the matrix but the diagnal of it is increased or decreaced.
}

float** MakeNewIDMatrix() {
	float** matrix;
	matrix = new float*[4];
	for (int i = 0; i < 4; i++)
		matrix[i] = new float[4];

	int oneSpot = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
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

void PrintMatrix(float** matrix) {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%i,", matrix[i][j]);
		}
		std::cout << std::endl;
	}
}

int main() {

	float** a, **b;
	float** result;
	
	a = MakeNewIDMatrix();
	b = MakeNewIDMatrix();

	/*a[0][3] = 4;
	a[1][3] = 3;
	a[2][3] = 2;

	b[0][3] = 1;
	b[1][3] = 1;
	b[2][3] = 1;*/

	/*result[0][3] = 5;
	result[1][3] = 4;
	result[2][3] = 3;*/

	//result = Translate(a,b);

	system("pause");
	return 0;
}