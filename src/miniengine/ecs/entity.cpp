//
// Created by matheus on 13/09/2026.
//

#include "entity.h"

namespace MiniEngine
{
    Entity::Entity(const entt::entity entity, Scene* scene, entt::registry* registry)
    {
        m_entity = entity;
        p_scene = scene;
        p_registry = registry;

        if (!hasComponent<Transform>())
            addComponent<Transform>();
        if (!hasComponent<ScriptComponent>())
            addComponent<ScriptComponent>();
    }
} // MiniEngine
