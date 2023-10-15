#include "Mesh.h"

namespace beap {

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
    {
        Mesh::vertices = vertices;
        Mesh::indices = indices;

        VAO.Bind();
        // Generates Vertex Buffer Object and links it to vertices
        VBO VBO(vertices);
        // Generates Element Buffer Object and links it to indices
        EBO EBO(indices);
        // Links VBO attributes such as coordinates and colors to VAO
        VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
        VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
        VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
        VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
        // Unbind all to prevent accidentally modifying them
        VAO.Unbind();
        VBO.Unbind();
        EBO.Unbind();
    }

    void Mesh::Draw()
    {

        shader->use();
        glActiveTexture(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        auto model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(eulerRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(eulerRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(eulerRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        shader->setMat4("model", model);

        VAO.Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    void Mesh::SetupMesh()
    {
        // Deprecated
    }

    void Mesh::SetupTexture(const char* texturePath)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Configures the way the texture repeats (if it does at all)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

        if (nrChannels == 4)
            glTexImage2D
            (
                GL_TEXTURE_2D,
                0,
                GL_SRGB_ALPHA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );
        else if (nrChannels == 3)
            glTexImage2D
            (
                GL_TEXTURE_2D,
                0,
                GL_SRGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
        else if (nrChannels == 1)
            glTexImage2D
            (
                GL_TEXTURE_2D,
                0,
                GL_SRGB,
                width,
                height,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                data
            );
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);

        shader->use();
        shader->setInt("tex", 0);
    }

}

