#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// Replace with material struct later. That can wait for now.
uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, TexCoord);
}