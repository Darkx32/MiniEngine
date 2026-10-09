//
// Created by matheus on 24/09/2026.
//

#ifndef MINIENGINE_PHYSICS_H
#define MINIENGINE_PHYSICS_H

namespace MiniEngine
{
    struct Vector2;
    struct Transform;

    enum class ShapeType
    {
        Box,
        Circle,
        Capsule
    };

    enum class BodyType
    {
        Dynamic,
        Static,
        Kinematic
    };

    struct Collider
    {
        ShapeType type = ShapeType::Box;

        float friction = 1.0f;
        float restitution = 1.0f;
        float density = 1.0f;
        bool isSensor = false;

        b2ShapeId id{};
    };

    class RigidBody
    {
    public:
        explicit RigidBody(const Transform& transform, BodyType bodyType = BodyType::Dynamic,
                           Collider collider = {});
        ~RigidBody();
        RigidBody(const RigidBody&) = delete;
        RigidBody& operator=(const RigidBody&) = delete;

        void addForce(const Vector2& force) const;
        void setLinearVelocity(const Vector2& linear) const;
        void setMass(float mass) const;
        [[nodiscard]] float getMass() const;
        void setGravityScale(float gravity) const;
        void setType(BodyType bodyType) const;
        [[nodiscard]] Collider getCollider() const;
        void setCollider(const Collider& newCollider, const Transform& transform);
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] b2BodyId getId() const { return id; }

    private:
        [[nodiscard]] static b2BodyType getBodyType(BodyType bodyType);
        [[nodiscard]] b2ShapeId createShape(const Transform& transform, const Collider& collider) const;
        std::vector<Collider> colliders;

        b2BodyId id{};
    };
} // MiniEngine

#endif //MINIENGINE_PHYSICS_H
