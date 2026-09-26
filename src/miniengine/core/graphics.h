//
// Created by matheus on 15/09/2026.
//

#ifndef MINIENGINE_GRAPHICS_H
#define MINIENGINE_GRAPHICS_H
#include <cstdint>

namespace MiniEngine
{
    class ResourceManager;
    class Engine;

    class Graphics
    {
    public:
        enum BgfxIdRender
        {
            DEFAULT,
            UI
        };

        static void setClearColor(uint32_t newColor);

        static uint16_t QuadPrimitiveID;
        static uint16_t CirclePrimitiveID;

    private:
        friend class Engine;
        struct InitData
        {
            ResourceManager* resourceManager;
            void* nativeWindowHandle;
            void* nativeDisplayType;
            const uint32_t width;
            const uint32_t height;
        };
        static bool initializePrograms(const InitData& initData);
        static void shutdown();
        static void rendererComponentInit();

        static uint32_t color;
    };
} // MiniAudio

#endif //MINIENGINE_GRAPHICS_H
