//
// Created by matheus on 24/09/2026.
//

#ifndef MINIENGINE_PHYSICSSYSTEM_H
#define MINIENGINE_PHYSICSSYSTEM_H
#include "entt/fwd.hpp"
#include "box2d/types.h"

namespace MiniEngine
{
    class PhysicsSystem
    {
    public:
        static void initialize();
        static void update(entt::registry* registry, float dt);
        static void shutdown();

        static b2WorldId getWorldId() {return id;}

    private:
        static b2WorldId id;
    };
} // MiniEngine

#endif //MINIENGINE_PHYSICSSYSTEM_H
