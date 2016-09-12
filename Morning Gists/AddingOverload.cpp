#include <iostream>
#include <assert.h>

void FunctionCalling(int(*f)(int, int), const int a, const int b) {
	std::cout << f(a, b) << std::endl;
}

int Add(const int a, const int b) {		//Can this be overloaded?
	return a + b;
}

int Add(const int a, const int b, const int c) {
	return a + b + c;
}

int main() {

	FunctionCalling(Add, 3, 4);
	FunctionCalling(Add, 3, 4, 7);	//Throws error

	Add(2,3,4);	//Works still. Is Overloaded

	system("pause");
	return 0;
}

//FunctionCalling looks for a very spisific signiture. FunctionCalling is looking for a function that returns an int and has two int parameters.
//Even if Add had multiple Definitions, Fuction calling is looking for one very spesific signiture.