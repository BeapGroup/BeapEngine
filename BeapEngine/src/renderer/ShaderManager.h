#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"

namespace beap
{
    class ShaderManager
    {
    private:
        std::vector<beap::Shader*> shaderList;
        
    public: // ------------------------- SHADER & SHADER SOURCE OPERATIONS ------------------
        // Shader operations
        bool AddShader(beap::Shader* shader);
        bool RemoveShader(beap::Shader* shader);
        std::vector<beap::Shader*> GetShaderList() { return shaderList; }
        int ShaderCount() { return shaderList.size(); }
        std::vector<const char*> GetShaderNames();

        // Shader source operations
        void RecompileShader(beap::Shader* shader);
        void ChangeShaderSource(beap::Shader* shader, const char* vertex, const char* fragment);
        void ChangeShaderFile(beap::Shader* shader, const char* vertex, const char* fragment);
        void ChangeShaderVertexSource(beap::Shader* shader, const char* vertex);
        void ChangeShaderFragmentSource(beap::Shader* shader, const char* fragment);
        void ChangeShaderVertexFile(beap::Shader* shader, const char* vertex);
        void ChangeShaderFragmentFile(beap::Shader* shader, const char* fragment);

        // Shader set operations
        void SetBool(const std::string& name, bool value);
        void SetInt(const std::string& name, int value);
        void SetFloat(const std::string& name, float value);
        void SetVec2(const std::string& name, const glm::vec2& value);
        void SetVec2(const std::string& name, float x, float y);
        void SetVec3(const std::string& name, const glm::vec3& value);
        void SetVec3(const std::string& name, float x, float y, float z);
        void SetVec4(const std::string& name, const glm::vec4& value);
        void SetVec4(const std::string& name, float x, float y, float z, float w);
        void SetMat2(const std::string& name, const glm::mat2& value);
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetMat4(const std::string& name, const glm::mat4& value);
    };
}
