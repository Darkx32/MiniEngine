//
// Created by matheus on 12/10/2026.
//

#include "lightsystem.h"

#include "entity.h"
#include "lights.h"
#include "renderer.h"

namespace MiniEngine
{
    void LightSystem::prepare(entt::registry* registry, const ShaderData& shaderData)
    {
        uint16_t index = 0;
        std::array<std::array<float, MAX_LIGHTS>, 4> positions = {};
        std::array<std::array<float, MAX_LIGHTS>, 4> colors = {};
        std::array<std::array<float, MAX_LIGHTS>, 4> params = {};

        for (const auto pointView = registry->view<const Transform, const PointLight2D>();
             const auto entity : pointView)
        {
            const auto& transform = pointView.get<Transform>(entity);
            const auto& [color, intensity, radius,
                falloff, enabled] = pointView.get<PointLight2D>(entity);

            if (!enabled)
                continue;

            if (index >= MAX_LIGHTS)
                break;

            positions[index][0] = transform.position.x;
            positions[index][1] = transform.position.y;
            positions[index][2] = 0.0f;
            positions[index][3] = 0.0f;

            colors[index][0] = color.x;
            colors[index][1] = color.y;
            colors[index][2] = color.z;
            colors[index][3] = intensity;

            params[index][0] = radius;
            params[index][1] = falloff;

            ++index;
        }

        for (const auto spotView = registry->view<const Transform, const SpotLight2D>();
             const auto entity : spotView)
        {
            const auto& transform = spotView.get<Transform>(entity);
            const auto& [color, intensity, radius, innerAngle,
                outerAngle, falloff, enabled] = spotView.get<SpotLight2D>(entity);

            if (!enabled)
                continue;

            if (index >= MAX_LIGHTS)
                break;

            const float angle = transform.rotation.z;
            const float directionX = std::cos(angle);
            const float directionY = std::sin(angle);

            positions[index][0] = transform.position.x;
            positions[index][1] = transform.position.y;
            positions[index][2] = directionX;
            positions[index][3] = directionY;

            colors[index][0] = color.x;
            colors[index][1] = color.y;
            colors[index][2] = color.z;
            colors[index][3] = intensity;

            params[index][0] = radius;
            params[index][1] = falloff;
            params[index][2] = std::cos(innerAngle);
            params[index][3] = std::cos(outerAngle);

            ++index;
        }

        if (index > 0)
        {
            bgfx::setUniform(
                bgfx::UniformHandle{shaderData.ulightPositions_idx},
                positions.data(),
                index);

            bgfx::setUniform(
                bgfx::UniformHandle{shaderData.ulightColors_idx},
                colors.data(),
                index);

            bgfx::setUniform(
                bgfx::UniformHandle{shaderData.ulightParams_idx},
                params.data(),
                index);
        }

        const std::array count = {static_cast<float>(index), 0.0f, 0.0f, 0.0f};

        bgfx::setUniform(
            bgfx::UniformHandle{shaderData.ulightCount_idx},
            count.data());

        std::array<float, 4> ambient = {};

        if (const auto ambientView = registry->view<const AmbientLight2D>(); ambientView.begin() !=
            ambientView.end())
        {
            const auto entity = *ambientView.begin();

            const auto [color, intensity] = ambientView.get<AmbientLight2D>(entity);

            ambient = {
                color.x, color.y, color.z, intensity
            };
        }
        else
        {
            ambient = {1.0f, 1.0f, 1.0f, 0.0f};
        }

        bgfx::setUniform(
            bgfx::UniformHandle{shaderData.uambientColor_idx},
            ambient.data());
    }
} // MiniEngine
