//
// Created by matheus on 15/09/2026.
//

#ifndef MINIENGINE_GRAPHICS_H
#define MINIENGINE_GRAPHICS_H
#include <cstdint>

namespace MiniEngine
{
    struct Mesh
    {
        uint16_t vbh = 0xFFFF;
        uint16_t ibh = 0xFFFF;
        uint16_t program = 0xFFFF;

        uint16_t uColor = 0xFFFF;
    };

    struct CubeMesh : Mesh
    {
        static CubeMesh init();
    };

    class Graphics
    {
    public:
        enum BgfxIdRender
        {
            DEFAULT,
            UI
        };

        static void setClearColor(uint32_t newColor);

        static CubeMesh primitiveCube;

    private:
        friend class Engine;
        static void initializePrograms();
        static void shutdownPrograms();

        static uint32_t color;
    };
} // MiniAudio

#endif //MINIENGINE_GRAPHICS_H
