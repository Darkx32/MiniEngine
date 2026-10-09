//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_SCENE_H
#define MINIENGINE_SCENE_H
#include "miniengine/ecs/entity.h"

namespace MiniEngine
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        [[nodiscard]] Entity createEntity();

        template <typename... Components>
        [[nodiscard]] std::vector<Entity> getAllEntitiesWithComponents()
        {
            std::vector<Entity> entities;

            for (auto view = registry->view<Components...>(); auto handle : view)
            {
                entities.push_back(Entity(handle, this, registry));
            }

            return entities;
        }

    private:
        friend class Engine;
        friend class Entity;
        entt::registry* registry;
    };
} // MiniEngine

#endif //MINIENGINE_SCENE_H
