//
// Created by matheus on 18/10/2026.
//

#ifndef MINIENGINE_SHADOWSYSTEM_H
#define MINIENGINE_SHADOWSYSTEM_H

namespace MiniEngine
{
    struct ShaderData;
    struct Transform;

    struct ShadowSystem
    {
        static void prepare(entt::registry* registry, entt::entity renderEntity, const ShaderData& shaderData);

        static constexpr uint16_t MAX_SHADOW_SEGMENTS = 32;
    };
} // MiniEngine

#endif //MINIENGINE_SHADOWSYSTEM_H
