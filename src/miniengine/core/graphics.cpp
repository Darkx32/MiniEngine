//
// Created by matheus on 15/09/2026.
//

#include "graphics.h"

#include <utility>

#include "bgfx/bgfx.h"
#include "miniengine/math/vector3.h"

#include "vs_simple.bin.h"
#include "fs_simple.bin.h"
#include "spdlog/spdlog.h"

namespace MiniEngine
{
    namespace
    {
        struct PosColorVertex
        {
            Vector3 vector3;

            static bgfx::VertexLayout ms_layout;
            static void init()
            {
                ms_layout.begin()
                    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                    .end();
            }
        };
        bgfx::VertexLayout PosColorVertex::ms_layout;

        std::pair<bgfx::ShaderHandle, bgfx::ShaderHandle> createShaderByRenderer()
        {
            const uint8_t* datav = nullptr;
            const uint8_t* dataf = nullptr;
            size_t sizev = 0;
            size_t sizef = 0;

            switch (bgfx::getRendererType())
            {
            case bgfx::RendererType::OpenGLES:
                datav = vs_simple_essl;
                sizev = sizeof(vs_simple_essl);
                dataf = fs_simple_essl;
                sizef = sizeof(fs_simple_essl);
                break;
            case bgfx::RendererType::OpenGL:
                datav = vs_simple_glsl;
                sizev = sizeof(vs_simple_glsl);
                dataf = fs_simple_glsl;
                sizef = sizeof(fs_simple_glsl);
                break;
            case bgfx::RendererType::Vulkan:
                datav = vs_simple_spv;
                sizev = sizeof(vs_simple_spv);
                dataf = fs_simple_spv;
                sizef = sizeof(fs_simple_spv);
                break;

            default:
                break;
            }

            if (!dataf || !datav)
                return {BGFX_INVALID_HANDLE, BGFX_INVALID_HANDLE};
            else
                return {
                bgfx::createShader(bgfx::makeRef(datav, static_cast<uint32_t>(sizev))),
                bgfx::createShader(bgfx::makeRef(dataf, static_cast<uint32_t>(sizef)))
            };
        }
    }

    CubeMesh CubeMesh::init()
    {
        static PosColorVertex s_vertices[] = {
            {.vector3 = {-0.5f,  0.5f, 0.0f} },
            {.vector3 = {0.5f,  0.5f, 0.0f} },
            {.vector3 = {0.5f, -0.5f, 0.0f} },
            {.vector3 = {-0.5f, -0.5f, 0.0f} },
        };

        static const uint16_t s_indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        const auto vbh = bgfx::createVertexBuffer(
            bgfx::makeRef(s_vertices, sizeof(s_vertices)),
            PosColorVertex::ms_layout);

        const auto ibh = bgfx::createIndexBuffer(
            bgfx::makeRef(s_indices, sizeof(s_indices)));

        const auto [vsh, fsh] = createShaderByRenderer();

        const auto program = bgfx::createProgram(vsh, fsh, true);

        const auto color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);

        if (!bgfx::isValid(vbh) || !bgfx::isValid(ibh))
        {
            spdlog::error("Error to create vertices and indices for quad primitive");
            return {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
        }
        if (!bgfx::isValid(program))
        {
            spdlog::error("Error to create shader program for quad primitive");
            return {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
        }
        if (!bgfx::isValid(color))
        {
            spdlog::error("Error to create color uniform for quad primitive");
            return {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
        }

        return {vbh.idx, ibh.idx, program.idx, color.idx};
    }

    void Graphics::setClearColor(const uint32_t newColor)
    {
        color = newColor;
    }

    void Graphics::initializePrograms()
    {
        PosColorVertex::init();
        primitiveCube = CubeMesh::init();
    }

    void Graphics::shutdownPrograms()
    {
        // QuadPrimitive
        bgfx::destroy(bgfx::VertexBufferHandle{primitiveCube.vbh});
        bgfx::destroy(bgfx::IndexBufferHandle{primitiveCube.ibh});
        bgfx::destroy(bgfx::ProgramHandle{primitiveCube.program});
        bgfx::destroy(bgfx::UniformHandle{primitiveCube.uColor});
    }

    uint32_t Graphics::color = 0x000000FF;
    CubeMesh Graphics::primitiveCube = {};
} // MiniAudio