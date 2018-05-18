#version 430


out vec4 fragColor;

in float red;
in vec3 Normal; //fragment normal vector 
in vec3 fragLoc; //fragment location 

in vec2 fragUV; //uvs for texture
uniform sampler2D tex; //read texture data 

layout(location = 1) uniform float time;
layout(location = 3) uniform vec3 lightLoc;
layout(location = 5) uniform vec3 camLoc;

void main()
{
	vec3 L = normalize(lightLoc - fragLoc); //fragment to light
	vec3 V = normalize(camLoc - fragLoc); //fragment to camera
	vec3 H = normalize(V+L); //half way vector
	vec3 N = normalize(Normal);
	
	//Ambient
	float ambientStrength = .1f;
	float ambient = ambientStrength;

	//diffuse 
	float diff = max(dot(L,N),0.0);
	float diffuse = .5f * diff;

	//specular 
	float specularStrength = .5f;
	float spec = pow(max(dot(H, N), 0.0), 32);
	float specular = specularStrength * spec;

	float brightness = ambient + diffuse + specular;
	vec4 rgba = texture(tex, fragUV);
	gl_FragColor =  vec4(rgba.rgb * (brightness * 6), rgba.a);
}