#include "MyApp.h"

SolarSystem::SolarSystem()
{
}

bool SolarSystem::Start()
{
	white = vec4(1, 0, 0, 1);
	black = vec4(0, 0, 0, 1);
	yellow = vec4(0, 1, 1, 1);

	if (glfwInit() == false)
		return false;

	window = glfwCreateWindow(1280, 720, "Anthony is the Best!!", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	cam.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	//view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	
	cam.setPerspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	//projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);	glm::ortho(-17.7, 17.7, -10.0, 10.0, 0.1, 50.0);

	cam.setSpeed(10);

	glEnable(GL_DEPTH_TEST);	//Enables the Depth Buffer

	Gizmos::create();

	return true;
}

bool SolarSystem::Update()
{
	//Calculate delta time
	current = (float)glfwGetTime();
	delta = current - previous;
	previous = current;

	//If window is open
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Gizmos::clear();

		angle += .01f;
		mat4 sunrot = glm::rotate(angle, glm::vec3(0, 1, 0));	//Rotate Sun
		Sun = mat4(1) * sunrot;		//Apply Rotation

		mat4 newEarthPos = glm::translate(EarthOffsetFromSun);														//Transform Matrix, Earth to Sun
		Earth = Sun * newEarthPos * glm::rotate(angle, glm::vec3(0, 1, 0)/* Rotating is part of how it moves */);	//Sun = Origin, newEarth * glm::rotate = How are you transforming it.
		
		mat4 newMoon = glm::translate(MoonOffsetFromEarth);							//Translation Matrix of the Moon
		Moon = Earth * newMoon * glm::rotate(angle, glm::vec3(0, 1, 0));			//Apply the translation

		return true;
	}
	return false;
}

void SolarSystem::Draw()
{
	//Draw
	Gizmos::addSphere(vec3(Sun[3]), 3, 20, 20, yellow, &Sun);					//Sun
	Gizmos::addSphere(vec3(Earth[3]), 1, 20, 20, vec4(0, .5, 0, 1), &Earth);	//Earth
	Gizmos::addSphere(vec3(Moon[3]), .5, 20, 20, white, &Moon);					//Moon

	cam.update(delta, window);
	Gizmos::draw(cam.getProjectionView());

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void SolarSystem::Shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
}
