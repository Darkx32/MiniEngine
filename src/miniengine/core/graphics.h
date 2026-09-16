//
// Created by matheus on 15/09/2026.
//

#ifndef MINIENGINE_GRAPHICS_H
#define MINIENGINE_GRAPHICS_H
#include <cstdint>

namespace MiniEngine
{
    enum BgfxIdRender
    {
        DEFAULT,
        UI
    };

    class Graphics
    {
    public:
        static void setClearColor(uint32_t newColor);

    private:
        friend class Engine;
        static uint32_t color;
    };
} // MiniAudio

#endif //MINIENGINE_GRAPHICS_H
