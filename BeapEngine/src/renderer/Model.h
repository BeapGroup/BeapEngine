#pragma once

#include <json/json.h>
#include "Mesh.h"

using json = nlohmann::json;

namespace beap {

class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const char* file);
	Model() = default;

	void Draw();
	std::vector<Mesh> meshes;

	static Model Plane(glm::f32vec3 p1, glm::f32vec3 p2);
	static Model Cube(glm::f32vec3 p1, glm::f32vec3 p2);
private:
	// Variables for easy access
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	// All the meshes and transformations

	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Loads a single mesh by its index
	void loadMesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> getData();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);

	// Assembles all the floats into vertices
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

}

