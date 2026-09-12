//
// Created by matheus on 12/09/2026.
//

#include "vector2.h"

#include <cmath>

namespace MiniEngine
{
    const Vector2 Vector2::Zero  = {  0.0f,  0.0f };
    const Vector2 Vector2::One   = {  1.0f,  1.0f };
    const Vector2 Vector2::Up    = {  0.0f,  1.0f };
    const Vector2 Vector2::Right = {  1.0f,  0.0f };

    float Vector2::length() const {
        return std::sqrt(lengthSquared());
    }

    float Vector2::lengthSquared() const {
        return x * x + y * y;
    }

    Vector2 Vector2::normalized() const {
        if (const float lenSq = lengthSquared(); lenSq > 0.00001f)
            if (lenSq > 0.00001f) {
                const float invLen = 1.0f / std::sqrt(lenSq);
                return { x * invLen, y * invLen };
            }
        return Zero;
    }

    void Vector2::normalize() {
        *this = normalized();
    }

    float Vector2::dot(const Vector2& rhs) const {
        return x * rhs.x + y * rhs.y;
    }

    float Vector2::DistanceTo(const Vector2& rhs) const {
        return (*this - rhs).length();
    }

    Vector2 Vector2::lerp(const Vector2& a, const Vector2& b, float t) {
        return {
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t
        };
    }

    Vector2 Vector2::Reflect(const Vector2& v, const Vector2& normal) {
        return v - normal * (2.0f * v.dot(normal));
    }
}