//
// Created by matheus on 18/10/2026.
//

#include "miniengine/systems/shadowsystem.h"

#include "miniengine/ecs/lights.h"
#include "miniengine/ecs/entity.h"
#include "miniengine/ecs/renderer.h"

namespace MiniEngine
{
    static Vector2 transformPoint(const Vector2& point, const Transform& transform)
    {
        const float angle = transform.rotation.z;

        const float c = std::cos(angle);
        const float s = std::sin(angle);

        const float x = point.x * transform.scale.x;
        const float y = point.y * transform.scale.y;

        const float rotatedX = x * c - y * s;
        const float rotatedY = x * s + y * c;

        return {
            rotatedX + transform.position.x,
            rotatedY + transform.position.y
        };
    }

    static std::vector<ShadowSegment> createBoxSegments(const Shadow2D& shadow, const Transform& transform)
    {
        std::array<Vector2, 4> vertices = {
            {
                {-1.0f, -1.0f},
                {1.0f, -1.0f},
                {1.0f, 1.0f},
                {-1.0f, 1.0f}
            }
        };

        for (auto& vertex : vertices)
        {
            vertex.x += shadow.offset.x;
            vertex.y += shadow.offset.y;

            vertex = transformPoint(vertex, transform);
        }

        std::vector<ShadowSegment> segments;
        segments.reserve(4);

        for (size_t i = 0; i < vertices.size(); ++i)
        {
            const size_t next = (i + 1) % vertices.size();

            segments.push_back({
                .start = vertices[i],
                .end = vertices[next]
            });
        }

        return segments;
    }

    static std::vector<ShadowSegment> createCircleSegments(const Shadow2D& shadow, const Transform& transform)
    {
        constexpr int SEGMENTS = 16;

        std::vector<ShadowSegment> segments;
        segments.reserve(SEGMENTS);

        for (int i = 0; i < SEGMENTS; ++i)
        {
            const float angle1 =
                (2.0f * std::numbers::pi_v<float> * static_cast<float>(i)) / SEGMENTS;

            const float angle2 =
                (2.0f * std::numbers::pi_v<float> * static_cast<float>(i + 1)) / SEGMENTS;

            Vector2 start = {
                std::cos(angle1) + shadow.offset.x,
                std::sin(angle1) + shadow.offset.y
            };

            Vector2 end = {
                std::cos(angle2) + shadow.offset.x,
                std::sin(angle2) + shadow.offset.y
            };

            start = transformPoint(start, transform);
            end = transformPoint(end, transform);

            segments.push_back({
                .start = start,
                .end = end
            });
        }

        return segments;
    }

    void ShadowSystem::prepare(entt::registry* registry, const entt::entity renderEntity, const ShaderData& shaderData)
    {
        std::array<std::array<float, 4>, MAX_SHADOW_SEGMENTS> segments = {};

        uint16_t index = 0;

        std::array ignore = {-1.0f, 0.0f, 0.0f, 0.0f};

        for (const auto view = registry->view<const Transform, const Shadow2D>(); const auto entity : view)
        {
            const auto& transform = view.get<Transform>(entity);
            const auto& shadow = view.get<Shadow2D>(entity);

            if (!shadow.enabled)
                continue;

            if (index + 4 > MAX_SHADOW_SEGMENTS)
                break;

            std::vector<ShadowSegment> segmentsCreated;
            switch (shadow.shape)
            {
            default:
            case ShadowShape2D::Box:
                {
                    segmentsCreated = createBoxSegments(shadow, transform);
                    break;
                }
            case ShadowShape2D::Circle:
                {
                    segmentsCreated = createCircleSegments(shadow, transform);
                    break;
                }
            }

            const uint16_t segmentStart = index;

            for (const auto& [start, end] : segmentsCreated)
            {
                segments[index++] = {
                    start.x,
                    start.y,
                    end.x,
                    end.y
                };
            }

            if (entity == renderEntity)
            {
                ignore[0] = static_cast<float>(segmentStart);
                ignore[1] = static_cast<float>(index - segmentStart);
            }
        }

        if (index > 0)
        {
            bgfx::setUniform(
                bgfx::UniformHandle{shaderData.ushadowSegments_idx},
                segments.data(),
                index
            );
        }

        const std::array shadowCount = {
            static_cast<float>(index),
            0.0f,
            0.0f,
            0.0f
        };

        bgfx::setUniform(
            bgfx::UniformHandle{shaderData.ushadowCount_idx},
            shadowCount.data()
        );

        bgfx::setUniform(
            bgfx::UniformHandle{shaderData.ushadowIgnore_idx},
            ignore.data()
        );
    }
} // MiniEngine
