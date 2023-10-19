#pragma once

#include "Light.h"
#include <vector>

namespace beap
{
    namespace renderer
    {
        class LightManager
        {
            public:
            std::vector<Light*> lights;

            void AddLight(Light* light)
            {
                lights.push_back(light);
            }
        };
    }
}