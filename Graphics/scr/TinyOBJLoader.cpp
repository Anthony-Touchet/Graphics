#include "MyApp.h"

TinyOBJLoader::TinyOBJLoader()
{
	glfwInit();

	window = glfwCreateWindow(1080, 720, "Window", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.35f,
		16 / 9.f, 0.1f, 1000.f);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer
}

bool TinyOBJLoader::Update()
{
	//If window is open
	if (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		return true;
	}
	return false;
}

bool TinyOBJLoader::Start()
{
	return true;
}

void TinyOBJLoader::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glfwSwapBuffers(window);
	glfwPollEvents();
}

void TinyOBJLoader::Shutdown()
{
	glfwDestroyWindow(window);			//Terminate window
	glfwTerminate();
}
