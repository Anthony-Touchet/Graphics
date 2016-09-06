#include <iostream>

//write a function that takes a function as a parameter.The function it receives
//should take two arguments(min, max).Calculate a random number between those two 
//numbers and return it.If the function fails return or assert on the call. if the 
//function that takes the min, max function fails give feedback to the user.

int CalculateRandom(int min, int max) {
	int result;

	result = std::rand() % max + min;

	return ;
}

int Test() {
	std::rand();
}

int Start(int(f)(int, int), int min, int max) {
	return f(min, max);
}

int main() {
	
	std::cout << Start(CalculateRandom, 1, 20) << std::endl;
	system("pause");
	return 0;
}