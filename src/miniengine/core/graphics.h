//
// Created by matheus on 15/09/2026.
//

#ifndef MINIENGINE_GRAPHICS_H
#define MINIENGINE_GRAPHICS_H
#include <cstdint>

namespace MiniEngine
{
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

    private:
        friend class Engine;
        static void initializePrograms(const Engine* engine);
        static void rendererComponentInit();

        static uint32_t color;
    };
} // MiniAudio

#endif //MINIENGINE_GRAPHICS_H
