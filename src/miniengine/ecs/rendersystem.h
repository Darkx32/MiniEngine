//
// Created by matheus on 16/09/2026.
//

#ifndef MINIENGINE_RENDERSYSTEM_H
#define MINIENGINE_RENDERSYSTEM_H
#include <entt/fwd.hpp>

namespace MiniEngine
{
    struct RenderSystem
    {
        static void render(entt::registry* registry);
    };
} // MiniEngine

#endif //MINIENGINE_RENDERSYSTEM_H
