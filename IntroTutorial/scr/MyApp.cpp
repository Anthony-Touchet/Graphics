#include "MyApp.h"

MyApp::MyApp()
{
}

bool MyApp::Start()
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

	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	glEnable(GL_DEPTH_TEST);	//Enables the Depth Buffer

	Gizmos::create();

	return true;
}

bool MyApp::Update()
{
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		current = glfwGetTime();
		delta = current - previous;
		previous = current;

		Gizmos::clear();

		angle += .01f;

		Sun = glm::rotate(Sun, (glm::mediump_float).05, glm::vec3(0, 1, 0));	//Rotate Sun

		mat4 newEarth = glm::translate(EarthOffsetFromSun);
		Earth = (Sun * newEarth);
		Earth = glm::rotate(Earth, (glm::mediump_float)20, glm::vec3(0, 1, 0));
		
		mat4 newMoon = glm::translate(MoonOffsetFromEarth);
		Moon = Earth * newMoon;

		

		return true;
	}
	return false;
}

void MyApp::Draw()
{
	//Draw
	Gizmos::addSphere(vec3(Sun[3][0], Sun[3][1], Sun[3][2]), 3, 20, 20, yellow, &Sun);						//Sun
	Gizmos::addSphere(vec3(Earth[3][0], Earth[3][1], Earth[3][2]), 1, 20, 20, vec4(0, .5, 0, 1), &Earth);	//Earth
	Gizmos::addSphere(vec3(Moon[3][0], Moon[3][1], Moon[3][2]), .5, 20, 20, white, &Moon);					//Moon

	Gizmos::draw(projection * view);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void MyApp::Shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
}
