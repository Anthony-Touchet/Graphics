#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <iostream>
#define GLM_SWIZZLE
#define GLM_FORCE_PURE

#include <stdio.h>
#include <stdlib.h>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

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

	Earth[3] = Sun[3] + vec4(6, 0, 0, 0);	//Earth relative to Sun
	Moon[3] = Earth[3] + vec4(2, 0, 0, 0);	//Moon Relative to Earth

	float previous = 0;
	float current;
	float delta;

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		current = glfwGetTime();
		delta = current - previous;
		previous = current;

		
		Gizmos::clear();

		//Rotate planets
		Sun = glm::rotate(Sun, (glm::mediump_float)180, glm::vec3(0, 1, 0));
		Earth = glm::rotate(Earth, (glm::mediump_float)180, glm::vec3(0, 1, 0));
		Moon = glm::rotate(Moon, (glm::mediump_float)180, glm::vec3(0, 1, 0));

		//Move Sun
		if (glfwGetKey(window, GLFW_KEY_W))
			Sun[3] -= vec4(1, 0, 0, 0) * delta;

		else if(glfwGetKey(window, GLFW_KEY_S))
			Sun[3] += vec4(1, 0, 0, 0) * delta;

		//Update Earth and moon
		Earth[3] = Sun[3] + vec4(6, 0, 0, 0);


		Moon[3] = Earth[3] + vec4(2, 0, 0, 0);

		//Draw
		Gizmos::addSphere(vec3(Sun[3][0], Sun[3][1], Sun[3][2]), 3, 20, 20, yellow, &Sun);				//Sun
		Gizmos::addSphere(vec3(Earth[3][0], Earth[3][1], Earth[3][2]), 1, 20, 20, vec4(0, 1, 0, 1), &Earth);	//Earth
		Gizmos::addSphere(vec3(Moon[3][0], Moon[3][1], Moon[3][2]), .5, 20, 20, white, &Moon);				//Moon

		Gizmos::draw(projection * view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}