//
// Created by matheus on 12/09/2026.
//

#ifndef MINIENGINE_RENDERSTARTUP_H
#define MINIENGINE_RENDERSTARTUP_H
#include <cstdint>

namespace MiniEngine
{
    bool initializeRender(void* nativeWindowHandle, void* nativeDisplayType, uint32_t width, uint32_t height);
    void shutdownRender();
} // MiniEngine

#endif //MINIENGINE_RENDERSTARTUP_H
