#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "../utils/Logger.h"
#include <glad/glad.h>

namespace beap
{
    namespace shaders
    {
        enum EShaderOperationFailReason
        {
            SHADER_ALREADY_IN_LIST,
            SHADER_NOT_IN_LIST,
            LIST_TOO_FULL,
            SHADER_FILE_DOESNT_EXIST,
            SHADER_COMPILE_ERROR_VERTEX,
            SHADER_COMPILE_ERROR_FRAGMENT,
            SHADER_COMPILE_ERROR_GEOMETRY,
            SHADER_COMPILE_ERROR_PROGRAM
        };
        struct FShaderOperationResult
        {
            bool Success;
            EShaderOperationFailReason FailReason;
            const char* ExtraInformation;
        };

        namespace generation
        {
            enum EShaderType
            {
                VERTEX,
                FRAGMENT,
                GEOMETRY
            };

            // The tabs/indentations signify that an option is only editable if the parent option is enabled.
            struct FShaderGenerationSettings
            {
                char GLSLHeader[30];
                bool hasTexture;
                bool hasLighting;
                    bool useAmbientLighting;
                    bool isBlinnPhong;
                    bool hasDiffuse;
                    bool hasSpecular;
                    bool hasShadows;
                    bool hasPointShadows;
                bool hasNormalMaps;
                bool hasParallaxOcclusionMaps;
            };

            struct FShaderGenerationResult
            {
                bool Success = false;
                bool SuccessWithErrors = false;
                const char* Result;
            };
        }
    }
    
    class ShaderManager
    {
    private:
        std::vector<beap::Shader*> shaderList;
        
    public: // ------------------------- SHADER & SHADER SOURCE OPERATIONS ------------------
        // Shader operations
        bool AddShader(beap::Shader* shader);
        void AddShader(beap::Shader* shader, beap::shaders::FShaderOperationResult& result);
        bool RemoveShader(beap::Shader* shader);
        void RemoveShader(beap::Shader* shader, beap::shaders::FShaderOperationResult& result);
        bool HasShader(beap::Shader* shader);
        void HasShader(beap::Shader* shader, beap::shaders::FShaderOperationResult& result);
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

    public: // ---------------- SHADER GENERATION STUFF ------------------ VERY EXPERIMENTAL -------------------------
        beap::shaders::generation::FShaderGenerationResult cachedResult = beap::shaders::generation::FShaderGenerationResult();
        beap::shaders::generation::FShaderGenerationResult GenerateShader(beap::shaders::generation::EShaderType shaderType, beap::shaders::generation::FShaderGenerationSettings settings);
        beap::shaders::generation::FShaderGenerationResult GenerateVertexShader(beap::shaders::generation::FShaderGenerationSettings settings);
        beap::shaders::generation::FShaderGenerationResult GenerateFragmentShader(beap::shaders::generation::FShaderGenerationSettings settings);

        bool checkCompileErrors(GLuint shader)
        {
            GLint success;
            GLchar infoLog[1024];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else
            {
                std::cout << "SUCCESS! Shader compiled successfully!" << std::endl;
                std::cout << "Warning, this isn't a guarantee that the shader will run successfully, it just means there isn't any compilation errors." << std::endl;

            }
            return success;
        }
    };
}
