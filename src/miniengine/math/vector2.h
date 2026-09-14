//
// Created by matheus on 12/09/2026.
//

#ifndef MINIENGINE_VECTOR2_H
#define MINIENGINE_VECTOR2_H

namespace MiniEngine
{
    struct Vector2 {
        float x = 0.0f;
        float y = 0.0f;

        constexpr Vector2() = default;
        constexpr Vector2(const float x, const float y) : x(x), y(y) {}
        explicit constexpr Vector2(const float scalar) : x(scalar), y(scalar) {}

        [[nodiscard]] const float* Data() const { return &x; }
        float* Data() { return &x; }

        Vector2 operator+(const Vector2& rhs) const { return {x + rhs.x, y + rhs.y}; }
        Vector2 operator-(const Vector2& rhs) const { return {x - rhs.x, y - rhs.y}; }
        Vector2 operator*(const float scalar) const { return {x * scalar, y * scalar}; }
        Vector2 operator/(const float scalar) const { return {x / scalar, y / scalar}; }

        Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
        Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        Vector2& operator*=(const float scalar) { x *= scalar; y *= scalar; return *this; }
        Vector2& operator/=(const float scalar) { x /= scalar; y /= scalar; return *this; }

        Vector2 operator-() const { return {-x, -y}; }

        bool operator==(const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
        bool operator!=(const Vector2& rhs) const { return !(*this == rhs); }

        [[nodiscard]] float length() const;
        [[nodiscard]] float lengthSquared() const;
        [[nodiscard]] Vector2 normalized() const;
        void normalize();

        [[nodiscard]] float dot(const Vector2& rhs) const;
        [[nodiscard]] float DistanceTo(const Vector2& rhs) const;

        static Vector2 lerp(const Vector2& a, const Vector2& b, float t);
        static Vector2 Reflect(const Vector2& v, const Vector2& normal);

        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 Up;
        static const Vector2 Right;
    };

    inline Vector2 operator*(float scalar, const Vector2& vec) {
        return vec * scalar;
    }
}

#endif //MINIENGINE_VECTOR2_H
