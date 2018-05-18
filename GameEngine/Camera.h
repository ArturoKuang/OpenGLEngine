#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h.>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include "RigidBody.h"
#include <string>
#include "Model.h"
class Camera
{
public:
	Camera();
	void update(const glm::vec3 &pos);
	void upload();
	void fpsControls(GLFWwindow* window, float &dt);
	virtual ~Camera();
private:
	//worldview matrix
	glm::mat4 worldView;
	//lookat matrix 
	glm::vec3 camLoc = { 8, 26, -12};
	glm::vec3 camRot = { -.86,3,0};
	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);
	glm::mat4 lookAtMat = glm::lookAt(eye, center, up);
	//velocity
	glm::vec3 velocity;

	//perspective (projection)
	float zoom = 1.f;
	int width = 800;
	int height = 600;
	float fovy = 3.14159f *.40f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.f;
	glm::mat4 perspectiveMat = glm::perspective(fovy, aspect, zNear, zFar);

};
//onject collidertype
enum colliderType
{
	colliderless,
	AABB,
	sphere
};

//keeps track of object transform data 
struct Transform
{
	glm::vec3 loc, rot, size;
	glm::mat4 objectWorldTransform;
};
//object
struct Object
{
	bool alive;
	colliderType col;
	Model model;
	RigidBody rigidBody = RigidBody();
	Transform transform;
	std::string fileName;
};

