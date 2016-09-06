#include <iostream>

void Hello() {
	std::cout << "Hello ";
}

void World() {
	std::cout << "World. ";
}

void How() {
	std::cout << "How ";
}

void DoYouDo() {
	std::cout << "do you do?" << std::endl;
}

void StoreFunction(void(*f)(), void(*arr[4])()) {
	for (int i = 0; i < 4; i++) {
		if (arr[i] == 0) {
			arr[i] = f;
			break;
		}
	}
}

//int main() {
//	void(*store[4])() = {0, 0, 0, 0};
//	
//	StoreFunction(Hello, store);
//	StoreFunction(World, store);
//	StoreFunction(How, store);
//	StoreFunction(DoYouDo, store);
//
//	for (int i = 0; i < 4; i++) {
//		(*store[i])();
//	}
//
//	//Expected Result: Hello World. How do you do?
//
//	//Actual Result: Hello WOrld. How do you do?
//	system("pause");
//	return 0;
//}