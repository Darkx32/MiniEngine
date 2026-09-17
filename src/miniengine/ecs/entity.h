//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_ENTITY_H
#define MINIENGINE_ENTITY_H

#include <entt/fwd.hpp>

#include "miniengine/math/vector3.h"

namespace MiniEngine
{
    class Scene;

    struct Transform
    {
        Vector3 position;
        Vector3 scale;
        Vector3 rotation;

        Transform() : position(Vector3::Zero), scale(Vector3::One), rotation(Vector3::Zero) {}
        Transform(const Vector3 pos, const Vector3 s, const Vector3 euler) : position(pos), scale(s), rotation(euler) {}
    };

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
