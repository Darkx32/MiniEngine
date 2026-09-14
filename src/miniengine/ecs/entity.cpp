//
// Created by matheus on 13/09/2026.
//

#include "entity.h"
#include <entt/entt.hpp>

#include "miniengine/core/scene.h"

namespace MiniEngine
{
    Entity::Entity(const entt::entity entity, Scene* scene)
    {
        m_entity = entity;
        p_scene = scene;
    }

    template <typename T, typename ... Args>
    T& Entity::addComponent(Args&&... args)
    {
        return p_scene->registry->emplace<T>(m_entity, std::forward<Args>(args)...);
    }

    template <typename T>
    void Entity::removeComponent() const
    {
        p_scene->registry->remove<T>(m_entity);
    }

    template <typename T>
    T& Entity::getComponent()
    {
        assert(getComponent<T>() && "Entity does not have that component");
        return p_scene->registry->get<T>(m_entity);
    }

    template <typename T>
    bool Entity::hasComponent() const
    {
        return p_scene->registry->all_of<T>(m_entity);
    }
} // MiniEngine