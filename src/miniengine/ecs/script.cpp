//
// Created by matheus on 08/11/2026.
//

#include "ecs/entity.h"

namespace MiniEngine
{
    Entity EntityHandle::toEntity() const
    {
        return Entity(id, scene, registry);
    }

    Entity IScript::getEntity() const
    {
        return entityHandle.toEntity();
    }
}
