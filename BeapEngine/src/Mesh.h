#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "thirdparty/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"

class Mesh
{
    
public:
    unsigned int VAO, VBO, EBO;

    //Replace with material struct later
    unsigned int texture;

    Shader* shader;

    glm::vec3 translation;
    glm::vec3 eulerRotation;
    glm::vec3 scale = glm::vec3(1.0f);

    void Draw();
    void SetupMesh();
    void SetupTexture(const char* texturePath);
};


#endif