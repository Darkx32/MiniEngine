//
// Created by matheus on 19/09/2026.
//

#include "scriptsystem.h"

#include "entity.h"
#include "entt/entt.hpp"

namespace MiniEngine
{
    void ScriptSystem::startup(entt::registry* registry)
    {
        registry->view<ScriptComponent>().each(
            [](entt::entity _, const ScriptComponent& scriptComponent)
            {
               for (auto& script : scriptComponent.scripts)
               {
                   script->startup();
               }
            });
    }

    void ScriptSystem::update(entt::registry* registry, float dt)
    {
        registry->view<ScriptComponent>().each(
            [dt](entt::entity _, const ScriptComponent& scriptComponent)
            {
               for (auto& script : scriptComponent.scripts)
               {
                   script->update(dt);
               }
            });
    }
} // MiniEngine