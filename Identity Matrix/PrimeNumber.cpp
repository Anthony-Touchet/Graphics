#include <iostream>

//Googled what a prime number is. Haven't worked with primes in a long time.
//Prime Number: A number that only has itself and 1 as factors. Can't be a multiple. 

bool IsPrime(int num) {
	for (int i = 2; i < 100; i++) {
		if (num == 1) {
			return false;
		}

		if (num % i == 0 && num != i) {
			return false;
		}
	}
	return true;
}

//Find Multipules
bool IsMultiple(int num) {
	for (int i = 2; i < num; i++) {
		if (num % i == 0) {
			return true;
		}
	}
	return false;
}

//Adding to array
int* AddToArray(int* arr, int size, int value) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == 0) {
			arr[i] = value;
			return arr;
		}
	}
}

//finding all primes
void FindAllPrimes(int max) {
	//Make array. 0 will repersent empty slots
	int* arr = new int[max];
	for  (int i = 0; i < max; i++)
	{
		arr[i] = 0;
	}

	//find all primes and put them in array
	for (int i = 1; i < max; i++) {
		if (IsPrime(i) && !IsMultiple(i)) {
			AddToArray(arr, max, i);
		}
	}

	//Print Result
	for (int i = 0; i < max; i++) {
		if (arr[i] != 0) {
			std::cout << arr[i] << std::endl;
		}
	}
}

int main() {

	std::cout << IsMultiple(14) << std::endl;	//Expecting true, Actual is true
	std::cout << IsMultiple(13) << std::endl;	//Expecting false, Actual is false

	std::cout << std::endl;

	FindAllPrimes(1000);

	system("pause");
	return 0;
}