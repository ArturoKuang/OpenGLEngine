#include "RigidBody.h"
#include <iostream>
#include <ctime> 
#include <cstdlib>

RigidBody::RigidBody()
{
	srand(time(0));
	r1 = .85f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - .85f)));
	r2 = .85f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - .85f)));
}


RigidBody::~RigidBody()
{
}

glm::vec3 RigidBody::calcGravForce()
{
	force += mass * cGrav;
	return force;
}

glm::vec3 RigidBody::calcVelocity(float time)
{
	//std::cout << velocity.x << ", " << velocity.y << ", " << velocity.z << std::endl;
	velocity += (force / mass) * time;
	return velocity;
}

glm::vec3 RigidBody::calcLoc(float time, glm::vec3 &pos)
{
	pos += velocity * time;
	//force = { 0,0,0 };
	//orbit
	return pos;
}

void RigidBody::drag()
{
	float cDrag = -.75f;
	force += cDrag * velocity;
}

void RigidBody::gravatationalForce(glm::vec3 & pos, glm::vec3 & rot)
{
	////direction of force
	//glm::vec3 gravForce = -pos;
	//float d = glm::length(gravForce);
	//d = glm::clamp(d, .75f, 25.0f);
	//gravForce = glm::normalize(gravForce);
	//std::cout << d << " d " << std::endl;

	////magnitude of force
	//float str = (1.5 * 2 * 1) / (d * d);

	////putting mag and dir togther
	//force += (gravForce * str);
	x = r1 * r * cos(theta);
	z = r2 * r * sin(theta);
	pos.x = x;
	pos.z = z;
	theta += dTheta;
	rot.x += .01f;

	//std::cout << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
	//std::cout << x << ", " << z << ", " << theta << std::endl;
}




void RigidBody::run(GLFWwindow * window, float dt)
{
	glm::vec3 forward = { 0,0,1 };
	glm::vec3 side = { 1,0,0 };
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		force -= forward;
	}
	else if (glfwGetKey(window, GLFW_KEY_A))
	{
		force += forward;
	}
	else if (glfwGetKey(window, GLFW_KEY_W))
	{
		force -= side;
	}
	else if (glfwGetKey(window, GLFW_KEY_S))
	{
		force += side;
	}
	else
	{
		force = { 0,0,0 };
		velocity = { 0,0,0 };
	}

}

void RigidBody::shoot()
{
	force += glm::vec3{ -10,0,0 };
}






