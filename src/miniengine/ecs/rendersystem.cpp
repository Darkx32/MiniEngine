//
// Created by matheus on 16/09/2026.
//

#include "rendersystem.h"

#include "entity.h"
#include "renderer.h"
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "entt/entt.hpp"
#include "miniengine/core/graphics.h"

namespace MiniEngine
{
    void RenderSystem::calculate(entt::registry* registry, const Vector2& windowSize)
    {
        if (const auto registryView = registry->view<Transform, Camera2D>(); registryView.begin() != registryView.end())
        {
            const auto entity = *registryView.begin();

            auto [transform, camera2d] =
                registryView.get<Transform, Camera2D>(entity);

            camera2d.calculate(windowSize);
        }
    }

    void RenderSystem::render(entt::registry* registry)
    {
        float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        if (const auto registryView = registry->view<Transform, Camera2D>(); registryView.begin() != registryView.end())
        {
            const auto entity = *registryView.begin();

            auto [transform, camera2d] =
                registryView.get<Transform, Camera2D>(entity);

            camera2d.updateView(transform);
            bgfx::setViewTransform(Graphics::DEFAULT, camera2d.view, camera2d.proj);
        }

        bgfx::touch(Graphics::DEFAULT);

        registry->view<const Transform, const MeshRenderer>().each(
            [&white](const Transform& transform, const MeshRenderer& mesh)
            {
                const bgfx::VertexBufferHandle vbh = {mesh.meshData->vbh_idx};
                const bgfx::IndexBufferHandle ibh = {mesh.meshData->ibh_idx};
                const bgfx::ProgramHandle program = {mesh.meshData->program_idx};
                const bgfx::UniformHandle color = {mesh.meshData->ucolor_idx};

                float model[16];

                bx::mtxSRT(model,
                transform.scale.x, transform.scale.y, transform.scale.z,
                transform.rotation.x, transform.rotation.y, transform.rotation.z,
                transform.position.x, transform.position.y, transform.position.z);

                bgfx::setTransform(model);

                bgfx::setUniform(color, white);

                bgfx::setVertexBuffer(0, vbh);
                bgfx::setIndexBuffer(ibh);
                bgfx::setState(BGFX_STATE_DEFAULT);
                bgfx::submit(Graphics::DEFAULT, program);
            });
    }
} // MiniEngine