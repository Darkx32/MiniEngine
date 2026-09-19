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
    void RenderSystem::render(entt::registry* registry)
    {
        float model[16];
        float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        registry->view<const Transform, const MeshRenderer>().each(
            [&model, &white](entt::entity _, const Transform& transform, const MeshRenderer& mesh)
            {
                const bgfx::VertexBufferHandle vbh = {mesh.meshData->vbh_idx};
                const bgfx::IndexBufferHandle ibh = {mesh.meshData->ibh_idx};
                const bgfx::ProgramHandle program = {mesh.meshData->program_idx};
                const bgfx::UniformHandle color = {mesh.meshData->ucolor_idx};

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