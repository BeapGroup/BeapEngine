#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 crntPos;

// Replace with material struct later. That can wait for now.
uniform sampler2D tex;

uniform vec3 lightPos;
uniform vec4 lightCol;

void main()
{


	FragColor = texture(tex, TexCoord);
}