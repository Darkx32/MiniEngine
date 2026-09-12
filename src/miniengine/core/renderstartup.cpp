//
// Created by matheus on 12/09/2026.
//

#include "renderstartup.h"

#include <bgfx/bgfx.h>
#include <bx/platform.h>
#include <spdlog/spdlog.h>

namespace MiniEngine
{
    bool initializeRender(void* nativeWindowHandle, void* nativeDisplayType, uint32_t width, uint32_t height)
    {
        bgfx::Init init;
        init.type = bgfx::RendererType::Count;

        init.swapChain.nwh = nativeWindowHandle;
        init.swapChain.ndt = nativeDisplayType;
        init.swapChain.width = width;
        init.swapChain.height = height;
        init.reset = BGFX_RESET_VSYNC;

        if (!bgfx::init(init))
        {
            spdlog::error("Failed to create render");
            return false;
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(width), static_cast<uint16_t>(height));

        return true;
    }

    void shutdownRender()
    {
        bgfx::shutdown();
    }
} // MiniEngine