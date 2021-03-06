#include "MyApp.h"

int main() {
	Application* app = new ProceduralGeneration();
	if(app->Start() == true) {
		while (app->Update() == true)
			app->Draw();
		app->Shutdown();
	}

	delete app;
	return 0;
}