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


Shader::Shader()
{
}


Shader::~Shader()
{
}

bool Shader::load()
{
	if (compile(GL_VERTEX_SHADER) && compile(GL_FRAGMENT_SHADER))
	{
		//links the shaders
		program = glCreateProgram();
		glAttachShader(program, vprogram);
		glAttachShader(program, fprogram);
		glLinkProgram(program);
		//checks if the linking is sucessful
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (linked)
		{
			return true; //program is linked 
		}
		else //program is not linked 
		{
			//gets log length
			GLint logLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
			//array to hold log
			GLchar* compilerLog = new GLchar[logLength];
			glGetProgramInfoLog(program, logLength, 0, &compilerLog[0]);
			for (int i = 0; i < logLength; i++)
			{
				std::cout << compilerLog[i];
			}

			//memory clean up
			glDeleteProgram(program);
			delete compilerLog;
			return false;
		}
	
	}
	else //vertex shader and fragment shader didnt compile
	{
		return false;
	}
	
}
//load to read and compiler a shader file and save the index where it is stored
bool Shader::compile(GLenum shaderType)
{
	std::string fileName;
	char* fileContents;
	//determine if the shader compiled
	GLint blen = 0;
	
	//determines the file type 
	if (shaderType == GL_VERTEX_SHADER)
	{
		fileName = filenamev;
	}
	else
	{
		fileName = filenamef;
	}
	//opens the file 
	std::ifstream inFile(fileName, std::ios::binary);
	if (inFile.is_open())
	{
		inFile.seekg(0, std::ios::end);
		int length = inFile.tellg();
		inFile.seekg(0, std::ios::beg);
		fileContents = new char[length + 1];
		fileContents[length] = '\0';
		inFile.read(fileContents, length);
		inFile.close();
		//create openGL vertex shader 
		if (shaderType == GL_VERTEX_SHADER) {
			vprogram = glCreateShader(shaderType);
			program = vprogram;
			glShaderSource(vprogram, 1, &fileContents, 0);
			glCompileShader(vprogram);
			//checks if the shader compiled(0 = false)
			glGetShaderiv(vprogram, GL_COMPILE_STATUS, &blen);
		}
		else {
			fprogram = glCreateShader(shaderType);
			program = fprogram;
			glShaderSource(fprogram, 1, &fileContents, 0);
			glCompileShader(fprogram);
			//checks if the shader compiled(0 = false)
			glGetShaderiv(fprogram, GL_COMPILE_STATUS, &blen);
		}
		delete[] fileContents; //deletes filecontents on stack 
		//shader didnt compile(maybe syntax error)
		//returns a shader log
		if (blen == 1)
		{
			//shader compiled 
			return true;
		}
		else
		{
			std::cout << "Error Compile Failure" << std::endl;
			//length of shader log
			GLint logLength;
			//gets the shader log length
			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLength);
			//array to hold log
			GLchar* compilerLog = new GLchar[logLength];
			//gets log
			glGetShaderInfoLog(program, logLength, 0, compilerLog);
			std::cout << compilerLog;
			//memory clean up 
			glDeleteShader(glCreateShader(shaderType));
			delete[] compilerLog;
			return false;
		}
		
	}
	else
	{
		std::cout << "Error Loading File" << std::endl;
	}
	return false;
}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram);

	program = 0;
	vprogram = 0;
	fprogram = 0;
}
