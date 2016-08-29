#include "Camera.h"

void Camera::setPerspective(float fieldOfView, float aspectRatio, float n, float f) {
	projectionTransform = glm::perspective(fieldOfView, aspectRatio, n, f);
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
	viewTransform = glm::lookAt(from, to, up);
}

void Camera::setPosition(vec3 position)
{
	worldTransform = worldTransform * glm::translate(position);
}

mat4 Camera::getWorldTransform()
{
	return worldTransform;
}

mat4 Camera::getView()
{
	return viewTransform;
}

mat4 Camera::getProjection()
{
	return projectionTransform;
}

mat4 Camera::getProjectionView()
{
	updateProjectionView();
	return projectionViewTransform;
}

void Camera::updateProjectionView()
{
	projectionViewTransform = projectionTransform * viewTransform * worldTransform;
}

void FlyCamera::update(float deltaTime, GLFWwindow* window)
{
	vec3 moveBy = vec3(0,0,0);					//How Much to move the camera by
	int stateW = glfwGetKey(window, GLFW_KEY_W);
	int stateS = glfwGetKey(window, GLFW_KEY_S);
	int stateA = glfwGetKey(window, GLFW_KEY_A);
	int stateD = glfwGetKey(window, GLFW_KEY_D);
	int stateMouse = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (stateMouse == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		
	}
	
	if (stateW == GLFW_PRESS) {
		moveBy.x += speed * deltaTime;
	}

	if (stateS == GLFW_PRESS) {
		moveBy.x -= speed * deltaTime;
	}

	if (stateA == GLFW_PRESS) {
		moveBy.z -= speed * deltaTime;
	}

	if (stateD == GLFW_PRESS) {
		moveBy.z += speed * deltaTime;
	}
	setPosition(moveBy);

	getView() = glm::inverse(getWorldTransform());

	getProjectionView();
}

void FlyCamera::setSpeed(float speed)
{
	this->speed = speed;
}
