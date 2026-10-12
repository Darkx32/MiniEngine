//
// Created by matheus on 10/10/2026.
//

#ifndef MINIENGINE_LIGHTS_H
#define MINIENGINE_LIGHTS_H
#include "miniengine/math/vector3.h"

namespace MiniEngine
{
    struct PointLight2D
    {
        Vector3 color = {1.0f, 1.0f, 1.0f};

        float intensity = 1.0f;
        float radius = 300.0f;
        float falloff = 2.0f;

        bool enabled = true;
    };

    struct SpotLight2D
    {
        Vector3 color = {1.0f, 1.0f, 1.0f};

        float intensity = 1.0f;
        float radius = 400.0f;

        float innerAngle = 20.0f;
        float outerAngle = 35.0f;

        float falloff = 2.0f;

        bool enabled = true;
    };

    struct AmbientLight2D
    {
        Vector3 color = {1.0f, 1.0f, 1.0f};

        float intensity = 0.0f;
    };
} // MiniEngine

#endif //MINIENGINE_LIGHTS_H
