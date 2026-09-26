//
// Created by matheus on 15/09/2026.
//

#include "graphics.h"

#include "resourcemanager.h"
#include "bgfx/bgfx.h"
#include "miniengine/Engine.h"
#include "miniengine/ecs/renderer.h"
#include "spdlog/spdlog.h"

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

                    spdlog::debug("[BGFX][{}:{}] {}", _filePath, _line, msg);
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

    void Graphics::setClearColor(const uint32_t newColor)
    {
        color = newColor;
    }

    bool Graphics::initializePrograms(const InitData& initData)
    {
        bgfx::Init init;
        init.type = bgfx::RendererType::Count;

        init.callback = &bgfx_callback;
        init.swapChain.nwh = initData.nativeWindowHandle;
        init.swapChain.ndt = initData.nativeDisplayType;
        init.swapChain.width = initData.width;
        init.swapChain.height = initData.height;
        init.reset = BGFX_RESET_NONE;

        if (!bgfx::init(init))
        {
            spdlog::error("Failed to create render");
            return false;
        }

        bgfx::setViewRect(DEFAULT, 0, 0, static_cast<uint16_t>(initData.width), static_cast<uint16_t>(initData.height));

        rendererComponentInit();
        QuadPrimitiveID = initData.resourceManager->create<QuadPrimitive>();
        CirclePrimitiveID = initData.resourceManager->create<CirclePrimitive>();

        return true;
    }

    void Graphics::shutdown()
    {
        bgfx::shutdown();
    }

    uint32_t Graphics::color = 0x000000FF;
    uint16_t Graphics::QuadPrimitiveID = 0xFFFF;
    uint16_t Graphics::CirclePrimitiveID = 0xFFFF;
} // MiniAudio