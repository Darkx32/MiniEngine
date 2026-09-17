//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_SCENE_H
#define MINIENGINE_SCENE_H

#include <entt/fwd.hpp>

namespace MiniEngine
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        [[nodiscard]] Entity createEntity();

    private:
        friend class Engine;
        friend class Entity;
        entt::registry* registry;
    };
} // MiniEngine

#endif //MINIENGINE_SCENE_H
