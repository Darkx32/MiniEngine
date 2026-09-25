//
// Created by matheus on 24/09/2026.
//

#ifndef MINIENGINE_PHYSICS_H
#define MINIENGINE_PHYSICS_H
#include <box2d/types.h>

namespace MiniEngine
{
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

        explicit RigidBody(const Transform& transform, ShapeType type = ShapeType::Box);
        ~RigidBody();

        [[nodiscard]] bool isValid() const;
        [[nodiscard]] b2BodyId getId() const {return id;}

    private:
        ShapeType type;
        b2BodyId id{};
    };
} // MiniEngine

#endif //MINIENGINE_PHYSICS_H
