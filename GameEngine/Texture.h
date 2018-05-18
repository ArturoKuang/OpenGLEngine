#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h.>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>

class Texture
{
public:
	Texture();
	virtual GLuint LoadTexture(const char* fileName); //gets the file and loads a texture 
	GLuint texInt;
	~Texture();
};

