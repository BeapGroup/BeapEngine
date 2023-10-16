#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 crntPos;

// Replace with material struct later. That can wait for now.
uniform sampler2D tex;

uniform vec3 lightPos;

void main()
{
	float ambient = 0.20f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	FragColor = texture(tex, TexCoord) * (diffuse + ambient);
}