//
// Created by matheus on 24/09/2026.
//

#include "physics.h"

#include "entity.h"
#include "physicssystem.h"
#include "miniengine/math/vector2.h"

namespace MiniEngine
{
    RigidBody::RigidBody(const Transform& transform, const BodyType bodyType, Collider collider)
    {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = getBodyType(bodyType);
        bodyDef.position = {.x = transform.position.x, .y = transform.position.y};
        bodyDef.rotation = b2MakeRot(-transform.rotation.z);

        id = b2CreateBody(PhysicsSystem::getWorldId(), &bodyDef);

        collider.id = createShape(transform, collider);

        colliders.push_back(collider);
    }

    RigidBody::~RigidBody()
    {
        if (isValid())
            b2DestroyBody(id);
    }

    void RigidBody::addForce(const Vector2& force) const
    {
        b2Body_ApplyForceToCenter(id, {.x = force.x, .y = force.y}, true);
    }

    void RigidBody::setLinearVelocity(const Vector2& linear) const
    {
        b2Body_SetLinearVelocity(id, {.x = linear.x, .y = linear.y});
    }

    void RigidBody::setMass(const float mass) const
    {
        const b2MassData massData = b2Body_GetMassData(id);
        const float area = massData.mass / b2Shape_GetDensity(colliders[0].id);

        b2Shape_SetDensity(colliders[0].id, mass / area, true);
    }

    float RigidBody::getMass() const
    {
        const b2MassData massData = b2Body_GetMassData(id);
        return massData.mass;
    }

    void RigidBody::setGravityScale(const float gravity) const
    {
        b2Body_SetGravityScale(id, gravity);
    }

    void RigidBody::setType(const BodyType bodyType) const
    {
        b2Body_SetType(id, getBodyType(bodyType));
    }

    Collider RigidBody::getCollider() const
    {
        return colliders[0];
    }

    void RigidBody::setCollider(const Collider& newCollider, const Transform& transform)
    {
        auto& collider = colliders[0];

        b2DestroyShape(collider.id, false);
        collider.id = createShape(transform, newCollider);
    }

    bool RigidBody::isValid() const
    {
        return b2Body_IsValid(id);
    }

    b2BodyType RigidBody::getBodyType(const BodyType bodyType)
    {
        switch (bodyType)
        {
        case BodyType::Dynamic:
            return b2_dynamicBody;
        case BodyType::Static:
            return b2_staticBody;
        case BodyType::Kinematic:
            return b2_kinematicBody;
        default:
            return b2_dynamicBody;
        }
    }

    b2ShapeId RigidBody::createShape(const Transform& transform, const Collider& collider) const
    {
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = collider.density;
        shapeDef.material.friction = collider.friction;

        switch (collider.type)
        {
        default:
        case ShapeType::Box:
            {
                const b2Polygon box = b2MakeBox(transform.scale.x / 2, transform.scale.y / 2);
                return b2CreatePolygonShape(id, &shapeDef, &box);
            }
        case ShapeType::Circle:
            {
                const b2Circle circle{
                    .center = {.x = 0.0f, .y = 0.0f}, .radius = transform.scale.x
                };
                return b2CreateCircleShape(id, &shapeDef, &circle);
            }
        case ShapeType::Capsule:
            {
                const float radius = transform.scale.x / 2.f;
                const float halfHeight = (transform.scale.y - 2.0f * radius) / 2.0f;

                b2Capsule capsule{};
                capsule.center1 = {.x = 0.0f, .y = -halfHeight};
                capsule.center2 = {.x = 0.0f, .y = halfHeight};
                capsule.radius = radius;
                return b2CreateCapsuleShape(id, &shapeDef, &capsule);
            }
        }
    }
} // MiniEngine
