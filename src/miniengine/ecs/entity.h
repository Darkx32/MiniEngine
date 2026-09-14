//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_ENTITY_H
#define MINIENGINE_ENTITY_H

#include <entt/fwd.hpp>

namespace MiniEngine
{
    class Scene;

    class Entity
    {
    public:
        Entity() = default;

        template<typename T, typename... Args>
        T& addComponent(Args&&... args);

        template<typename T>
        void removeComponent() const;

        template<typename T>
        T& getComponent();

        template<typename T>
        [[nodiscard]] bool hasComponent() const;

    private:
        friend class Scene;
        explicit Entity(entt::entity entity, Scene* scene);

        entt::entity m_entity{0};
        Scene* p_scene{};
    };
} // MiniEngine

#endif //MINIENGINE_ENTITY_H
