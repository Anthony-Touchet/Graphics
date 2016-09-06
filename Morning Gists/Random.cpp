#include <iostream>
#include <assert.h>
#include <time.h>

//write a function that takes a function as a parameter.The function it receives
//should take two arguments(min, max).Calculate a random number between those two 
//numbers and return it.If the function fails return or assert on the call. if the 
//function that takes the min, max function fails give feedback to the user.

int CalculateRandom(const int &min, const int &max) {
	int result;
	int random = std::rand();
	result = (random % max) + min;
	//assert(result >= min && result <= max);
	return result;
}

int Start(int(f)(const int&, const int&), const int &min, const int &max) {
	if (f(min, max) > max || f(min, max) < min) {
		printf("The function failed");
		return 0;
	}
	return f(min, max);
}

int main() {

	srand(time(NULL));

	std::cout << Start(CalculateRandom, 1, 10) << std::endl;//Works
	std::cout << Start(CalculateRandom, 8, 1) << std::endl;	//fails

	system("pause");
	return 0;
}