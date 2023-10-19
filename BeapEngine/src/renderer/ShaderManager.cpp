#include "ShaderManager.h"

bool beap::ShaderManager::AddShader(beap::Shader* shader)
{
    beap::shaders::FShaderOperationResult r;
    AddShader(shader, r);
    return r.Success;
}

void beap::ShaderManager::AddShader(beap::Shader* shader, beap::shaders::FShaderOperationResult& result)
{
    if (HasShader(shader))
    {
        result.Success = false;
        result.FailReason = beap::shaders::SHADER_ALREADY_IN_LIST;
        result.ExtraInformation = "Requested shader is already in shader manager list. Only one instance of the same shader can exist in the list at the same time";
        beap::utils::Logger::LogError("ShaderManager::AddShader Error | Tried to add already in list shader to the list.", beap::utils::ELogErrorSeverity::WARNING);
    }
    else
    {
        shaderList.push_back(shader);
    }
}

bool beap::ShaderManager::RemoveShader(beap::Shader* shader)
{

    return false;
}

bool beap::ShaderManager::HasShader(beap::Shader* shader)
{
    bool contains = false;
    for (Shader* s : shaderList)
    {
        if (s == shader)
        {
            contains = true;
        }
    }
    return contains;
}

beap::shaders::generation::FShaderGenerationResult beap::ShaderManager::GenerateShader(beap::shaders::generation::EShaderType shaderType, beap::shaders::generation::FShaderGenerationSettings settings)
{
    beap::shaders::generation::FShaderGenerationResult result;

    switch (shaderType)
    {
    case beap::shaders::generation::VERTEX:
        result = GenerateVertexShader(settings);
        break;
    case beap::shaders::generation::FRAGMENT:
        result = GenerateFragmentShader(settings);
        break;
    case beap::shaders::generation::GEOMETRY:
        break;
    default:
        beap::utils::Logger::Log("");
        break;
    }


    return result;
}

// WIP
beap::shaders::generation::FShaderGenerationResult beap::ShaderManager::GenerateVertexShader(beap::shaders::generation::FShaderGenerationSettings settings)
{
    std::string vertexSource = "";
    
    // Include shader header
    vertexSource += settings.GLSLHeader + '\n';

    vertexSource += "layout(location = 0) in vec3 aPos\n";
    vertexSource += "layout(location = 1) in vec3 aNormal\n";
    vertexSource += "layout(location = 2) in vec3 aColor\n";
    vertexSource += "layout(location = 3) in vec2 aTexCoords\n";

    return beap::shaders::generation::FShaderGenerationResult();
}

beap::shaders::generation::FShaderGenerationResult beap::ShaderManager::GenerateFragmentShader(beap::shaders::generation::FShaderGenerationSettings settings)
{
    std::string fragmentSource = "";

    // Include shader header
    fragmentSource += "#version ";
    fragmentSource += settings.GLSLHeader;
    fragmentSource += '\n';

    fragmentSource += "out vec4 FragColor;\n";
    fragmentSource += "in vec2 TexCoord;\n";
    fragmentSource += "in vec3 Normal;\n";
    fragmentSource += "in vec3 crntPos;\n";


    if (settings.hasTexture)
    {
        fragmentSource += "uniform sampler2D diffuseTexture;\n";
    }
    else
    {
        // use "color" instead of "colour" because.... well just because.
        fragmentSource += "uniform vec4 diffuseColor;\n";
    }

    if (settings.useAmbientLighting)
    {
        fragmentSource += "uniform vec4 ambientColor;\n";
    }

    // Generate the FLightData struct in the shader.
    if (settings.hasLighting)
    {
        fragmentSource += "#define MAX_LIGHTS 16\n";
        fragmentSource += "struct Light {\n";
        fragmentSource += "vec3 lightColor;\n";
        fragmentSource += "vec3 lightPos;\n";
        fragmentSource += "float intensity;\n";
        fragmentSource += "float attenuation;\n";
        fragmentSource += "}\n";
        fragmentSource += "Light lights[MAX_LIGHTS];\n";
    }

    fragmentSource += "void main() {\n"; // ============================ MAIN

#pragma region DiffuseTextureStandardisation
    if (settings.hasTexture)
    {
        fragmentSource += "vec4 diffuseVar = texture(diffuseTexture, TexCoord);\n";
    }
    else
    {
        fragmentSource += "vec4 diffuseVar = diffuseColor;s\n";
    }
#pragma endregion
    
    fragmentSource += "vec4 finalOutput;\n";

#pragma region Ambient
    if (settings.useAmbientLighting)
    {
        // use the alpha value of ambientColor to detemine to ambient light power.
        fragmentSource += "float ambient = ambientColor.a;\n";
    }
    else
    {
        fragmentSource += "float ambient = 0.0f;\n";
    }
#pragma endregion


    fragmentSource += "FragColor = finalOutput;\n";
    fragmentSource += "}";

    beap::shaders::generation::FShaderGenerationResult res;
    res.Result = fragmentSource.c_str();

    // Try compiling the shader and check for errors

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* src = fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &src, NULL);
    glCompileShader(fragmentShader);
    bool success = checkCompileErrors(fragmentShader);
    res.Success = success;

    std::cout << res.Result << std::endl;
    return res;
}
