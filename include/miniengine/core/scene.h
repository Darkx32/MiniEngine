//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_SCENE_H
#define MINIENGINE_SCENE_H
#include <vector>
#include "miniengine/ecs/entity.h"

namespace MiniEngine
{
    class Scene
    {
    public:
        Scene();
        virtual ~Scene() = default;

        virtual void initialize();

        template <typename... Components>
        [[nodiscard]] std::vector<Entity> getAllEntitiesWithComponents()
        {
            std::vector<Entity> entities;

            for (auto view = registry->view<Components...>(); auto handle : view)
            {
                entities.push_back(Entity(handle, this, registry.get()));
            }

            return entities;
        }

    protected:
        [[nodiscard]] Entity createEntity();

    private:
        friend class Engine;
        friend class Entity;
        std::unique_ptr<entt::registry> registry;
    };
} // MiniEngine

#endif //MINIENGINE_SCENE_H
