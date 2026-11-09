//
// Created by matheus on 13/09/2026.
//

#include "miniengine/core/scene.h"
#include "miniengine/ecs/entity.h"

namespace MiniEngine
{
    Scene::Scene()
    {
        registry = std::make_unique<entt::registry>();
    }

    void Scene::initialize()
    {
    }

    Entity Scene::createEntity()
    {
        const entt::entity entity = registry->create();
        registry->emplace<Transform>(entity);
        registry->emplace<ScriptComponent>(entity);
        return Entity(entity, this, registry.get());
    }
} // MiniEngine
