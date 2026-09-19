//
// Created by matheus on 13/09/2026.
//

#include "scene.h"
#include "miniengine/ecs/entity.h"

namespace MiniEngine
{
    Scene::Scene() : registry(new entt::registry) {}

    Scene::~Scene()
    {
        delete registry;
    }

    Entity Scene::createEntity()
    {
        const entt::entity entity = registry->create();
        return Entity(entity, this);
    }
} // MiniEngine