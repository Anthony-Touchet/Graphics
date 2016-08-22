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


class MyApp {

public:
	MyApp();
	bool Start();
	bool Update();
	void Draw();
	void Shutdown();

private:
	GLFWwindow* window;
	mat4 view;
	mat4 projection;

	vec4 white;
	vec4 black;
	vec4 yellow;

	mat4 Sun = mat4(1);		//Sun Matrix
	mat4 Earth = mat4(1);	//Earth Matrix
	mat4 Moon = mat4(1);	//Moon Matrix

	vec3 EarthOffsetFromSun = vec3(6, 0, 0);			//Earth relative to Sun

	vec3 MoonOffsetFromEarth = vec3(2, 0, 0);			//Moon Relative to Earth

	float previous = 0;
	float current;
	float delta;

	float angle = 0;
};

//void MultiplyMatrix(float a[4][4], float b[4][4]) {
//	float result[4][4];
//	float slot = 0;
//
//	for (int i = 0; i < 4; i++) {
//		for (int ii = 0; ii < 4; ii++) {
//			for (int j = 0; j < 4; j++) {
//				slot = a[i][j] * b[j][ii];
//			}
//			result[i][ii] = slot;
//			slot = 0;
//		}
//	}
//
//	for (int z = 0; z < 4; z++) {
//		for (int zz = 0; zz < 4; zz++) {
//			std::cout << result[z][zz] << " ";
//		}
//		std::cout << std::endl;
//	}
//		
	
//}