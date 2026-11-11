//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_SCENE_H
#define MINIENGINE_SCENE_H
#include <vector>
#include "miniengine/ecs/entity.h"

namespace MiniEngine
{
    class engine;

    class Scene
    {
    public:
        explicit Scene(engine* engine);
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

        [[nodiscard]] engine* getEngine() const;

    protected:
        [[nodiscard]] Entity createEntity();

    private:
        friend class engine;
        friend class Entity;
        std::unique_ptr<entt::registry> registry;
        engine* p_engine = nullptr;
    };
} // MiniEngine

#endif //MINIENGINE_SCENE_H
