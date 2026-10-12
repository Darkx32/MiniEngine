//
// Created by matheus on 10/10/2026.
//

#ifndef MINIENGINE_LIGHTS_H
#define MINIENGINE_LIGHTS_H

namespace MiniEngine
{
    struct PointLight2D
    {
        float color[3] = {1.0f, 1.0f, 1.0f};

        float intensity = 1.0f;
        float radius = 300.0f;
        float falloff = 2.0f;

        bool enabled = true;
    };

    struct SpotLight2D
    {
        float color[3] = {1.0f, 1.0f, 1.0f};

        float intensity = 1.0f;
        float radius = 400.0f;

        float innerAngle = 20.0f;
        float outerAngle = 35.0f;

        float falloff = 2.0f;

        bool enabled = true;
    };
} // MiniEngine

#endif //MINIENGINE_LIGHTS_H
