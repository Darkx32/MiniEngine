//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_ENTITY_H
#define MINIENGINE_ENTITY_H

#include "entt/entt.hpp"
#include "miniengine/core/resourcemanager.h"
#include "miniengine/core/scene.h"
#include "miniengine/math/vector3.h"

namespace MiniEngine
{
    class ResourceManager;

    class IScript
    {
    public:
        virtual ~IScript() = default;
        virtual void startup() = 0;
        virtual void update(float dt) = 0;

    protected:
        friend class Entity;

        Entity* entity = nullptr;
        ResourceManager* resourceManager = nullptr;
    };

    struct ScriptComponent
    {
        std::vector<std::unique_ptr<IScript>> scripts;

        ScriptComponent() = default;

        ScriptComponent(const ScriptComponent&) = delete;
        ScriptComponent& operator=(const ScriptComponent&) = delete;

        ScriptComponent(ScriptComponent&&) noexcept = default;
        ScriptComponent& operator=(ScriptComponent&&) noexcept = default;
    };

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

        void addScript(std::unique_ptr<IScript> script)
        {
            script->entity = this;
            script->resourceManager = &ResourceManager::get();
            auto& [scripts] = getComponent<ScriptComponent>();
            scripts.emplace_back(
                std::move(script)
            );
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            static_assert(!std::is_base_of_v<IScript, T>, "Scripts must be added using addScript()");
            return p_scene->registry->emplace<T>(m_entity, std::forward<Args>(args)...);
        }

        template <typename T>
        void removeComponent() const
        {
            p_scene->registry->remove<T>(m_entity);
        }

        template <typename T>
        T& getComponent()
        {
            assert(hasComponent<T>() && "Entity does not have that component");
            return p_scene->registry->get<T>(m_entity);
        }

        template <typename T>
        [[nodiscard]] bool hasComponent() const
        {
            return p_scene->registry->all_of<T>(m_entity);
        }

    private:
        friend class Scene;
        explicit Entity(entt::entity entity, Scene* scene);

        entt::entity m_entity{0};
        Scene* p_scene{};
    };
} // MiniEngine

#endif //MINIENGINE_ENTITY_H
