//
// Created by matheus on 24/09/2026.
//

#ifndef MINIENGINE_PHYSICS_H
#define MINIENGINE_PHYSICS_H
#include <box2d/types.h>

namespace MiniEngine
{
    struct Vector2;
    struct Transform;

    class RigidBody
    {
    public:
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

        explicit RigidBody(const Transform& transform, BodyType bodyType = BodyType::Dynamic, ShapeType shapeType = ShapeType::Box);
        ~RigidBody();

        void addForce(const Vector2& force) const;
        void setGravityScale(float gravity) const;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] b2BodyId getId() const {return id;}

    private:
        b2BodyId id{};
    };
} // MiniEngine

#endif //MINIENGINE_PHYSICS_H
