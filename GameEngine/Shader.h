#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h.>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <string>

class Shader
{
public:
	Shader();
	virtual ~Shader();
	bool load();
	bool compile(GLenum shaderType);
	void use();
	void unload();
private:
	GLuint program, vprogram, fprogram;
	std::string filenamev = "shaders/vShader.glsl";
	std::string filenamef = "shaders/fShader.glsl";
};

