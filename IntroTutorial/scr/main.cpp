#include "MyApp.h"

int main() {
	MyApp* app = new MyApp();
	if (app->Start() == true) {
		while (app->Update() == true)
			app->Draw();
		app->Shutdown();
	}

	delete app;
	return 0;
}

