//
// Created by matheus on 15/09/2026.
//

#include "graphics.h"

#include "resourcemanager.h"
#include "bgfx/bgfx.h"
#include "miniengine/Engine.h"
#include "miniengine/ecs/renderer.h"

namespace MiniEngine
{
    void Graphics::setClearColor(const uint32_t newColor)
    {
        color = newColor;
    }

    void Graphics::initializePrograms(const Engine* engine)
    {
        rendererComponentInit();
        QuadPrimitiveID = engine->getResourceManager()->create<QuadPrimitive>();
    }

    uint32_t Graphics::color = 0x000000FF;
    uint16_t Graphics::QuadPrimitiveID = 0xFFFF;
} // MiniAudio