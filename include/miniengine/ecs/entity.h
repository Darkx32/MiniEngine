//
// Created by matheus on 13/09/2026.
//

#ifndef MINIENGINE_ENTITY_H
#define MINIENGINE_ENTITY_H

#include "miniaudio.h"
#include "script.h"
#include "entt/entt.hpp"
#include "miniengine/core/resourcemanager.h"
#include "miniengine/math/vector3.h"

namespace MiniEngine
{
    class ResourceManager;
    class Scene;

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

        Transform() : position(Vector3::Zero), scale(Vector3::One), rotation(Vector3::Zero)
        {
        }

        Transform(const Vector3 pos, const Vector3 s, const Vector3 euler) : position(pos), scale(s), rotation(euler)
        {
        }
    };

    struct AudioStream
    {
        explicit AudioStream(const std::string& filepath);
        ~AudioStream();

        void play();
        [[nodiscard]] bool isPlaying() const;
        void stop();

        ma_sound stream;
        std::string filepath;
        std::string group = "master";
        bool autoPlay = false;
    };

    class Entity
    {
    public:
        Entity() = default;
        explicit Entity(entt::entity entity, Scene* scene, entt::registry* registry);

        [[nodiscard]] Scene* getScene() const { return p_scene; }

        void addScript(std::unique_ptr<IScript> script) const
        {
            script->entityHandle = {
                .id = m_entity,
                .scene = p_scene,
                .registry = p_registry
            };

            script->resourceManager = &ResourceManager::get();

            auto& [scripts] = getComponent<ScriptComponent>();
            scripts.emplace_back(std::move(script));
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args) const
        {
            static_assert(!std::is_base_of_v<IScript, T>, "Scripts must be added using addScript()");
            return p_registry->emplace<T>(m_entity, std::forward<Args>(args)...);
        }

        template <typename T>
        void removeComponent() const
        {
            p_registry->remove<T>(m_entity);
        }

        template <typename T>
        [[nodiscard]] T& getComponent() const
        {
            assert(hasComponent<T>() && "Entity does not have that component");
            return p_registry->get<T>(m_entity);
        }

        template <typename T>
        [[nodiscard]] bool hasComponent() const
        {
            return p_registry->all_of<T>(m_entity);
        }

    private:
        friend class Scene;

        entt::entity m_entity{0};
        Scene* p_scene{};
        entt::registry* p_registry = nullptr;
    };
} // MiniEngine

#endif //MINIENGINE_ENTITY_H
