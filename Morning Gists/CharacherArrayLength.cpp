#include <iostream>
#include <assert.h>

void IsFunctionGood(int(*f)(char*), char* ex) {
	//assert(f(ex) == 5);
	std::cout << f(ex);
}

int FindLengthOfArray(char* a) {
	
	char* p = &a[0];				//p is a character pointer set to the beginning of the array
	int result = 0;					//What will be returned
	for (; *p != NULL; p++) {		//As long 
		result++;
	}
	return result;
}

int main() {

	char ex[] = { "Hello" };

	IsFunctionGood(FindLengthOfArray, ex);

	system("pause");
	return 0;
}