//
// Created by matheus on 16/09/2026.
//

#include "../systems/rendersystem.h"

#include "shadowsystem.h"
#include "../ecs/entity.h"
#include "../systems/lightsystem.h"
#include "../ecs/renderer.h"
#include "miniengine/core/graphics.h"

namespace MiniEngine
{
    void RenderSystem::calculate(entt::registry* registry, const Vector2& windowSize)
    {
        if (const auto registryView = registry->view<const Transform, Camera2D>(); registryView.begin() != registryView.
            end())
        {
            const auto entity = *registryView.begin();

            auto [transform, camera2d] =
                registryView.get<Transform, Camera2D>(entity);

            camera2d.calculate(windowSize);
        }
    }

    void RenderSystem::render(entt::registry* registry)
    {
        if (const auto registryView = registry->view<const Transform, Camera2D>(); registryView.begin() != registryView.
            end())
        {
            const auto entity = *registryView.begin();

            auto [transform, camera2d] =
                registryView.get<Transform, Camera2D>(entity);

            camera2d.updateView(transform);
            bgfx::setViewTransform(Graphics::DEFAULT, camera2d.view, camera2d.proj);
        }

        bgfx::touch(Graphics::DEFAULT);

        registry->view<const Transform, const MeshRenderer>().each(
            [&registry](const entt::entity entity, const Transform& transform, const MeshRenderer& mesh)
            {
                auto& resourceManager = ResourceManager::get();
                const auto* p_meshData = resourceManager.get<MeshData>(mesh.meshData);
                const auto* p_shaderData = resourceManager.get<ShaderData>(p_meshData->shaderData);
                const auto& p_material = resourceManager.get<Material>(mesh.material);

                const bgfx::VertexBufferHandle vbh = {p_meshData->vbh_idx};
                const bgfx::IndexBufferHandle ibh = {p_meshData->ibh_idx};
                const bgfx::ProgramHandle program = {p_shaderData->program_idx};
                const bgfx::UniformHandle color = {p_shaderData->ucolor_idx};

                float model[16];

                bx::mtxSRT(model,
                           transform.scale.x, transform.scale.y, transform.scale.z,
                           transform.rotation.x, transform.rotation.y, transform.rotation.z,
                           transform.position.x, transform.position.y, transform.position.z);

                bgfx::setTransform(model);

                ShadowSystem::prepare(registry, entity, *p_shaderData);
                LightSystem::prepare(registry, transform, *p_shaderData);
                bgfx::setUniform(color, p_material->color.data());

                bgfx::setVertexBuffer(0, vbh);
                bgfx::setIndexBuffer(ibh);
                bgfx::setState(BGFX_STATE_DEFAULT);
                bgfx::submit(Graphics::DEFAULT, program);
            });
    }
} // MiniEngine
