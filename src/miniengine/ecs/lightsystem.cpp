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
        float positions[MAX_LIGHTS][4] = {};
        float colors[MAX_LIGHTS][4] = {};
        float params[MAX_LIGHTS][4] = {};

        for (const auto spotView = registry->view<const Transform, const PointLight2D>(); const auto entity : spotView)
        {
            const auto& transform = spotView.get<Transform>(entity);
            const auto& light = spotView.get<PointLight2D>(entity);

            if (!light.enabled)
                continue;

            if (index >= MAX_LIGHTS)
                break;

            positions[index][0] = transform.position.x;
            positions[index][1] = transform.position.y;
            positions[index][2] = transform.position.z;
            positions[index][3] = 0.0f;

            colors[index][0] = light.color[0];
            colors[index][1] = light.color[1];
            colors[index][2] = light.color[2];
            colors[index][3] = light.intensity;

            params[index][0] = light.radius;
            params[index][1] = light.falloff;

            ++index;
        }

        if (index > 0)
        {
            bgfx::setUniform(
                bgfx::UniformHandle{shaderData.ulightPositions_idx},
                positions,
                index);

            bgfx::setUniform(
                bgfx::UniformHandle{shaderData.ulightColors_idx},
                colors,
                index);

            bgfx::setUniform(
                bgfx::UniformHandle{shaderData.ulightParams_idx},
                params,
                index);
        }

        const float count[4] = {static_cast<float>(index), 0.0f, 0.0f, 0.0f};

        bgfx::setUniform(
            bgfx::UniformHandle{shaderData.ulightCount_idx},
            count);

        constexpr float ambient[4] = {1.0f, 1.0f, 1.0f, 0.0f};

        bgfx::setUniform(
            bgfx::UniformHandle{shaderData.uambientColor_idx},
            ambient);
    }
} // MiniEngine
