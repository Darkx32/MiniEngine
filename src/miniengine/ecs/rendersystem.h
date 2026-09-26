//
// Created by matheus on 16/09/2026.
//

#ifndef MINIENGINE_RENDERSYSTEM_H
#define MINIENGINE_RENDERSYSTEM_H
#include <entt/fwd.hpp>

namespace MiniEngine
{
    struct Vector2;

    struct RenderSystem
    {
        static void calculate(entt::registry* registry, const Vector2& windowSize);
        static void render(entt::registry* registry);
    };
} // MiniEngine

#endif //MINIENGINE_RENDERSYSTEM_H
