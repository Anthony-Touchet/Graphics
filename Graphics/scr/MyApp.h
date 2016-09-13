#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#define GLM_SWIZZLE
#define GLM_FORCE_PURE

#include <stdio.h>
#include <stdlib.h>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include <vector>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class Application{
public:
	virtual bool Update() = 0;
	virtual bool Start() = 0;
	virtual void Draw() = 0;
	virtual void Shutdown() = 0;
};

class SolarSystem : public Application {

public:
	SolarSystem();
	bool Start() override;
	bool Update() override;
	void Draw() override;
	void Shutdown() override;

private:
	GLFWwindow* window;
	FlyCamera cam;

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

struct  Vertex
	{
		vec4 position;
		vec4 color;
	};

class RenderingGeometry : public Application {

public:
	

	RenderingGeometry();
	bool Start() override;
	bool Update() override;
	void Draw() override;
	void Shutdown() override;
	void MakePlane();
	std::string GetShader(std::string text);
	void MakeCube();
	void MakeDisc();
	void MakeShpere();
	Vertex* GenVertexes(unsigned int p, const int &rad);
	Vertex * GenSphere(const unsigned int & sides, const unsigned int &mirid, Vertex *& halfCircle);

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
	GLfloat m_time;
	mat4 m_projectionViewMatrix;

private:
	GLFWwindow* window;
	FlyCamera cam;

	int indexCount;

	float previous = 0;
	float current;
	float delta;
	float PI = 3.14159265359;
	
};