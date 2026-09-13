//
// Created by matheus on 12/09/2026.
//

#include "renderstartup.h"

#include <bgfx/bgfx.h>
#include <spdlog/spdlog.h>

namespace MiniEngine
{
    namespace
    {
        class MiniEngineBGFXCallback : public bgfx::CallbackI
        {
        public:
            void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override
            {
                char tempBuffer[2048];

                va_list argsCopy;
                va_copy(argsCopy, _argList);
                const int written = std::vsnprintf(tempBuffer, sizeof(tempBuffer), _format, argsCopy);
                va_end(argsCopy);

                if (written > 0)
                {
                    std::string_view msg(tempBuffer, static_cast<size_t>(written));

                    while (!msg.empty() && (msg.back() == '\n' || msg.back() == '\r'))
                    {
                        msg.remove_suffix(1);
                    }

                    spdlog::info("[BGFX][{}:{}] {}", _filePath, _line, msg);
                }
            }

            void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override
            {
                spdlog::critical("[BGFX][{}:{}] Code: {} - {}", _filePath, _line, static_cast<int>(_code), _str);

                spdlog::default_logger()->flush();

                std::abort();
            }

            void profilerBegin(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override {}
            void profilerBeginLiteral(const char* _name, uint32_t _abgr, const char* _filePath, uint16_t _line) override {}
            void profilerEnd() override {}
            uint32_t cacheReadSize(uint64_t _id) override {return 0;}
            bool cacheRead(uint64_t _id, void* _data, uint32_t _size) override { return false; }
            void cacheWrite(uint64_t _id, const void* _data, uint32_t _size) override {}
            void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, bgfx::TextureFormat::Enum _format,
                const void* _data, uint32_t _size, bool _yflip) override {}
            void captureBegin(uint32_t _width, uint32_t _height, uint32_t _pitch, bgfx::TextureFormat::Enum _format, bool _yflip) override {}
            void captureEnd() override {}
            void captureFrame(const void* _data, uint32_t _size) override {}
        };
    }

    static MiniEngineBGFXCallback bgfx_callback;

    bool RenderStartup::initializeRender(void* nativeWindowHandle, void* nativeDisplayType, uint32_t width, uint32_t height)
    {
        bgfx::Init init;
        init.type = bgfx::RendererType::Count;

        init.callback = &bgfx_callback;
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

    void RenderStartup::shutdownRender()
    {
        bgfx::shutdown();
    }
} // MiniEngine