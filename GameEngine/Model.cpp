#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>


Model::Model()
{
	vertArr = 0;
	vertCount = 0;
}

bool Model::buffer(std::string objFile)
{
	std::vector<glm::vec3> locations; //stores vector locations
	std::vector<glm::vec2> uvs; //stores model uv
	std::vector<glm::vec3> normals; //stores normals
	std::vector<VertInd> vertInds; //stores vertix indices 

	// open the file for reading
	std::ifstream inFile;
	inFile.open(objFile);

	// input string using stringstream to parse file
	std::string input;

	// if file opened right
	if (inFile.is_open())
	{
		// loop through file till empty
		while (getline(inFile, input))
		{
			std::istringstream ss(input);
			std::string lineLabel;

			ss >> lineLabel;

			if (lineLabel == "v") // LOCATIONS
			{
				float a, b, c;
				ss >> a >> b >> c;
				locations.push_back(glm::vec3(a, b, c));
			}
			else if (lineLabel == "vt") // UVS
			{
				float a, b;
				ss >> a >> b;
				uvs.push_back(glm::vec2(a, b));
			}
			else if (lineLabel == "vn") // NORMALS
			{
				float a, b, c;
				ss >> a >> b >> c;
				normals.push_back(glm::vec3(a, b, c));
			}
			else if (lineLabel == "f") // INDICES
			{
				// do three times
				for (int i = 0; i < 3; i++)
				{
					unsigned int a, b, c;
					char s1, s2;
					// write int then slash...
					ss >> a >> s1 >> b >> s2 >> c;
					// decrement each int by one and add to vertInds
					vertInds.push_back(VertInd{ a - 1, b - 1, c - 1 });
				}
			}
		}
		// close the file
		inFile.close();
	}
	else
	{
		std::cout << "ERROR LOADING MODEL FILE";
		return false;
	}

	vertCount = vertInds.size();

	// duplicate location and uv vertices into a single buffer
	std::vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
		vertBufData[i] = { locations[vertInds[i].locInd],
		uvs[ vertInds[i].uvInd],
		normals[vertInds[i].normInd] };

	// generate buffer
	GLuint vertBuf;
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);

	// bind buffer
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	// storing data in the buffer
	glBufferData(GL_ARRAY_BUFFER, // where to copy to
		sizeof(Vertex) * vertCount, // # of bytes to copy
		&vertBufData[0],		  // where to copy from
		GL_STATIC_DRAW);		  // "hint" to OpenGL

								  // enable the attribute
	glEnableVertexAttribArray(0);

	// set up the attribute
	glVertexAttribPointer(
		0,					// attribute index - 0 in this case
		3,					// number of components
		GL_FLOAT,			// type of data
		GL_FALSE,			// should we normalize data?
		sizeof(Vertex),	    // Stride (bytes per vertex)
		0);					// offset to this attribute

	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(5 * sizeof(GLfloat)));
	
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(VertInd));
	return true;
}

void Model::render()
{
	//bind vertex array
	glBindVertexArray(vertArr);
	//draw the vertex array
	glDrawArrays(GL_TRIANGLES, 0, vertCount);
}



Model::~Model()
{
}
