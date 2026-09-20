//
// Created by matheus on 18/09/2026.
//
#include "renderer.h"

#include "vs_simple.bin.h"
#include "fs_simple.bin.h"
#include "miniengine/math/vector3.h"
#include <utility>

#include "entity.h"
#include "bgfx/bgfx.h"
#include "bx/math.h"
#include "miniengine/core/graphics.h"
#include "miniengine/math/vector2.h"
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
    }
    bgfx::VertexLayout PosColorVertex::ms_layout;

    static std::pair<bgfx::ShaderHandle, bgfx::ShaderHandle> createShaderByRenderer()
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
        return {
            bgfx::createShader(bgfx::makeRef(datav, static_cast<uint32_t>(sizev))),
            bgfx::createShader(bgfx::makeRef(dataf, static_cast<uint32_t>(sizef)))
        };
    }

    QuadPrimitive::QuadPrimitive()
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
            vbh_idx = BGFX_INVALID_HANDLE;
            ibh_idx = BGFX_INVALID_HANDLE;
        }
        else if (!bgfx::isValid(program))
        {
            spdlog::error("Error to create shader program for quad primitive");
            program_idx = BGFX_INVALID_HANDLE;
        }
        else if (!bgfx::isValid(color))
        {
            spdlog::error("Error to create color uniform for quad primitive");
            ucolor_idx = BGFX_INVALID_HANDLE;
        }

        vbh_idx = vbh.idx;
        ibh_idx = ibh.idx;
        program_idx = program.idx;
        ucolor_idx = color.idx;
    }

    QuadPrimitive::~QuadPrimitive()
    {
        if (bgfx::isValid(bgfx::VertexBufferHandle{vbh_idx}))
        {
            bgfx::destroy(bgfx::VertexBufferHandle{vbh_idx});
            vbh_idx = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(bgfx::IndexBufferHandle{ibh_idx}))
        {
            bgfx::destroy(bgfx::IndexBufferHandle{ibh_idx});
            ibh_idx = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(bgfx::ProgramHandle{program_idx}))
        {
            bgfx::destroy(bgfx::ProgramHandle{program_idx});
            program_idx = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(bgfx::UniformHandle{ucolor_idx}))
        {
            bgfx::destroy(bgfx::UniformHandle{ucolor_idx});
            ucolor_idx = BGFX_INVALID_HANDLE;
        }
    }

    void Camera2D::calculate(const Vector2& windowSize)
    {
        const float halfWidth = windowSize.x / (2.0f * zoom);
        const float halfHeight = windowSize.y / (2.0f * zoom);

        bx::mtxOrtho(
        proj,-halfWidth,halfWidth,
         halfHeight,-halfHeight,
        -100.0f,100.0f, 0.0f,bgfx::getCaps()->homogeneousDepth);
    }

    void Camera2D::updateView(const Transform& transform)
    {
        bx::mtxLookAt(view,
    {transform.position.x, transform.position.y, -1.0f},
    {transform.position.x, transform.position.y, 0.0f});
    }

    void Graphics::rendererComponentInit()
    {
        PosColorVertex::init();
    }
}
