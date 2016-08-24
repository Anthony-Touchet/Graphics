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
	virtual void update(float deltaTime) = 0;
	void setPerspective(float, float, float, float);

private:
	void updateProjectionView();
	mat4 worldTransform;
	mat4 viewTransform;
	mat4 projectionTransform;
	mat4 projectionViewTransform;
};

class FlyCamera : public Camera {
public:
	void update(float deltaTime) override;
	void setSpeed(float speed);

private:
	vec3 up;
	float speed;
};