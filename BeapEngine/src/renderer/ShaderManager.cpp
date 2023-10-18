#include "ShaderManager.h"

bool beap::ShaderManager::AddShader(beap::Shader* shader)
{
    shaderList.push_back(shader);
    return true;
}

