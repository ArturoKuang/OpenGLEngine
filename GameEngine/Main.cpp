#include <GL/glew.h>
#include <GLFW/glfw3.h.>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "RigidBody.h"
#include <map>



std::map<int, bool> keyIsDown;
std::map<int, bool> keyWasDown;

void keyCallback(GLFWwindow* windpwPtr, int key, int scancode, int action, int mods)
{
	keyIsDown[key] = action;
}

//checks collision b/w two objects 
bool collides(const Object &a, const Object &b)
{
	if (a.col == colliderless && b.col == colliderless)
	{
		return false;
	}
	//checks sphere to sphere collision 
	else if (a.col == sphere && b.col == sphere)
	{
		glm::vec3 distanceVec = a.transform.loc - b.transform.loc;
		float combinedRadi = a.transform.size.x + b.transform.size.x;
		float distance = (distanceVec.x * distanceVec.x) + (distanceVec.y * distanceVec.y) + (distanceVec.z * distanceVec.z);
		distance = glm::sqrt(distance);
		if (distance > combinedRadi)
			return false;
	}
	//checks aabb to aabb collision 
	else if (a.col == AABB && b.col == AABB)
	{
		float xDist = abs(a.transform.loc.x - b.transform.loc.x);
		float yDist = abs(a.transform.loc.y - b.transform.loc.y);
		float zDist = abs(a.transform.loc.z - b.transform.loc.z);
		//checks x,y,z direction for AABB collision 
		if (xDist > (a.transform.size.x + b.transform.size.x) || yDist > (a.transform.size.y + b.transform.size.y) || zDist > (a.transform.size.z + b.transform.size.z))
			return false;
	}
	//check aabb to sphere collsion
	else if(a.col == sphere && b.col == AABB)
	{
		// find min and max values for AABB
		float maxX = b.transform.loc.x + (b.transform.size.x / 2);
		float maxY = b.transform.loc.y + (b.transform.size.y / 2);
		float maxZ = b.transform.loc.z + (b.transform.size.z / 2);
		float minX = b.transform.loc.x - (b.transform.size.x / 2);
		float minY = b.transform.loc.y - (b.transform.size.y / 2);
		float minZ = b.transform.loc.z - (b.transform.size.z / 2);	 
		// calculate max value between AABB and sphere
		float x = fmax(minX, fmin(a.transform.loc.x, maxX));
		float y = fmax(minY, fmin(a.transform.loc.y, maxY));
		float z = fmax(minZ, fmin(a.transform.loc.z, maxZ));
		// calculate distance
		float dist = pow((x - a.transform.loc.x), 2) + pow((y - a.transform.loc.y), 2) + pow((z - a.transform.loc.z), 2);
		//radius^2
		float radiuSq = a.transform.size.x * a.transform.size.x;

		if (dist > radiuSq)
			return false;
	}
	//check sphere to aabb collsion
	else if (b.col == sphere && a.col == AABB)
	{
		// find min and max values for AABB
		float maxX = a.transform.loc.x + (a.transform.size.x / 2);
		float maxY = a.transform.loc.y + (a.transform.size.y / 2);
		float maxZ = a.transform.loc.z + (a.transform.size.z / 2);
		float minX = a.transform.loc.x - (a.transform.size.x / 2);
		float minY = a.transform.loc.y - (a.transform.size.y / 2);
		float minZ = a.transform.loc.z - (a.transform.size.z / 2);
		// calculate max value between AABB and sphere
		float x = fmax(minX, fmin(b.transform.loc.x, maxX));
		float y = fmax(minY, fmin(b.transform.loc.y, maxY));
		float z = fmax(minZ, fmin(b.transform.loc.z, maxZ));
		// calculate distance
		float dist = pow((x - b.transform.loc.x), 2) + pow((y - b.transform.loc.y), 2) + pow((z - b.transform.loc.z), 2);
		//radius^2
		float radiuSq = b.transform.size.x * b.transform.size.x;

		if (dist > radiuSq)
			return false;
	}
	return true;
}

int main()
{
	//textures 
	GLuint* texIDs = new GLuint[10];
	Texture texture;
	//objects in scene 
	std::vector<Object> objectList;
	Object sun;
	sun.fileName = texIDs[0];
	sun.transform.loc = glm::vec3(0, 0, 0);
	sun.transform.rot = glm::vec3(0, 0, 0);
	sun.transform.size = glm::vec3(1, 1, 1);
	sun.rigidBody.force = { 0,0,0 };
	sun.rigidBody.mass = 3.0f;
	sun.rigidBody.velocity = { 0,0,0 };
	sun.col = sphere;
	sun.alive = true;

	Object mercury;
	mercury.fileName = texIDs[1];
	mercury.transform.loc = glm::vec3(0, 0, 0);
	mercury.transform.rot = glm::vec3(0, 0, 0);
	mercury.transform.size = glm::vec3(1, 1, 1);
	mercury.rigidBody.force = { 0,0,0 };
	mercury.rigidBody.mass = .5f;
	mercury.rigidBody.velocity = { 0,0,0 };
	mercury.rigidBody.r = 3.0f;
	mercury.rigidBody.dTheta = .01f;
	mercury.col = sphere;
	mercury.alive = true;

	Object venus;
	venus.fileName = texIDs[2];
	venus.transform.loc = glm::vec3(0, 0, 0);
	venus.transform.rot = glm::vec3(0, 0, 0);
	venus.transform.size = glm::vec3(1, 1, 1);
	venus.rigidBody.force = { 0,0,0 };
	venus.rigidBody.mass = .5f;
	venus.rigidBody.velocity = { 0,0,0 };
	venus.rigidBody.r = 7.0f;
	venus.rigidBody.dTheta = .005f;
	venus.col = sphere;
	venus.alive = true;

	Object earth;
	earth.fileName = texIDs[3];
	earth.transform.loc = glm::vec3(0, 0, 0);
	earth.transform.rot = glm::vec3(0, 0, 0);
	earth.transform.size = glm::vec3(1, 1, 1);
	earth.rigidBody.force = { 0,0,0 };
	earth.rigidBody.mass = .5f;
	earth.rigidBody.velocity = { 0,0,0 };
	earth.rigidBody.r = 11.0f;
	earth.rigidBody.dTheta = .003f;
	earth.col = sphere;
	earth.alive = true;

	Object mars;
	mars.fileName = texIDs[4];
	mars.transform.loc = glm::vec3(0, 0, 0);
	mars.transform.rot = glm::vec3(0, 0, 0);
	mars.transform.size = glm::vec3(1, 1, 1);
	mars.rigidBody.force = { 0,0,0 };
	mars.rigidBody.mass = .5f;
	mars.rigidBody.velocity = { 0,0,0 };
	mars.rigidBody.r = 15.0f;
	mars.rigidBody.dTheta = .002f;
	mars.col = sphere;
	mars.alive = true;

	Object jupiter;
	jupiter.fileName = texIDs[5];
	jupiter.transform.loc = glm::vec3(0, 0, 0);
	jupiter.transform.rot = glm::vec3(0, 0, 0);
	jupiter.transform.size = glm::vec3(1, 1, 1);
	jupiter.rigidBody.force = { 0,0,0 };
	jupiter.rigidBody.mass = .5f;
	jupiter.rigidBody.velocity = { 0,0,0 };
	jupiter.rigidBody.r = 19.0f;
	jupiter.rigidBody.dTheta = .001f;
	jupiter.col = sphere;
	jupiter.alive = true;

	Object saturn;
	saturn.fileName = texIDs[6];
	saturn.transform.loc = glm::vec3(0, 0, 0);
	saturn.transform.rot = glm::vec3(0, 0, 0);
	saturn.transform.size = glm::vec3(1, 1, 1);
	saturn.rigidBody.force = { 0,0,0 };
	saturn.rigidBody.mass = .5f;
	saturn.rigidBody.velocity = { 0,0,0 };
	saturn.rigidBody.r = 23.0f;
	saturn.rigidBody.dTheta = .0009f;
	saturn.col = sphere;
	saturn.alive = true;

	Object uranus;
	uranus.fileName = texIDs[7];
	uranus.transform.loc = glm::vec3(0, 0, 0);
	uranus.transform.rot = glm::vec3(0, 0, 0);
	uranus.transform.size = glm::vec3(1, 1, 1);
	uranus.rigidBody.force = { 0,0,0 };
	uranus.rigidBody.mass = .5f;
	uranus.rigidBody.velocity = { 0,0,0 };
	uranus.rigidBody.r = 27.0f;
	uranus.rigidBody.dTheta = .0007f;
	uranus.col = sphere;
	uranus.alive = true;

	Object neptune;
	neptune.fileName = texIDs[8];
	neptune.transform.loc = glm::vec3(0, 0, 0);
	neptune.transform.rot = glm::vec3(0, 0, 0);
	neptune.transform.size = glm::vec3(1, 1, 1);
	neptune.rigidBody.force = { 0,0,0 };
	neptune.rigidBody.mass = .5f;
	neptune.rigidBody.velocity = { 0,0,0 };
	neptune.rigidBody.r = 32.0f;
	neptune.rigidBody.dTheta = .0005f;
	neptune.col = sphere;
	neptune.alive = true;

	Object spaceship;
	spaceship.fileName = texIDs[9];
	spaceship.transform.loc = glm::vec3(10, 0, 0);
	spaceship.transform.rot = glm::vec3(0, 0, 0);
	spaceship.transform.size = glm::vec3(.1, .1, .1);
	spaceship.rigidBody.force = { 0,0,0 };
	spaceship.rigidBody.mass = .5f;
	spaceship.rigidBody.velocity = { 0,0,0 };
	spaceship.col = AABB;
	spaceship.alive = true;

	Object bullet;
	bullet.fileName = texIDs[10];
	bullet.transform.loc = glm::vec3(100, 0, 0);
	bullet.transform.rot = glm::vec3(0, 0, 0);
	bullet.transform.size = glm::vec3(.5, .5, .5);
	bullet.rigidBody.force = { 0,0,0 };
	bullet.rigidBody.mass = .5f;
	bullet.rigidBody.velocity = { 0,0,0 };
	bullet.col = sphere;
	bullet.alive = true;

	//time
	float t0 = 0; //prev time
	float t = 0; //current time
	float dt = 0; //delta time

	if (glfwInit() == GL_FALSE)
		return -1;
	GLFWwindow * GLFWwindowPTR = glfwCreateWindow(800, 600, "ArturoKuang DSA1 Engine", NULL, NULL);
	if (GLFWwindowPTR != nullptr)
	{
		glfwMakeContextCurrent(GLFWwindowPTR);
	}
	else
	{
		glfwTerminate();
		return -1;
	}
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	//shader
	Shader color = Shader();
	//camera
	Camera camera = Camera();
	////model loading
	/*Model model = Model();
	model.buffer("models/sphere.obj");*/
	sun.model.buffer("models/sphere.obj");
	mercury.model.buffer("models/sphere.obj");
	venus.model.buffer("models/sphere.obj");
	earth.model.buffer("models/sphere.obj");
	mars.model.buffer("models/sphere.obj");
	saturn.model.buffer("models/sphere.obj");
	jupiter.model.buffer("models/sphere.obj");
	uranus.model.buffer("models/sphere.obj");
	neptune.model.buffer("models/sphere.obj");
	spaceship.model.buffer("models/spaceship.obj");
	bullet.model.buffer("models/sphere.obj");
	objectList.push_back(sun);
	objectList.push_back(mercury);
	objectList.push_back(venus);
	objectList.push_back(earth);
	objectList.push_back(mars);
	objectList.push_back(jupiter);
	objectList.push_back(saturn);
	objectList.push_back(uranus);
	objectList.push_back(neptune);
	objectList.push_back(spaceship);
	objectList.push_back(bullet);
	//shader 
	color.load();
	color.use();
	//load textures
	GLuint one = texture.LoadTexture("textures/sun.jpg");
	GLuint two = texture.LoadTexture("textures/mercury.jpg");
	GLuint three = texture.LoadTexture("textures/venus.jpg");
	GLuint four = texture.LoadTexture("textures/earth.jpg");
	GLuint five = texture.LoadTexture("textures/mars.jpg");
	GLuint six = texture.LoadTexture("textures/jupiter.jpg");
	GLuint seven = texture.LoadTexture("textures/saturn.jpg");
	GLuint eight = texture.LoadTexture("textures/uranus.jpg");
	GLuint nine = texture.LoadTexture("textures/neptune.jpg");
	GLuint ten = texture.LoadTexture("textures/spaceship.png");
	GLuint eleven = texture.LoadTexture("textures/asteriod.png");
	//populate texture array 
	texIDs[0] = one;
	texIDs[1] = two;
	texIDs[2] = three;
	texIDs[3] = four;
	texIDs[4] = five;
	texIDs[5] = six;
	texIDs[6] = seven;
	texIDs[7] = eight;
	texIDs[8] = nine;
	texIDs[9] = ten;
	texIDs[10] = eleven;
	//lighting 
	glm::vec3 lightLoc(0.0f, 10.0f, 0.0f);
	glUniform3fv(3, 1, &lightLoc.x);
	glEnable(GL_DEPTH_TEST);

	bool canShoot = true;
	//input
	glfwSetKeyCallback(GLFWwindowPTR, keyCallback);

	while (!glfwGetKey(GLFWwindowPTR, GLFW_KEY_ESCAPE))
	{
		//wireframe render
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		//get delta t every frame
		t0 = t; //prev time
		t = (float)glfwGetTime(); //cur time
		dt = t - t0; // delta time
					
		//get key events 
		// update which key was down
		keyWasDown = keyIsDown;
		glfwPollEvents();

		//object pos
		for (int i = 0; i < objectList.size(); i++)
		{
			Transform trans = objectList[i].transform;
			objectList[i].transform.objectWorldTransform = glm::translate(trans.loc) * glm::yawPitchRoll(trans.rot.x, trans.rot.y, trans.rot.z) * glm::scale(trans.size);
		}
		//spaceship
		objectList[9].rigidBody.run(GLFWwindowPTR, dt);
		objectList[9].rigidBody.calcVelocity(dt);
		objectList[9].rigidBody.calcLoc(dt, objectList[9].transform.loc);
		//projectile
		objectList[10].rigidBody.calcVelocity(dt);
		objectList[10].rigidBody.calcLoc(dt, objectList[10].transform.loc);
		//planet orbit
		if (objectList[0].alive == true)
		{
			objectList[0].transform.rot.x += .01f;
		}
		for (int i = 1; i < 9; i++)
		{
			if (objectList[i].alive == true)
			{
				objectList[i].rigidBody.gravatationalForce(objectList[i].transform.loc, objectList[i].transform.rot);
			}
		}
				
		////spaceship collision 
		for (int i = 0; i < objectList.size(); i++)
		{
			if (collides(objectList[9], objectList[i]) && i != 9 && i !=10)
			{
				objectList[9].transform.loc = glm::vec3(10, 0, 0);
				std::cout << "spaceship crashed, you have respawned" << std::endl;
			}
			if (collides(objectList[10], objectList[i]) && i != 9 && i != 10)
			{
				objectList[i].alive = false;
				objectList[i].transform.loc = { 1000,0,0 };
				std::cout << "you destroyed a planet" << std::endl;
			}
		}

		//shoot
		if (keyWasDown[GLFW_KEY_SPACE] && canShoot == true)
		{
			canShoot = false;
			objectList[10].transform.loc = glm::vec3(objectList[9].transform.loc.x - 1.3f, objectList[9].transform.loc.y, objectList[9].transform.loc.z);
			objectList[10].rigidBody.force = {0,0,0};
			objectList[10].rigidBody.velocity = { 0,0,0 };
			objectList[10].rigidBody.shoot();
		}
		//if bullet leaves screen 
		if (objectList[10].transform.loc.x >= -30)
			canShoot = true;


		//change window color 
		glClearColor(0, 0, 0, 0);
		//clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//camera
		camera.update(objectList[9].transform.loc);
		camera.fpsControls(GLFWwindowPTR, dt);

		for (int i = 0; i < objectList.size(); i++)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindTexture(GL_TEXTURE_2D, texIDs[i]);
			glUniformMatrix4fv(7, 1, GL_FALSE, &objectList[i].transform.objectWorldTransform[0][0]);
			//bind and draw 
			objectList[i].model.render();
		}

		//swap the front and back color buffers 
		glBindVertexArray(0);
		glfwSwapBuffers(GLFWwindowPTR);

	}
	for (int i = 0; i < 11; i++)
	{
		glDeleteTextures(1, &texIDs[i]);
	}
	glfwTerminate();
	return 0;

}






