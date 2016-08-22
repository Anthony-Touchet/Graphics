#include "MyApp.h"

int main() {
	SolarSystem* app = new SolarSystem();
	if (app->Start() == true) {
		while (app->Update() == true)
			app->Draw();
		app->Shutdown();
	}

	delete app;
	return 0;
}

