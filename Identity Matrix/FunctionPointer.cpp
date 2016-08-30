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

//int main() {
//	void(*hello)() = &Hello;
//	void(*world)() = &World;
//	void(*how)() = &How;
//	void(*youdo)() = &DoYouDo;
//
//	void(*store[4])() = {hello, world, how, youdo};
//
//	for (int i = 0; i < 4; i++) {
//		(*store[i])();
//	}
//
//	//Expected Result: Hello World. How do you do?
//
//	system("pause");
//	return 0;
//}