//
// Created by matheus on 15/09/2026.
//

#include "graphics.h"

namespace MiniEngine
{
    void Graphics::setClearColor(const uint32_t newColor)
    {
        color = newColor;
    }

    uint32_t Graphics::color = 0x000000FF;
} // MiniAudio