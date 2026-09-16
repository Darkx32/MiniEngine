//
// Created by matheus on 15/09/2026.
//

#include "vector3.h"

#include <cmath>

#include "bx/math.h"

namespace MiniEngine
{
    namespace
    {
        bx::Vec3 toBx(const Vector3& v)
        {
            return {v.x, v.y, v.z};
        }

        Vector3 toVector3(const bx::Vec3& v)
        {
            return {v.x, v.y, v.z};
        }
    }

    const Vector3 Vector3::Zero  = {  0.0f,  0.0f, 0.0f };
    const Vector3 Vector3::One   = {  1.0f,  1.0f, 1.0f };
    const Vector3 Vector3::Up    = {  0.0f,  1.0f, 1.0f };
    const Vector3 Vector3::Right = {  1.0f,  0.0f, 0.0f };

    float Vector3::length() const {
        return std::sqrt(lengthSquared());
    }

    float Vector3::lengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vector3 Vector3::normalized() const {
        return toVector3(bx::normalize(toBx(*this)));
    }

    void Vector3::normalize() {
        *this = normalized();
    }

    float Vector3::dot(const Vector3& rhs) const {
        const auto vec3a = toBx(*this);
        const auto vec3b = toBx(rhs);

        return bx::dot(vec3a, vec3b);
    }

    float Vector3::DistanceTo(const Vector3& rhs) const {
        const auto vec3a = toBx(*this);
        const auto vec3b = toBx(rhs);

        return bx::distance(vec3a, vec3b);
    }

    Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, const float t) {
        const auto vec3a = toBx(a);
        const auto vec3b = toBx(b);

        return toVector3(bx::lerp(vec3a, vec3b, t));
    }

    Vector3 Vector3::Reflect(const Vector3& v, const Vector3& normal) {
        const auto vec3a = toBx(v);
        const auto vec3b = toBx(normal);

        const float d = 2.0f * bx::dot(vec3a, vec3b);

        return toVector3(bx::sub(vec3a, bx::mul(vec3b, d)));
    }
} // MiniEngine