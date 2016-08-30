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

class Camera {
public:
	virtual void update(float deltaTime, GLFWwindow* window) = 0;
	void setPerspective(float, float, float, float);
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform();
	mat4 getView();
	mat4 getProjection();
	mat4 getProjectionView();
	double lastx = 0;
	double lasty = 0;
	void RotateWorld(mat4 rotate);
	mat4 worldTransform;
	mat4 viewTransform;
private:	
	void updateProjectionView();
	
	mat4 projectionTransform;
	mat4 projectionViewTransform;
};

class FlyCamera : public Camera {
public:
	void update(float deltaTime, GLFWwindow* window) override;
	void setSpeed(float speed);

private:
	vec3 up;
	float speed;
};