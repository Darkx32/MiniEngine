//
// Created by matheus on 01/10/2026.
//

#include "debugui.h"

#include <algorithm>

#include "graphics.h"
#include "miniengine_shaders.hpp"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "../pch.h"

namespace MiniEngine
{
    namespace
    {
        struct ImDrawVert
        {
            [[maybe_unused]] ImVec2 pos;
            [[maybe_unused]] ImVec2 uv;
            [[maybe_unused]] ImU32 col{};

            static bgfx::VertexLayout layout;
            static void init()
            {
                layout.begin()
                    .add(bgfx::Attrib::Position, 2,
                         bgfx::AttribType::Float)

                    .add(bgfx::Attrib::TexCoord0, 2,
                         bgfx::AttribType::Float)

                    .add(bgfx::Attrib::Color0, 4,
                         bgfx::AttribType::Uint8,
                         true)

                    .end();
            }
        };
    }
    bgfx::VertexLayout ImDrawVert::layout;

    void DebugUI::initialize(SDL_Window* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();

        unsigned char* pixels;
        int width;
        int height;

        io.Fonts->GetTexDataAsRGBA32(
            &pixels,
            &width,
            &height
        );

        ImGui_ImplSDL3_InitForOther(window);

        ImDrawVert::init();
        const auto [idx] = Shaders::createProgram("vs_imgui","fs_imgui");
        idx_program = idx;

        const auto [texture_idx] = bgfx::createTexture2D(
            static_cast<uint16_t>(width),
            static_cast<uint16_t>(height),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_SAMPLER_NONE,
            bgfx::copy(
                pixels,
                width * height * 4
            )
        );
        idx_texture = texture_idx;
        io.Fonts->SetTexID(texture_idx);

        const auto [sampler_idx] = bgfx::createUniform(
            "s_tex",
            bgfx::UniformType::Sampler
        );
        idx_sampler = sampler_idx;
    }

    void DebugUI::update(const SDL_Event* event)
    {
        ImGui_ImplSDL3_ProcessEvent(event);
    }

    void DebugUI::render()
    {
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("MiniEngine");

        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        const bgfx::Stats* stats = bgfx::getStats();

        ImGui::SeparatorText("BGFX Timers");

        const uint64_t gpuTime = stats->gpuTimeEnd - stats->gpuTimeBegin;

        ImGui::Text("CPU frame time: %.2f ms", static_cast<double>(stats->cpuTimeFrame) / static_cast<double>(stats->cpuTimerFreq) * 1000);
        ImGui::Text("GPU frame time: %.2f ms", static_cast<double>(gpuTime) / static_cast<double>(stats->gpuTimerFreq) * 1000);
        ImGui::Text("Wait render time: %.2f ms", static_cast<double>(stats->waitRender) / static_cast<double>(stats->cpuTimerFreq) * 1000);
        ImGui::Text("Wait submit time: %.2f ms", static_cast<double>(stats->waitSubmit) / static_cast<double>(stats->cpuTimerFreq) * 1000);

        ImGui::SeparatorText("Memory");

        ImGui::Text("Texture memory usage: %.2f MB", static_cast<double>(stats->textureMemoryUsed) / (1024 * 1024));
        ImGui::Text("Render memory usage: %.2f MB", static_cast<double>(stats->rtMemoryUsed) / (1024 * 1024));
        ImGui::Text("GPU vram usage: %.2f/%.2f MB", static_cast<double>(stats->gpuMemoryUsed) / (1024 * 1024), static_cast<double>(stats->gpuMemoryMax) / (1024 * 1024));

        ImGui::SeparatorText("Counters");

        ImGui::Text("Draw calls: %u", stats->numDraw);
        ImGui::Text("Textures: %u", stats->numTextures);
        ImGui::Text("Shaders: %u", stats->numShaders);
        ImGui::Text("Programs: %u", stats->numPrograms);

        ImGui::End();
        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();
        if (!drawData || drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f) return;

        const float fbWidth = drawData->DisplaySize.x * drawData->FramebufferScale.x;
        const float fbHeight = drawData->DisplaySize.y * drawData->FramebufferScale.y;
        if (fbWidth <= 0.0f || fbHeight <= 0.0f) return;

        bgfx::setViewRect(
            Graphics::DEBUG_UI,
            0,
            0,
            static_cast<uint16_t>(fbWidth),
            static_cast<uint16_t>(fbHeight)
        );

        float projection[16];
        constexpr uint64_t state =
              BGFX_STATE_WRITE_RGB
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_MSAA
            | BGFX_STATE_BLEND_FUNC(
                  BGFX_STATE_BLEND_SRC_ALPHA,
                  BGFX_STATE_BLEND_INV_SRC_ALPHA
              );

        const float left   = drawData->DisplayPos.x;
        const float right  = drawData->DisplayPos.x + drawData->DisplaySize.x;
        const float top    = drawData->DisplayPos.y;
        const float bottom = drawData->DisplayPos.y + drawData->DisplaySize.y;

        bx::mtxOrtho(
            projection,
            left,
            right,
            bottom,
            top,
            0.0f,
            1000.0f,
            0.0f,
            bgfx::getCaps()->homogeneousDepth
        );

        bgfx::setViewTransform(
            Graphics::DEBUG_UI,
            nullptr,
            projection
        );

        for (int listIndex = 0; listIndex < drawData->CmdListsCount; ++listIndex)
        {
            const ImDrawList* drawList = drawData->CmdLists[listIndex];

            const auto numVertices = static_cast<uint32_t>(drawList->VtxBuffer.Size);

            const auto numIndices = static_cast<uint32_t>(drawList->IdxBuffer.Size);
            if (bgfx::getAvailTransientVertexBuffer(numVertices, ImDrawVert::layout) < numVertices)
                break;

            if (bgfx::getAvailTransientIndexBuffer(numIndices, sizeof(ImDrawIdx) == 4) < numIndices)
                break;

            bgfx::TransientVertexBuffer tvb{};
            bgfx::TransientIndexBuffer tib{};

            bgfx::allocTransientVertexBuffer(
                &tvb,
                numVertices,
                ImDrawVert::layout
            );

            bgfx::allocTransientIndexBuffer(
                &tib,
                numIndices,
                sizeof(ImDrawIdx) == 4
            );

            memcpy(
                tvb.data,
                drawList->VtxBuffer.Data,
                drawList->VtxBuffer.Size * sizeof(ImDrawVert)
            );

            memcpy(
                tib.data,
                drawList->IdxBuffer.Data,
                drawList->IdxBuffer.Size * sizeof(ImDrawIdx)
            );

            for (const ImDrawCmd& cmd : drawList->CmdBuffer)
            {
                if (cmd.UserCallback != nullptr)
                {
                    if (cmd.UserCallback == ImDrawCallback_ResetRenderState) {}
                    else
                        cmd.UserCallback(drawList, &cmd);

                    continue;
                }

                const ImVec2 clipOffset = drawData->DisplayPos;
                const ImVec2 clipScale  = drawData->FramebufferScale;

                float clipMinX =
                    (cmd.ClipRect.x - clipOffset.x) * clipScale.x;

                float clipMinY =
                    (cmd.ClipRect.y - clipOffset.y) * clipScale.y;

                float clipMaxX =
                    (cmd.ClipRect.z - clipOffset.x) * clipScale.x;

                float clipMaxY =
                    (cmd.ClipRect.w - clipOffset.y) * clipScale.y;

                if (clipMaxX <= clipMinX || clipMaxY <= clipMinY) continue;
                clipMinX = std::max(clipMinX, 0.0f);
                clipMinY = std::max(clipMinY, 0.0f);

                clipMaxX = std::min(clipMaxX, fbWidth);
                clipMaxY = std::min(clipMaxY, fbHeight);

                bgfx::setScissor(
                    static_cast<uint16_t>(clipMinX),
                    static_cast<uint16_t>(clipMinY),
                    static_cast<uint16_t>(clipMaxX - clipMinX),
                    static_cast<uint16_t>(clipMaxY - clipMinY)
                );

                bgfx::setState(state);
                bgfx::setTexture(
                    0,
                    {idx_sampler},
                    {idx_texture}
                );

                bgfx::setVertexBuffer(
                    0,
                    &tvb,
                    cmd.VtxOffset,
                    numVertices - cmd.VtxOffset
                );

                bgfx::setIndexBuffer(
                    &tib,
                    cmd.IdxOffset,
                    cmd.ElemCount
                );

                bgfx::submit(
                    Graphics::DEBUG_UI,
                    {idx_program}
                );
            }
        }
    }

    void DebugUI::shutdown()
    {
        if (const auto program = bgfx::ProgramHandle{idx_program}; bgfx::isValid(program))
            bgfx::destroy(program);
        if (const auto sample = bgfx::UniformHandle{idx_sampler}; bgfx::isValid(sample))
            bgfx::destroy(sample);
        if (const auto texture = bgfx::TextureHandle{idx_texture}; bgfx::isValid(texture))
            bgfx::destroy(texture);

        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    uint16_t DebugUI::idx_program = BGFX_INVALID_HANDLE;
    uint16_t DebugUI::idx_sampler = BGFX_INVALID_HANDLE;
    uint16_t DebugUI::idx_texture = BGFX_INVALID_HANDLE;
} // MiniEngine