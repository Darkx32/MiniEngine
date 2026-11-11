//
// Created by matheus on 13/09/2026.
//

#include "miniengine/core/scene.h"
#include "miniengine/ecs/entity.h"
#include "miniengine/engine.h"

namespace MiniEngine
{
    Scene::Scene(engine* engine) : registry(std::make_unique<entt::registry>())
    {
        this->p_engine = engine;
    }

    void Scene::initialize()
    {
    }

    engine* Scene::getEngine() const
    {
        return p_engine;
    }

    Entity Scene::createEntity()
    {
        const entt::entity entity = registry->create();
        registry->emplace<Transform>(entity);
        registry->emplace<ScriptComponent>(entity);
        return Entity(entity, this, registry.get());
    }
} // MiniEngine
