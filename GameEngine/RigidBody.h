#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h.>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <math.h>
#include <algorithm>

class RigidBody
{
public:
	RigidBody();
	virtual ~RigidBody();
	float mass;
	float theta = 3.14159265f / 4;
	glm::vec3 cGrav = { 0, -.981, 0 }; //grav constant 
	float dTheta; //chane in theata
	float r; //radius
	float r1; //random 1
	float r2; //random 2
	float x;
	float z;
	glm::vec3 velocity;
	glm::vec3 force;
	glm::vec3 calcGravForce(); //calcs gravatational force
	glm::vec3 calcVelocity(float time); //calc change in velocity 
	glm::vec3 calcLoc(float time, glm::vec3 &pos); // calc the change in location 
	void drag();
	void gravatationalForce(glm::vec3 &pos, glm::vec3 & rot);
	void run(GLFWwindow* window, float dt);
	void shoot();

};


