//
// Created by matheus on 12/10/2026.
//

#ifndef MINIENGINE_LIGHTSYSTEM_H
#define MINIENGINE_LIGHTSYSTEM_H

#include <cstdint>
#include <entt/fwd.hpp>

namespace MiniEngine
{
    struct ShaderData;
    struct Transform;
    struct Vector3;

    struct LightSystem
    {
        static void prepare(entt::registry* registry, const Transform& oTransform, const ShaderData& shaderData);
        static bool circleIntersectsCircle(Vector3 aPos, float aRadius, Vector3 bPos, float bRadius);

        static constexpr uint16_t MAX_LIGHTS = 16;
    };
} // MiniEngine

#endif //MINIENGINE_LIGHTSYSTEM_H
