#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h.>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include "Shader.h"
#include "Shader.h"
#include <fstream>
#include <iostream>
class Model
{
public:
	Model();
	bool buffer(std::string objFile);
	void render();
	virtual ~Model();
private:
	unsigned int vertCount;
	GLuint vertArr;
};
struct Vertex
{
	glm::vec3 loc;
	glm::vec2 uv;
	glm::vec3 norm;
};
struct VertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};

