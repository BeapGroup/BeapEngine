#include "Model.h"

namespace beap {

	std::string get_file_contents(const char* filename)
	{
		if (std::ifstream in(filename, std::ios::binary); in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return contents;
		}
		throw(errno);
	}

	Model::Model(const char* file)
	{
		// Make a JSON object
		std::string text = get_file_contents(file);
		JSON = json::parse(text);

		// Get the binary data
		Model::file = file;
		data = getData();

		// Traverse all nodes
		traverseNode(0);
	}

	void Model::Draw()
	{
		// Go over all meshes and draw each one
		for (auto& mesh : meshes) {
			mesh.Draw();
		}
	}

	void Model::loadMesh(unsigned int indMesh)
	{
		// Get all accessor indices
		unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
		unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
		unsigned int texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
		unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

		// Use accessor indices to get all vertices components
		std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
		std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
		std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
		std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
		std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
		std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

		// Combine all the vertex components and also get the indices and textures
		std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
		std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);

		// Combine the vertices, indices, and textures into a mesh
		meshes.emplace_back(vertices, indices);
	}

	void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix)
	{
		// Current node
		json node = JSON["nodes"][nextNode];

		// Get translation if it exists
		auto translation = glm::vec3(0.0f, 0.0f, 0.0f);
		if (node.find("translation") != node.end())
		{
			float transValues[3];
			for (unsigned int i = 0; i < node["translation"].size(); i++)
				transValues[i] = (node["translation"][i]);
			translation = glm::make_vec3(transValues);
		}
		// Get quaternion if it exists
		auto rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		if (node.find("rotation") != node.end())
		{
			float rotValues[4] =
			{
				node["rotation"][3],
				node["rotation"][0],
				node["rotation"][1],
				node["rotation"][2]
			};
			rotation = glm::make_quat(rotValues);
		}
		// Get scale if it exists
		auto scale = glm::vec3(1.0f, 1.0f, 1.0f);
		if (node.find("scale") != node.end())
		{
			float scaleValues[3];
			for (unsigned int i = 0; i < node["scale"].size(); i++)
				scaleValues[i] = (node["scale"][i]);
			scale = glm::make_vec3(scaleValues);
		}
		// Get matrix if it exists
		auto matNode = glm::mat4(1.0f);
		if (node.find("matrix") != node.end())
		{
			float matValues[16];
			for (unsigned int i = 0; i < node["matrix"].size(); i++)
				matValues[i] = (node["matrix"][i]);
			matNode = glm::make_mat4(matValues);
		}

		// Initialize matrices
		auto trans = glm::mat4(1.0f);
		auto rot = glm::mat4(1.0f);
		auto sca = glm::mat4(1.0f);

		// Use translation, rotation, and scale to change the initialized matrices
		trans = glm::translate(trans, translation);
		rot = glm::mat4_cast(rotation);
		sca = glm::scale(sca, scale);

		// Multiply all matrices together
		glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

		// Check if the node contains a mesh and if it does load it
		if (node.find("mesh") != node.end())
		{
			translationsMeshes.push_back(translation);
			rotationsMeshes.push_back(rotation);
			scalesMeshes.push_back(scale);
			matricesMeshes.push_back(matNextNode);

			loadMesh(node["mesh"]);
		}

		// Check if the node has children, and if it does, apply this function to them with the matNextNode
		if (node.find("children") != node.end())
		{
			for (unsigned int i = 0; i < node["children"].size(); i++)
				traverseNode(node["children"][i], matNextNode);
		}
	}

	std::vector<unsigned char> Model::getData()
	{
		// Create a place to store the raw text, and get the uri of the .bin file
		std::string bytesText;
		std::string uri = JSON["buffers"][0]["uri"];

		// Store raw text data into bytesText
		auto fileStr = std::string(file);
		std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
		bytesText = get_file_contents((fileDirectory + uri).c_str());

		// Transform the raw text data into bytes and put them in a vector
		std::vector<unsigned char> textdata(bytesText.begin(), bytesText.end());
		return textdata;
	}

	std::vector<float> Model::getFloats(json accessor)
	{
		std::vector<float> floatVec;

		// Get properties from the accessor
		unsigned int buffViewInd = accessor.value("bufferView", 1);
		unsigned int count = accessor["count"];
		unsigned int accByteOffset = accessor.value("byteOffset", 0);
		std::string type = accessor["type"];

		// Get properties from the bufferView
		json bufferView = JSON["bufferViews"][buffViewInd];
		unsigned int byteOffset = bufferView["byteOffset"];

		// Interpret the type and store it into numPerVert
		unsigned int numPerVert;
		if (type == "SCALAR") numPerVert = 1;
		else if (type == "VEC2") numPerVert = 2;
		else if (type == "VEC3") numPerVert = 3;
		else if (type == "VEC4") numPerVert = 4;
		else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

		// Go over all the bytes in the data at the correct place using the properties from above
		unsigned int beginningOfData = byteOffset + accByteOffset;
		unsigned int lengthOfData = count * 4 * numPerVert;
		for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i)
		{
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
			float value;
			std::memcpy(&value, bytes, sizeof(float));
			floatVec.push_back(value);
		}

		return floatVec;
	}

	std::vector<GLuint> Model::getIndices(json accessor)
	{
		std::vector<GLuint> indices;

		// Get properties from the accessor
		unsigned int buffViewInd = accessor.value("bufferView", 0);
		unsigned int count = accessor["count"];
		unsigned int accByteOffset = accessor.value("byteOffset", 0);
		unsigned int componentType = accessor["componentType"];

		// Get properties from the bufferView
		json bufferView = JSON["bufferViews"][buffViewInd];
		unsigned int byteOffset = bufferView["byteOffset"];

		// Get indices with regards to their type: unsigned int, unsigned short, or short
		unsigned int beginningOfData = byteOffset + accByteOffset;
		if (componentType == 5125)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i)
			{
				unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
				unsigned int value;
				std::memcpy(&value, bytes, sizeof(unsigned int));
				indices.push_back(value);
			}
		}
		else if (componentType == 5123)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
			{
				unsigned char bytes[] = { data[i++], data[i++] };
				unsigned short value;
				std::memcpy(&value, bytes, sizeof(unsigned short));
				indices.push_back((GLuint)value);
			}
		}
		else if (componentType == 5122)
		{
			for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i)
			{
				unsigned char bytes[] = { data[i++], data[i++] };
				short value;
				std::memcpy(&value, bytes, sizeof(short));
				indices.push_back((GLuint)value);
			}
		}

		return indices;
	}



	std::vector<Vertex> Model::assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	)
	{
		std::vector<Vertex> vertices;
		for (int i = 0; i < positions.size(); i++)
		{
			vertices.push_back
			(
				Vertex
				{
					positions[i],
					normals[i],
					glm::vec3(1.0f, 1.0f, 1.0f),
					texUVs[i]
				}
			);
		}
		return vertices;
	}

	std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
	{
		std::vector<glm::vec2> vectors;
		for (int i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}
	std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
	{
		std::vector<glm::vec3> vectors;
		for (int i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}
	std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
	{
		std::vector<glm::vec4> vectors;
		for (int i = 0; i < floatVec.size(); i)
		{
			vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
		}
		return vectors;
	}

	Model Model::Plane(glm::vec3 p1, glm::vec3 p2) {
		auto model = Model();
		auto normal = glm::vec3(0, 1, 0);
		auto color = glm::vec3(0);

		auto v_0 = Vertex(p1, normal, color, glm::vec2(0, 0));
		auto v_1 = Vertex(glm::vec3(p2.x, p1.y, p1.z), normal, color, glm::vec2(1, 0));
		auto v_2 = Vertex(glm::vec3(p1.x, p1.y, p2.z), normal, color, glm::vec2(0, 1));
		auto v_3 = Vertex(p2, normal, color, glm::vec2(1, 1));

		auto vertices = std::vector<Vertex>{ v_0, v_1, v_2, v_3 };
		auto indices = std::vector<unsigned int>{ 0, 1, 3, 0, 2, 3 };

		model.meshes.emplace_back(vertices, indices);
		return model;
	}

	Model Model::Cube(glm::vec3 p1, glm::vec3 p2) {
		float n = glm::sqrt(2) / 2;
		auto model = Model();
		auto color = glm::vec3(0);
		

		// top plane vertices
		auto tp_v0 = Vertex(p1, glm::vec3(-n, n, -n), color, glm::vec2(0, 0));
		auto tp_v1 = Vertex(glm::vec3(p2.x, p1.y, p1.z), glm::vec3(n, n, -n), color, glm::vec2(1, 0));
		auto tp_v2 = Vertex(glm::vec3(p1.x, p1.y, p2.z), glm::vec3(-n, n, n), color, glm::vec2(0, 1));
		auto tp_v3 = Vertex(glm::vec3(p2.x, p1.y, p2.z), glm::vec3(n, n, n), color, glm::vec2(1, 1));

		// bottom plane vertices
		auto bp_v0 = Vertex(glm::vec3(p1.x, p2.y, p1.z), glm::vec3(-n, -n, -n), color, glm::vec2(0, 0));
		auto bp_v1 = Vertex(glm::vec3(p2.x, p2.y, p1.z), glm::vec3(n, -n, -n), color, glm::vec2(1, 0));
		auto bp_v2 = Vertex(glm::vec3(p1.x, p2.y, p2.z), glm::vec3(-n, -n, n), color, glm::vec2(0, 1));
		auto bp_v3 = Vertex(p2, glm::vec3(n, n, n), color, glm::vec2(1, 1));

		auto vertices = std::vector<Vertex>{ tp_v0, tp_v1, tp_v2, tp_v3, bp_v0, bp_v1, bp_v2, bp_v3, };
		auto indices = std::vector<unsigned int>{
			//top plane
			0, 1, 3, 0, 2, 3,
			//left plane
			0, 2, 4, 2, 6, 4,
			// right plane
			1, 3, 5, 3, 7, 5,
			//bottom plane
			2, 3, 7, 2, 6, 7,
			//back plane
			0, 1, 4, 1, 5, 4,
			//front plane
			2, 3, 6, 3, 6, 7
		};

		model.meshes.emplace_back(vertices, indices);
		return model;
	}
}

