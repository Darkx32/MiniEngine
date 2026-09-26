//
// Created by matheus on 24/09/2026.
//

#include "physicssystem.h"

#include "entity.h"
#include "physics.h"
#include "box2d/box2d.h"
#include "entt/entity/registry.hpp"

namespace MiniEngine
{
    void PhysicsSystem::initialize()
    {
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = {.x = 0.0f, .y = -9.8f};

        id = b2CreateWorld(&worldDef);
    }

    void PhysicsSystem::update(entt::registry* registry, const float dt)
    {
        constexpr int subStepCount = 4;

        b2World_Step(id, dt, subStepCount);

        registry->view<Transform, const RigidBody>().each(
            [](Transform& transform, const RigidBody& rigidbody)
        {
            const auto id = rigidbody.getId();

            const auto [x, y] = b2Body_GetPosition(id);
            const auto rotation = b2Body_GetRotation(id);

            transform.position.x = x;
            transform.position.y = y;
            transform.rotation.z = b2Rot_GetAngle(rotation);
        });
    }

    void PhysicsSystem::shutdown()
    {
        b2DestroyWorld(id);
    }

    b2WorldId PhysicsSystem::id = b2_nullWorldId;
} // MiniEngine