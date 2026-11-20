//
// Created by matheus on 15/09/2026.
//

#ifndef MINIENGINE_GRAPHICS_H
#define MINIENGINE_GRAPHICS_H
#include <cstdint>

#include "resourcemanager.h"

namespace MiniEngine
{
    class ResourceManager;
    class engine;

    class Graphics
    {
    public:
        enum BgfxIdRender
        {
            DEFAULT,
            UI,
            DEBUG_UI
        };

        static void setClearColor(uint32_t newColor);

        static ResourceHandle getQuadPrimitiveHandle();
        static ResourceHandle getCirclePrimitiveHandle();
        static ResourceHandle getDefaultShaderHandle();

    private:
        friend class engine;

        struct InitData
        {
            ResourceManager* resourceManager;
            void* nativeWindowHandle;
            void* nativeDisplayType;
            const uint32_t width;
            const uint32_t height;
            const bool type;
        };

        static bool initializePrograms(const InitData& initData);
        static void shutdown();
        static void rendererComponentInit();

        inline static uint32_t color = 0x000000FF;
    };
} // MiniAudio

#endif //MINIENGINE_GRAPHICS_H
