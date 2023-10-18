#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 Normal;
out vec3 crntPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 m = view * model;

	m[0][0] = 1;
	m[0][2] = 0;
	m[1][0] = 0;
	m[1][2] = 0;
	m[2][0] = 0;
	//m[2][2] = 1;

	gl_Position = projection * m * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoords.x, aTexCoords.y);
	Normal = aNormal;
	crntPos = vec3(model * vec4(aPos, 1.0f));
}