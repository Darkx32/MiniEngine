//
// Created by matheus on 24/09/2026.
//

#include "physics.h"

#include "entity.h"
#include "physicssystem.h"
#include "box2d/box2d.h"

namespace MiniEngine
{
    RigidBody::RigidBody(const Transform& transform, const ShapeType type) : type(type)
    {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = {.x = transform.position.x, .y = transform.position.y};

        id = b2CreateBody(PhysicsSystem::getWorldId(), &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.material.friction = 0.3f;

        switch (type)
        {
        case ShapeType::Box:
            {
                const b2Polygon box = b2MakeBox(transform.scale.x / 2, transform.scale.y / 2);
                b2CreatePolygonShape(id, &shapeDef, &box);
                break;
            }
        case ShapeType::Circle:
            {
                const b2Circle circle{
                    .center = {.x = 0.0f, .y = 0.0f}, .radius = transform.scale.x / 2.f};
                b2CreateCircleShape(id, &shapeDef, &circle);
                break;
            }
        case ShapeType::Capsule:
            {
                const float radius = transform.scale.x / 2.f;
                const float halfHeight = (transform.scale.y - 2.0f * radius) / 2.0f;

                b2Capsule capsule{};
                capsule.center1 = {.x = 0.0f, .y = -halfHeight};
                capsule.center2 = {.x = 0.0f, .y = halfHeight};
                capsule.radius = radius;
                b2CreateCapsuleShape(id, &shapeDef, &capsule);
                break;
            }
        }
    }

    RigidBody::~RigidBody()
    {
        if (isValid())
            b2DestroyBody(id);
    }

    bool RigidBody::isValid() const
    {
        return b2Body_IsValid(id);
    }
} // MiniEngine