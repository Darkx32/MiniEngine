//
// Created by matheus on 19/09/2026.
//

#ifndef MINIENGINE_SCRIPTSYSTEM_H
#define MINIENGINE_SCRIPTSYSTEM_H
#include <entt/fwd.hpp>

namespace MiniEngine
{
    struct ScriptSystem
    {
        static void startup(entt::registry* registry);
        static void update(entt::registry* registry, float dt);
    };
} // MiniEngine

#endif //MINIENGINE_SCRIPTSYSTEM_H
