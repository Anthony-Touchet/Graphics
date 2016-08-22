#include <iostream>

int main() {
	const int size = 4;
	
	float matrix[size][size];
	int oneSpot = 0;
	for (int i = 0; i < size; i++) {
		for (int ii = 0; ii < size; ii++) {
			if (i == oneSpot && ii == oneSpot) {
				matrix[i][ii] = 1;
				oneSpot++;
			}

			else {
				matrix[i][ii] = 0;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		for (int ii = 0; ii < size; ii++) {
			std::cout << matrix[i][ii] << " ";
		}

		std::cout << std::endl;
	}

	system("pause");
	return 0;
}