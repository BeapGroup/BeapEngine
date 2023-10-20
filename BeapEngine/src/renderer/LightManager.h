#pragma once

#include <vector>
#include "../utils/Light.h"

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