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

void Camera::RotateWorld(mat4 rotate)
{
	worldTransform = worldTransform * rotate;
}

void Camera::updateProjectionView()
{
	projectionViewTransform = projectionTransform * viewTransform * worldTransform;
}

mat4 GetRotateMatrix(float angle, vec3 up) {
	return glm::rotate(angle, up);
}

void FlyCamera::update(float deltaTime, GLFWwindow* window)
{
	vec3 moveBy = vec3(0);					//How Much to move the camera by
	mat4 rotateBy = mat4(1);					//How Much to rotate by
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	double Xoffset;
	double Yoffset;


	Xoffset = lastx - x;
	Yoffset = lasty - y;

	lastx = x;
	lasty = y;

	//Scale down effect
	Xoffset *= .005;
	Yoffset *= .005;

	int stateW = glfwGetKey(window, GLFW_KEY_W);	//State W is in
	int stateS = glfwGetKey(window, GLFW_KEY_S);	//State S is in
	int stateA = glfwGetKey(window, GLFW_KEY_A);	//State A is in
	int stateD = glfwGetKey(window, GLFW_KEY_D);	//State D is in
	int stateMouse = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);	//State Mouse is in

	if (stateMouse == GLFW_PRESS) {				//pressing the Mouse Button
		if (Yoffset > 1 || Yoffset < 1) {
			double rotate = Yoffset;
			rotateBy[1][1] = cos(rotate);
			rotateBy[1][2] = -sin(rotate);
			rotateBy[2][1] = sin(rotate);
			rotateBy[2][2] = cos(rotate);
		}

		if (Xoffset > 1 || Xoffset < 1) {
			double rotate = Xoffset;

			rotateBy[0][0] = cos(rotate);
			rotateBy[0][2] = sin(rotate);
			rotateBy[2][0] = -sin(rotate);
			rotateBy[2][2] = cos(rotate);
		}

	}

	if (stateW == GLFW_PRESS) {
		moveBy.x += speed * deltaTime;
		moveBy.z += speed * deltaTime;
	}

	if (stateS == GLFW_PRESS) {
		moveBy.x -= speed * deltaTime;
		moveBy.z -= speed * deltaTime;
	}

	if (stateA == GLFW_PRESS) {
		moveBy.x += speed * deltaTime;
		moveBy.z -= speed * deltaTime;
	}

	if (stateD == GLFW_PRESS) {
		moveBy.x -= speed * deltaTime;
		moveBy.z += speed * deltaTime;
	}
	worldTransform *= glm::translate(moveBy);

	worldTransform *= rotateBy;

	getProjectionView();
}

void FlyCamera::setSpeed(float speed)
{
	this->speed = speed;
}