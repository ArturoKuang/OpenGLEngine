#include "Camera.h"


Camera::Camera()
{
	worldView = glm::mat4();
}

void Camera::upload()
{
	glUniformMatrix4fv(4, 1, GL_FALSE, &worldView[0][0]);
}

void Camera::update(const glm::vec3 &pos)
{
	//perspective 
	rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	eye = camLoc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);
	lookAtMat = glm::lookAt(eye, center, up);

	worldView = perspectiveMat * lookAtMat;
	glUniformMatrix4fv(4, 1, GL_FALSE, &worldView[0][0]);
	glUniform3fv(5,1,&camLoc.x);
}

void Camera::fpsControls(GLFWwindow* window, float &dt)
{
	//turn with mouse pos
	float sens = .0005f;
	int w = 800, h = 600;
	double x, y;
	glfwGetCursorPos(window, &x, &y);	
	glfwGetWindowSize(window, &width, &height);

	//camRot.x = shipPos.x;
	camRot.y -= sens * (x - width *.5f);
	camRot.x -= sens * (y - height *.5f);
	camRot.x = glm::clamp(camRot.x, -.5f * 3.1415f, .5f * 3.1415f);
	glfwSetCursorPos(window, w*.5f, h*.5f);

	//move with arrows
	glm::vec3 camDir;

	if (glfwGetKey(window, GLFW_KEY_LEFT))
		camDir.x -= 1;
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		camDir.x += 1;
	if (glfwGetKey(window, GLFW_KEY_UP))
		camDir.z -= 1;
	if (glfwGetKey(window, GLFW_KEY_DOWN))
		camDir.z += 1;
	float speed = 1.5f;
	if (camDir != glm::vec3())
		camDir = glm::normalize(camDir);
	//sets velocity
	velocity = rotMat * camDir * speed;
	//updates the location and eye
	camLoc += velocity * dt;
	eye = camLoc;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}


Camera::~Camera()
{
}
