//
// Created by matheus on 15/09/2026.
//

#ifndef MINIENGINE_VECTOR3_H
#define MINIENGINE_VECTOR3_H

namespace MiniEngine
{
    struct Vector3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;

        constexpr Vector3() = default;
        constexpr Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
        explicit constexpr Vector3(const float scalar) : x(scalar), y(scalar), z(scalar) {}

        [[nodiscard]] const float* Data() const { return &x; }
        float* Data() { return &x; }

        Vector3 operator+(const Vector3& rhs) const { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
        Vector3 operator-(const Vector3& rhs) const { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
        Vector3 operator*(const float scalar) const { return {x * scalar, y * scalar, z * scalar}; }
        Vector3 operator/(const float scalar) const { return {x / scalar, y / scalar, z / scalar}; }

        Vector3& operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; return *this; }
        Vector3& operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        Vector3& operator*=(const float scalar) { x *= scalar; y *= scalar; return *this; }
        Vector3& operator/=(const float scalar) { x /= scalar; y /= scalar; return *this; }

        Vector3 operator-() const { return {-x, -y, -z}; }

        bool operator==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y; }
        bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }

        [[nodiscard]] float length() const;
        [[nodiscard]] float lengthSquared() const;
        [[nodiscard]] Vector3 normalized() const;
        void normalize();

        [[nodiscard]] float dot(const Vector3& rhs) const;
        [[nodiscard]] float DistanceTo(const Vector3& rhs) const;

        static Vector3 lerp(const Vector3& a, const Vector3& b, float t);
        static Vector3 Reflect(const Vector3& v, const Vector3& normal);

        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 Up;
        static const Vector3 Right;
    };

    inline Vector3 operator*(const float scalar, const Vector3& vec) {
        return vec * scalar;
    }
} // MiniEngine

#endif //MINIENGINE_VECTOR3_H
