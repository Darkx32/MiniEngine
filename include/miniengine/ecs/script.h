//
// Created by matheus on 08/11/2026.
//

#ifndef MINIENGINE_SCRIPT_H
#define MINIENGINE_SCRIPT_H

#include <entt/fwd.hpp>

namespace MiniEngine
{
    class Scene;
    class ResourceManager;
    class Entity;

    struct EntityHandle
    {
        entt::entity id;
        Scene* scene;
        entt::registry* registry;

        [[nodiscard]] Entity toEntity() const;
    };

    class IScript
    {
    public:
        virtual ~IScript() = default;
        virtual void startup() = 0;
        virtual void update(float dt) = 0;

        [[nodiscard]] Entity getEntity() const;

    protected:
        friend class Entity;
        ResourceManager* resourceManager = nullptr;

    private:
        EntityHandle entityHandle{};
    };
}

#endif //MINIENGINE_SCRIPT_H
