#version 430

layout (location = 4) uniform mat4 worldView;
layout (location = 7) uniform mat4 model;
layout(location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

layout (location = 6) in vec2 uv;

out float red;
out vec3 Normal; //normals for lighting 
out vec3 fragLoc; //location 
out vec2 fragUV; //cordinates to the frag shader for texture

void main()
{
	fragUV = uv;
	gl_Position =  worldView * model * vec4(position, 1);
	red = (position.x + 1.0f) / 2;

	fragLoc = position; 
	Normal = normal; 
}