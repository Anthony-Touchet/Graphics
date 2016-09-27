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

#include <FBX\Header\FBXFile.h>

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

struct Vertex
{
	vec4 position;
	vec4 color;
};

struct VertexM {
	float x, y, z, w;
	float nx, ny, nz, nw;
	float tx, ty, tz, tw;
	float s, t;
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
	void MakeShpere(const int radius, const unsigned int verts, const unsigned int halfCircles);
	Vertex* GenHalfCircleVertexes(unsigned int p, const int &rad);
	Vertex * GenSphereVerts(const unsigned int & sides, const unsigned int &mirid, Vertex *& halfCircle);
	unsigned int* GenSphereIndicies(const unsigned int &verts, const unsigned int &mird);

	void ClearPlaneBuffers();
	void ClearCubeBuffers();
	void ClearSphereBuffers();

	void DrawPlane();
	void DrawCube();
	void DrawSphere();

	unsigned int planeVAO;
	unsigned int planeVBO;
	unsigned int planeIBO;
	unsigned int cubeVAO;
	unsigned int cubeVBO;
	unsigned int cubeIBO;
	unsigned int sphereVAO;
	unsigned int sphereVBO;
	unsigned int sphereIBO;

	unsigned int m_programID;
	GLfloat m_time;
	mat4 m_projectionViewMatrix;
	std::vector<unsigned int> indicesSTD;

private:
	GLFWwindow* window;
	FlyCamera cam;

	int planeindexCount;
	int cubeindexCount;
	int sphereindexCount;

	float previous = 0;
	float current;
	float delta;
	float PI = 3.14159265359;
	
};

class Texturing : public Application {
public:
	// Inherited via Application
	virtual bool Update() override;
	virtual bool Start() override;
	virtual void Draw() override;
	virtual void Shutdown() override;

	Texturing(); 
	std::string GetShader(std::string text);
	void MakePlane();
	void createOpenGLBuffers(FBXFile* fbx);
	void cleanupOpenGLBuffers(FBXFile* fbx);

private:
	GLFWwindow* window;
	FlyCamera cam;
	FBXFile* m_fbx;

	float previous = 0;
	float current;
	float delta;

	unsigned int m_program;
	unsigned int m_texture;
	unsigned int m_texture2;

	int planeindexCount;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
};

class AdvTexturing : public Application {
public:
	// Inherited via Application
	virtual bool Update() override;
	virtual bool Start() override;
	virtual void Draw() override;
	virtual void Shutdown() override;

	AdvTexturing();
	std::string GetShader(std::string text);
	void MakeData();

private:
	GLFWwindow* window;
	FlyCamera cam;

	float previous = 0;
	float current;
	float delta;

	unsigned int m_program;
	unsigned int m_texture, m_normal;

	int planeindexCount;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
};