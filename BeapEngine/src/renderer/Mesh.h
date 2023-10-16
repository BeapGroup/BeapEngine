#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../thirdparty/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include "Shader.h"
#include <vector>
#include "gl/VAO.h"
#include "gl/EBO.h"

namespace beap {

    class Mesh
    {

    public:
        VAO VAO;
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        //Replace with material struct later
        unsigned int texture;

        Shader* shader;

        glm::vec3 translation = glm::vec3(0.0f);
        glm::vec3 eulerRotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        Mesh* parent;
        Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

        void Draw();
        void SetupMesh();
        void SetupTexture(const char* texturePath);

        //static Mesh Plane(glm::f32vec3 p1, glm::f32vec3 p2);
    };

}
