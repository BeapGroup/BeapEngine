#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

namespace beap {

	// Structure to standardize the vertices used in the meshes
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texUV;

		Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec3 col, glm::vec2 uv)
			: position(pos), normal(norm), color(col), texUV(uv) {}
	};



	class VBO
	{
	public:
		// Reference ID of the Vertex Buffer Object
		GLuint ID;
		// Constructor that generates a Vertex Buffer Object and links it to vertices
		VBO(std::vector<Vertex>& vertices);

		// Binds the VBO
		void Bind();
		// Unbinds the VBO
		void Unbind();
		// Deletes the VBO
		void Delete();
	};

}

