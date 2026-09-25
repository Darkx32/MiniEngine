//
// Created by matheus on 13/09/2026.
//

#include "entity.h"

namespace MiniEngine
{
    Entity::Entity(const entt::entity entity, Scene* scene)
    {
        m_entity = entity;
        p_scene = scene;

        addComponent<Transform>();
        addComponent<ScriptComponent>();

    }
} // MiniEngine