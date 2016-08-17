#include "Application.h"

void MultiplyMatrix(float a[4][4], float b[4][4]) {
	float result[4][4];
	float slot = 0;

	for (int i = 0; i < 4; i++) {
		for (int ii = 0; ii < 4; ii++) {
			for (int j = 0; j < 4; j++) {
				slot = a[i][j] * b[j][ii];
			}
			result[i][ii] = slot;
			slot = 0;
		}
	}

	for (int z = 0; z < 4; z++) {
		for (int zz = 0; zz < 4; zz++) {
			std::cout << result[z][zz] << " ";
		}
		std::cout << std::endl;
	}
		
	
}

int main() {

	if (glfwInit() == false)
		return -1;

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Anthony is the Best!!", nullptr, nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	Gizmos::create();

	mat4 view = glm::lookAt(vec3(10,10,10), vec3(0), vec3(0,1,0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);	//Enables the Depth Buffer

	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 yellow(0, 1, 1, 1);

	mat4 Sun = mat4(1);		//Sun Matrix
	mat4 Earth = mat4(1);	//Earth Matrix
	mat4 Moon = mat4(1);	//Moon Matrix

	vec4 EarthOffsetFromSun = vec4(6, 0, 0, 0);			//Earth relative to Sun
	
	vec4 MoonOffsetFromEarth = vec4(2, 0, 0, 0);	//Moon Relative to Earth

	float previous = 0;
	float current;
	float delta;

	float angle = 0;

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		current = glfwGetTime();
		delta = current - previous;
		previous = current;

		Gizmos::clear();

		angle += .01f;

		//Rotate planets
		Sun = glm::rotate(Sun, (glm::mediump_float)180, glm::vec3(0, 1, 0));
		Earth = glm::rotate(Earth, (glm::mediump_float)180, glm::vec3(0, 1, 0));
		Moon = glm::rotate(Moon, (glm::mediump_float)180, glm::vec3(0, 1, 0));

		//Move Sun
		if (glfwGetKey(window, GLFW_KEY_W))
			Sun[3] -= vec4(1, 0, 0, 0) * delta;

		else if(glfwGetKey(window, GLFW_KEY_S))
			Sun[3] += vec4(1, 0, 0, 0) * delta;

		//Update Earth and moon/ This is where they will have to orbit.
		Earth[3] = vec4(cos(angle) * EarthOffsetFromSun[0], 0, sin(angle) * EarthOffsetFromSun[0], 1);

		mat4 newMoon = mat4(1);
		newMoon[3] = vec4(cos(.001f) * MoonOffsetFromEarth[0], 0, sin(.001f) * MoonOffsetFromEarth[0], 1);

		Moon = Earth * newMoon;

		//Draw
		Gizmos::addSphere(vec3(Sun[3][0], Sun[3][1], Sun[3][2]), 3, 20, 20, yellow, &Sun);						//Sun
		Gizmos::addSphere(vec3(Earth[3][0], Earth[3][1], Earth[3][2]), 1, 20, 20, vec4(0, 1, 0, 1), &Earth);	//Earth
		Gizmos::addSphere(vec3(Moon[3][0], Moon[3][1], Moon[3][2]), .5, 20, 20, white, &Moon);					//Moon

		Gizmos::draw(projection * view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

